
static const char *module_name = "QINGSTOR";

static struct fsal_staticfsinfo_t default_qingstor_info = {
	.maxfilesize = UINT64_MAX,
	.maxlink = _POSIX_LINK_MAX,
	.maxnamelen = 1024,
	.maxpathlen = 1024,
	.no_trunc = true,
	.chown_restricted = false,
	.case_insensitive = false,
	.case_preserving = true,
	.link_support = false,
	.symlink_support = false,
	.lock_support = false,
	.lock_support_owner = false,
	.lock_support_async_block = false,
	.named_attr = true, /* XXX */
	.unique_handles = true,
	.lease_time = {10, 0},
	.acl_support = false,
	.cansettime = true,
	.homogenous = true,
	.supported_attrs = RGW_SUPPORTED_ATTRIBUTES,
	.maxread = FSAL_MAXIOSIZE,
	.maxwrite = FSAL_MAXIOSIZE,
	.umask = 0,
	.rename_changes_key = true,
};

/* Module methods
 */


static struct config_item qingstor_items[] = {
	CONF_ITEM_PATH("qingstor_conf", 1, MAXPATHLEN, NULL,
		rgw_fsal_module, conf_path),
	CONF_ITEM_STR("name", 1, MAXPATHLEN, NULL,
		rgw_fsal_module, name),
	CONF_ITEM_STR("cluster", 1, MAXPATHLEN, NULL,
		rgw_fsal_module, cluster),
	CONF_ITEM_STR("init_args", 1, MAXPATHLEN, NULL,
		rgw_fsal_module, init_args),
	CONF_ITEM_MODE("umask", 0,
			rgw_fsal_module, fs_info.umask),
	CONF_ITEM_MODE("xattr_access_rights", 0,
			rgw_fsal_module, fs_info.xattr_access_rights),
	CONFIG_EOL
};


struct config_block qingstor_block = {
	.dbus_interface_name = "org.ganesha.nfsd.config.fsal.qingstor",
	.blk_desc.name = "QINGSTOR",
	.blk_desc.type = CONFIG_BLOCK,
	.blk_desc.u.blk.init = noop_conf_init,
	.blk_desc.u.blk.params = qingstor_items,
	.blk_desc.u.blk.commit = noop_conf_commit
};


/* init_config
 * must be called with a reference taken (via lookup_fsal)
 */

static fsal_status_t init_config(struct fsal_module *module_in,
				config_file_t config_struct,
				struct config_error_type *err_type)
{
	struct qingstor_fsal_module *myself =
	    container_of(module_in, struct qingstor_fsal_module, fsal);

	LogDebug(COMPONENT_FSAL,
		 "QINGSTOR module setup.");

	myself->fs_info = default_qingstor_info;
	(void) load_config_from_parse(config_struct,
				      &qingstor_block,
				      myself,
				      true,
				      err_type);
	if (!config_error_is_harmless(err_type))
		return fsalstat(ERR_FSAL_INVAL, 0);

	return fsalstat(ERR_FSAL_NO_ERROR, 0);
}

 /**
 * @brief Indicate support for extended operations.
 *
 * @retval true if extended operations are supported.
 */

bool support_ex(struct fsal_obj_handle *obj)
{
	return true;
}


/**
 * @brief Create a new export under this FSAL
 *
 * This function creates a new export object for the QINGSTOR FSAL.
 *
 * @todo ACE: We do not handle re-exports of the same cluster in a
 * sane way.  Currently we create multiple handles and cache objects
 * pointing to the same one.  This is not necessarily wrong, but it is
 * inefficient.  It may also not be something we expect to use enough
 * to care about.
 *
 * @param[in]     module_in  The supplied module handle
 * @param[in]     path       The path to export
 * @param[in]     options    Export specific options for the FSAL
 * @param[in,out] list_entry Our entry in the export list
 * @param[in]     next_fsal  Next stacked FSAL
 * @param[out]    pub_export Newly created FSAL export object
 *
 * @return FSAL status.
 */

