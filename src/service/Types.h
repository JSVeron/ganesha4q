// +-------------------------------------------------------------------------
// | Copyright (C) 2016 Yunify, Inc.
// +-------------------------------------------------------------------------
// | Licensed under the Apache License, Version 2.0 (the "License");
// | you may not use this work except in compliance with the License.
// | You may obtain a copy of the License in the LICENSE file, or at:
// |
// | http://www.apache.org/licenses/LICENSE-2.0
// |
// | Unless required by applicable law or agreed to in writing, software
// | distributed under the License is distributed on an "AS IS" BASIS,
// | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// | See the License for the specific language governing permissions and
// | limitations under the License.
// +-------------------------------------------------------------------------

#pragma once

#include  "QsSdkConfig.h"
#include  "common/QsErrors.h"
#include  <string>
#include  <vector>
#include  <memory>
#include  "http/HttpCommon.h"
#include  "types/ACLType.h"
#include  "types/BucketType.h"
#include  "types/ConditionType.h"
#include  "types/CORSRuleType.h"
#include  "types/GranteeType.h"
#include  "types/IPAddressType.h"
#include  "types/IsNullType.h"
#include  "types/KeyType.h"
#include  "types/KeyDeleteErrorType.h"
#include  "types/NotIPAddressType.h"
#include  "types/ObjectPartType.h"
#include  "types/OwnerType.h"
#include  "types/StatementType.h"
#include  "types/StringLikeType.h"
#include  "types/StringNotLikeType.h"
#include  "types/UploadsType.h"


//typedef std::shared_prt<std::iostream> QsStreamPtr;

namespace QingStor
{

	typedef QsBaseType QsInput;

	class QS_SDK_API QsOutput :public QsBaseType
	{
	public:
		QsOutput() :m_responseCode(Http::HttpResponseCode::REQUEST_NOT_MADE){};

		QsOutput(QsError err, Http::HttpResponseCode responseCode) : m_responseCode(responseCode){};

		virtual ~QsOutput(){};

		virtual bool IsVaild(){ return true; };

		inline std::string GetRequestID(){ return m_requestID; };

		inline Http::HttpResponseCode GetResponseCode(){ return m_responseCode; };

		inline void SetRequestID(std::string requestID){ m_requestID =  requestID; };

		inline void SetResponseCode(Http::HttpResponseCode responseCode){ m_responseCode = responseCode; };

	protected:
		std::string m_requestID;
		Http::HttpResponseCode m_responseCode;
	};

	// Properties presents the service properties.
	struct Properties{
// Bucket name
			std::string  BucketName; // Required
		// Object key
			std::string  ObjectKey; // Required
		// QingCloud Zone ID
			std::string  Zone; 
		
	};

} // namespace











































