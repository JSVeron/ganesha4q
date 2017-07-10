/********************************************************************
 * 2017 -
 * open source under Apache License Version 2.0
 ********************************************************************/
/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _QINGSTOR_LIBQINGSTOR_CONFIGURATION_H_
#define _QINGSTOR_LIBQINGSTOR_CONFIGURATION_H_

#include  "common/QsErrors.h"
#include <string>

namespace QingStor {
//namespace Internal {
class QsConfig {
public:
	QsConfig(std::string access_key_id, std::string secret_access_key);

	QsConfig();

	QsError loadConfigFile(std::string config_file);

public:
	std::string m_AccessKeyId;
	std::string m_SecretAccessKey;
	std::string m_Host;
	//std::string m_Location;
	int m_Port;
	std::string m_Protocol;
	int m_ConnectionRetries;
	std::string m_AdditionalUserAgent;
	//int m_NConnections;
	//int64_t m_ChunkSize;
	std::string m_LogLevel;
};
//}
}


#endif /* _QINGSTOR_LIBQINGSTOR_CONFIGURATION_H_ */