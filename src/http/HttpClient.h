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

#include <QsSdkConfig.h>
#include "config/QsConfig.h"
#include <string>

#include <external/curl/curl.h>

#include <memory>
#include <atomic>
//#include <condition_variable>
#include <QsSdkConfig.h>


namespace QingStor
{
    namespace Http
    {

        class HttpRequest;
        class HttpResponse;

        /**
          * Abstract HttpClient. All it does is make HttpRequests and return their response.
          */
        class QS_SDK_API HttpClient
        {
        public:
            HttpClient(const QingStor::QsConfig& clientConfig);
            virtual ~HttpClient() {}

            /*
            * Takes an http request, makes it, and returns the newly allocated HttpResponse
            */
            std::shared_ptr<HttpResponse> MakeRequest(HttpRequest& request) const;

            static void InitGlobalState();
    		static void CleanupGlobalState();

        private:
			CURL* AcquireCurlHandle() const;
            /**
              * Returns a handle to the pool for reuse. It is imperative that this is called
              * after you are finished with the handle.
              */
			void ReleaseCurlHandle(CURL* handle) const;

        private:
          	//mutable CurlHandleContainer m_curlHandleContainer;
    		    bool m_isUsingProxy;
    		    std::string m_proxyUserName;
    		    std::string m_proxyPassword;
    		    std::string m_proxyHost;
    		    unsigned m_proxyPort;
    		    bool m_verifySSL;
    		    std::string m_caPath;
    		    std::string m_caFile;
    		    bool m_allowRedirects;

    		    static std::atomic<bool> isInit;

    		    //Callback to read the content from the content body of the request
    		    static size_t ReadBody(char* ptr, size_t size, size_t nmemb, void* userdata);
    		    //callback to write the content from the response to the response object
    		    static size_t WriteData(char* ptr, size_t size, size_t nmemb, void* userdata);
    		    //callback to write the headers from the response to the response
    		    static size_t WriteHeader(char* ptr, size_t size, size_t nmemb, void* userdata);

        };

    } // namespace Http
} // namespace QingStor