static struct config_item export_params[] = {
	CONF_ITEM_NOOP("name"),
	CONF_MAND_STR("user_id", 0, MAXUIDLEN, NULL,
		      qingstor_export, qingstor_user_id),
	CONF_MAND_STR("access_key_id", 0, MAXKEYLEN, NULL,
		      qingstor_export, qingstor_access_key_id),
	CONF_MAND_STR("secret_access_key", 0, MAXSECRETLEN, NULL,
		      qingstor_export, qingstor_secret_access_key),
	CONFIG_EOL
};

static struct config_block export_param_block = {
	.dbus_interface_name = "org.ganesha.nfsd.config.fsal.qingstor-export%d",
	.blk_desc.name = "FSAL",
	.blk_desc.type = CONFIG_BLOCK,
	.blk_desc.u.blk.init = noop_conf_init,
	.blk_desc.u.blk.params = export_params,
	.blk_desc.u.blk.commit = noop_conf_commit
};

static fsal_status_t create_export(struct fsal_module *module_in,
				   void *parse_node,
				   struct config_error_type *err_type,
				   const struct fsal_up_vector *up_ops)
{
	/* The status code to return */
	fsal_status_t status = { ERR_FSAL_NO_ERROR, 0 };
	/* The internal export object */
	struct qingstor_export *export = NULL;
	/* The 'private' root handle */
	struct qingstor_handle *handle = NULL;
	/* Stat for root */
	struct stat st;
	/* Return code */
	int rc = 0;
	/* Return code from RGW calls */
	int qingstor_status;
	/* True if we have called fsal_export_init */
	bool initialized = false;

	/* once */
	if (!QSFSM.libqs) {
		PTHREAD_MUTEX_lock(&init_mtx);
		if (!QSFSM.libqs) {
			char *conf_path = NULL;
			char *inst_name = NULL;
			char *cluster = NULL;

			int argc = 1;
			char *argv[5] = { "nfs-ganesha",
					  NULL, NULL, NULL, NULL };
			int clen;

			if (QSFSM.conf_path) {
				clen = strlen(RGWFSM.conf_path) + 8;
				conf_path = (char *) gsh_malloc(clen);
				sprintf(conf_path, "--conf=%s",
					RGWFSM.conf_path);
				argv[argc] = conf_path;
				++argc;
			}

			if (QSFSM.name) {
				clen = strlen(QSFSM.name) + 8;
				inst_name = (char *) gsh_malloc(clen);
				sprintf(inst_name, "--name=%s", RGWFSM.name);
				argv[argc] = inst_name;
				++argc;
			}

			/*
			if (RGWFSM.cluster) {
				clen = strlen(RGWFSM.cluster) + 8;
				cluster = (char *) gsh_malloc(clen);
				sprintf(cluster, "--cluster=%s",
					RGWFSM.cluster);
				argv[argc] = cluster;
				++argc;
			}
			*/

			if (RGWFSM.init_args) {
				argv[argc] = RGWFSM.init_args;
				++argc;
			}

			// 初始化lib库
			rc = librgw_create(&RGWFSM.rgw, argc, argv);
			if (rc != 0) {
				LogCrit(COMPONENT_FSAL,
					"QINGSTOR module: librqs init failed (%d)",
					rc);
			}

			if (conf_path)
				gsh_free(conf_path);
			if (inst_name)
				gsh_free(inst_name);
			/*
			if (cluster)
				gsh_free(cluster);
				*/
		}
		PTHREAD_MUTEX_unlock(&init_mtx);
	}

	if (rc != 0) {
		status.major = ERR_FSAL_BAD_INIT;
		goto error;
	}

	export = gsh_calloc(1, sizeof(struct qingstor_export));
	if (export == NULL) {
		status.major = ERR_FSAL_NOMEM;
		LogCrit(COMPONENT_FSAL,
			"Unable to allocate export object for %s.",
			op_ctx->ctx_export->fullpath);
		goto error;
	}

	fsal_export_init(&export->export);
	export_ops_init(&export->export.exp_ops);

	/* get params for this export, if any */
	if (parse_node) {
		rc = load_config_from_node(parse_node,
					   &export_param_block,
					   export,
					   true,
					   err_type);

		if (rc != 0) {
			gsh_free(export);
			return fsalstat(ERR_FSAL_INVAL, 0);
		}
	}

	initialized = true;

	// 挂载得到rgw_fs，即
	// 本质上是为了得到qingstor_fs结构并记录在export中，将来使用
	/*
	rgw_status = rgw_mount(RGWFSM.rgw,
			export->rgw_user_id,
			export->qingstor_access_key_id,
			export->qingstor_secret_access_key,
			&export->qingstor_fs,
			RGW_MOUNT_FLAG_NONE);
	if (rgw_status != 0) {
		status.major = ERR_FSAL_SERVERFAULT;
		LogCrit(COMPONENT_FSAL,
			"Unable to mount RGW cluster for %s.",
			op_ctx->ctx_export->fullpath);
		if (rgw_status == -EINVAL) {
			LogCrit(COMPONENT_FSAL,
			"Authorization Failed for user %s ",
			export->rgw_user_id);
		}
		goto error;
	}
	*/

	// 把我们的export插入到fasl链（棧）中
	if (fsal_attach_export(module_in, &export->export.exports) != 0) {
		status.major = ERR_FSAL_SERVERFAULT;
		LogCrit(COMPONENT_FSAL,
			"Unable to attach export for %s.",
			op_ctx->ctx_export->fullpath);
		goto error;
	}

	// 注册object失效函数
	if (rgw_register_invalidate(export->rgw_fs, rgw_fs_invalidate,
					up_ops->up_fsal_export,
					RGW_REG_INVALIDATE_FLAG_NONE) != 0) {
		LogCrit(COMPONENT_FSAL,
			"Unable to register invalidates for %s.",
			op_ctx->ctx_export->fullpath);
		goto error;
	}

	export->export.fsal = module_in;

	LogDebug(COMPONENT_FSAL,
		 "QINGSTOR module export %s.",
		 op_ctx->ctx_export->fullpath);

	rc = qingstor_getattr(export->rgw_fs, export->rgw_fs->root_fh, &st,
			RGW_GETATTR_FLAG_NONE);
	if (rc < 0)
		return rgw2fsal_error(rc);

	rc = construct_handle(export, export->rgw_fs->root_fh, &st, &handle);
	if (rc < 0) {
		status = rgw2fsal_error(rc);
		goto error;
	}

	op_ctx->fsal_export = &export->export;

	export->root = handle;
	export->export.up_ops = up_ops;

	return status;

 error:
	if (export) {
		gsh_free(export);
	}

	if (initialized)
		initialized = false;

	return status;
}

