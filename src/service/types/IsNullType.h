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

#include "../../common/QsBaseType.h"
#include <string>


// Headers of CustomizedType.
// IsNullType presents IsNull.
class QS_SDK_API IsNullType: QsBaseType
{
public:
	IsNullType(){};
	IsNullType(std::string serializedStr);

	// Refer url

	inline void SetReferer(bool  Referer)
	{
		m_propsHasBeenSet.push_back("Referer");
		m_Referer = Referer;
	};

	inline bool  GetReferer(){return m_Referer;};
	std::string Serialize();

private:		
	
	// Refer url
	bool  m_Referer;
	

};












































