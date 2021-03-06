
const string root_name = "/";


//  typedef std::tuple<QsFileHandle*, uint32_t> LookupFHResult;
enum QsFsError
{
	QS_FS_ERR_NO_ERROR = 0,
	QS_FS_ERR_UNKNOWN,
	QS_FS_ERR_NTWORK_ERROR,
	QS_FS_ERR_FILE_NO_EXIST,
	QS_FS_ERR_DIR_NO_EXIST,
	QS_FS_ERR_FILE_ALEADY_EXIST,
	QS_FS_ERR_DIR_ALEADY_EXIST,
	QS_FS_ERR_PATH_TOO_LONG,
	QS_FS_ERR_INVAILD_PARENT_PATH,
};

typedef std::string ObjectKey;


class QsFHResult{
public:	
	QsFHResult(QsFsErr err):m_err(err), m_fh(nullptr){};
	QsFHResult(QsFileHandle* fh ):m_err(QsFsError::QS_FS_ERR_NO_ERROR), m_fh(fh){};
	QsFsErr getErr(){ return m_err};
	, nullptr * getFH(){ return m_fh};
private:
    QsFileHandle* m_fh;
    QsFsErr m_err;	
};


 /*
   * XXX
   * The current 64-bit, non-cryptographic hash used here is intended
   * for prototyping only.
   *
   * However, the invariant being prototyped is that objects be
   * identifiable by their hash components alone.  We believe this can
   * be legitimately implemented using 128-hash values for bucket and
   * object components, together with a cluster-resident cryptographic
   * key.  Since an MD5 or SHA-1 key is 128 bits and the (fast),
   * non-cryptographic CityHash128 hash algorithm takes a 128-bit seed,
   * speculatively we could use that for the final hash computations.
   */
  struct QsFHKey
  {
    qingstor_fh_hk fh_hk;
    //uint32_t version;

    static constexpr uint64_t seed = 8675309;

    QsFHKey(const qingstor_fh_hk& _hk)
      : fh_hk(_hk){
      // nothing
    }

    QsFHKey(const uint64_t bk, const uint64_t ok){
      fh_hk.bucket = bk;
      fh_hk.object = ok;
    }

    QsFHKey(const uint64_t bk, const char *_o)
      : version(0) {
      fh_hk.bucket = bk;
      fh_hk.object = XXH64(_o, ::strlen(_o), seed);
    }
    
    QsFHKey(const std::string& _b, const std::string& _o)
      : version(0) {
      fh_hk.bucket = XXH64(_b.c_str(), _o.length(), seed);
      fh_hk.object = XXH64(_o.c_str(), _o.length(), seed);
    }

  }; /* QsFHKey */

class QsFileHandle
{
	struct qingstor_file_handle fh;
      //std::mutex mtx;

    QsFileSystem* fs;
    QsFileHandle* bucket;
    QsFileHandle* parent;
   	ObjectKey name; /* XXX file or bucket name */
   	QsFHKey fhk;

    ObjectKey lastMarker;

    static constexpr uint32_t QS_FS_FLAG_NONE =    0x0000;
    static constexpr uint32_t QS_FS_FLAG_OPEN =    0x0001;
    static constexpr uint32_t QS_FS_FLAG_ROOT =    0x0002;
    static constexpr uint32_t QS_FS_FLAG_CREATE_BACKEND =  0x0004;
    static constexpr uint32_t QS_FS_FLAG_CREATING =  0x0008;
    static constexpr uint32_t QS_FS_FLAG_DIRECTORY = 0x0010;
    static constexpr uint32_t QS_FS_FLAG_BUCKET = 0x0020;
    static constexpr uint32_t QS_FS_FLAG_LOCK =   0x0040;
    static constexpr uint32_t QS_FS_FLAG_DELETED = 0x0080;
    static constexpr uint32_t QS_FS_FLAG_UNLINK_THIS = 0x0100;
    static constexpr uint32_t QS_FS_FLAG_LOCKED = 0x0200;
    static constexpr uint32_t QS_FS_FLAG_STATELESS_OPEN = 0x0400;
    static constexpr uint32_t QS_FS_FLAG_EXACT_MATCH = 0x0800;