/**
 * @brief Initialize and register the FSAL
 *
 * This function initializes the FSAL module handle, being called
 * before any configuration or even mounting of a RGW cluster.  It
 * exists solely to produce a properly constructed FSAL module
 * handle.
 */

MODULE_INIT void init(void)
{
	struct fsal_module *myself = &QSFSM.fsal;

	LogDebug(COMPONENT_FSAL,
		 "QINGSTOR module registering.");

	/* register_fsal seems to expect zeroed memory. */
	memset(myself, 0, sizeof(*myself));

	if (register_fsal(myself, module_name, FSAL_MAJOR_VERSION,
			  FSAL_MINOR_VERSION, FSAL_ID_QINGSTOR) != 0) {
		/* The register_fsal function prints its own log
		   message if it fails */
		LogCrit(COMPONENT_FSAL,
			"QINGSTOR module failed to register.");
	}

	/* Set up module operations */
	myself->m_ops.create_export = create_export;
	myself->m_ops.init_config = init_config;
	myself->m_ops.support_ex = support_ex;
}

/**
 * @brief Release FSAL resources
 *
 * This function unregisters the FSAL and frees its module handle.  The
 * FSAL also has an open instance of the rgw library, so we also need to
 * release that.
 */

MODULE_FINI void finish(void)
{
	int ret;

	LogDebug(COMPONENT_FSAL,
		 "QINGSTOR module finishing.");

	ret = unregister_fsal(&QSFSM.fsal);
	if (ret != 0) {
		LogCrit(COMPONENT_FSAL,
			"QINGSTOR: unregister_fsal failed (%d)", ret);
	}

	/* release the library */

	//if (QSFSM.rgw) {
		/////???????????
		//librgw_shutdown(QSFSM.rgw);
	//}
}
