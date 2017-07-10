/*
  * Copyright 2010-2016 Amazon.com, Inc. or its affiliates. All Rights Reserved.
  *
  * Licensed under the Apache License, Version 2.0 (the "License").
  * You may not use this file except in compliance with the License.
  * A copy of the License is located at
  *
  *  http://aws.amazon.com/apache2.0
  *
  * or in the "license" file accompanying this file. This file is distributed
  * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
  * express or implied. See the License for the specific language governing
  * permissions and limitations under the License.
  */

#pragma once

//#include  "QsSdkConfig.h"

//#include <openssl/hmac.h>
//#include <openssl/bio.h>
//#include <openssl/evp.h>
//#include <openssl/buffer.h>
//#include <openssl/sha.h>
//
#include <stdlib.h>
namespace QingStor
{
    namespace Utils
    {
        namespace Encoder
        {
           /* Encodes a binary safe base 64 string */	
			char *	Base64Encode(const char *buffer, size_t length);
			
			void 	sha256hmac(const char *str, char out[33], const char *secret);

        } // namespace Json
    } // namespace Utils
} // namespace QingStor