    struct State {
      uint64_t dev;
      uint64_t size;
      uint64_t nlink;
      uint32_t owner_uid; /* XXX need Unix attr */
      uint32_t owner_gid; /* XXX need Unix attr */
      mode_t unix_mode;
      struct timespec ctime;
      struct timespec mtime;
      struct timespec atime;
      State() : dev(0), size(0), nlink(1), owner_uid(0), owner_gid(0),
    ctime{0,0}, mtime{0,0}, atime{0,0} {}
    } state;


    void clearState();



    QsFHKey makeFHKey(const std::string& name) const {
        if (depth <= 1)
            return QsFHKey(fhk.fh_hk.object, name.c_str());
        else {
            std::string key_name = makeKeyName(name.c_str());
            return QsFHKey(fhk.fh_hk.bucket, key_name.c_str());
        }
    }

    const QsFHKey& getKey() const {
      return fhk;
    }

    // fileContent;
    // child 
    struct file {
      RGWWriteRequest* write_req;
      file() : write_req(nullptr) {}
      ~file();
    };

    struct directory {

      static constexpr uint32_t FLAG_NONE =     0x0000;

      //uint32_t flags;
      //rgw_obj_key last_marker;

      // no gc ,so bo need last_readdir
      //struct timespec last_readdir;

      directory() : flags(FLAG_NONE), last_readdir{0,0} {}
    };

    const qingstor_file_handle getWrapperHandle() const { return fh; };
    const std::string& getObjectName() const { return name; };
   	const std::string getObjectKey(bool omit_bucket = false) const;

   	int readdir(rgw_readdir_cb rcb, void *cb_arg, uint64_t *offset, bool *eof,
		uint32_t flags);

	void addMarker(uint64_t off, const ObjectKey& marker) {
		if (off > 0) {
			//unique_lock guard(mtx);
			lastMarker = marker;
		}
	}

    const ObjectKey findMarker(uint64_t off) const {
      if (off > 0) {
        	  return lastMarker;
      }
      return "";
    }






private:
      QsFileHandle(QsFileSystem* _fs, uint32_t fs_inst)
      : fs(_fs), bucket(nullptr), parent(nullptr), depth(0), flags(FLAG_ROOT)
      {
          /* root */
          fh.fh_type = QS_FS_TYPE_DIRECTORY;
          //variant_type = directory();
          /* stat */
          state.dev = fs_inst;
          state.unix_mode = QS_RWXMODE|S_IFDIR;
          /* pointer to self */
          fh.fh_private = this;
      }

      inline std::string makeKeyName(const char *name) const {
        std::string strkeyName = getObjectKey();
        if (strkeyName.length() > 0)
            strkeyName += "/";
        strkeyName += name;
        return strkeyName;
    }

    void initRootFS(std::string& fsid, const std::string& objectName) {
      /* fh_key */
      fh.fh_hk.bucket = XXH64(fsid.c_str(), fsid.length(), QsFHKey::seed);
      fh.fh_hk.object = XXH64(objectName.c_str(), object_name.length(),
            QsFHKey::seed);
      fhk = fh.fh_hk;
      name = objectName;
    }


    /////////////////////////////////

}

class QsFileSystem
{

	struct BucketStats {
      size_t size;
      size_t size_rounded;
      real_time creation_time;
      uint64_t num_entries;
    };
        
    struct qingstor_file_system fs;
    QsFileHandle rootFH;

    static std::atomic<uint32_t> fs_inst_counter;


    Bucket m_qsBucket;

    releaseFileHandle(QsFileHandle *qsFileHandle);

    int readDir(qingstor_readdir_callback readCallback, void *cb_arg, uint64_t *offset,
           bool *eof, uint32_t flags);

    bool validObjectName(const char* name);

    uint32_t new_inst() {
      return ++fs_inst_counter;
    }

    uint32_t get_inst() { return rootFH.state.dev; }




}