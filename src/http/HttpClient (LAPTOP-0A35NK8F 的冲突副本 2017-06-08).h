#ifndef QS_HTTP_CLIENT_H
#define QS_HTTP_CLIENT_H


#include <QsSdkConfig.h>
#include "config/QsConfig.h"
#include "http/HttpRequest.h"
#include "http/HttpResponse.h"
#include <string>

#include <external/curl/curl.h>

#include <memory>
#include <atomic>
//#include <condition_variable>


namespace QingStor
{
    namespace Http
    {
		extern QS_SDK_API const char* DATE_HEADER;
		extern QS_SDK_API const char* AWS_DATE_HEADER;
		extern QS_SDK_API const char* AWS_SECURITY_TOKEN;
		extern QS_SDK_API const char* ACCEPT_HEADER;
		extern QS_SDK_API const char* ACCEPT_CHAR_SET_HEADER;
		extern QS_SDK_API const char* ACCEPT_ENCODING_HEADER;
		extern QS_SDK_API const char* AUTHORIZATION_HEADER;
		extern QS_SDK_API const char* AWS_AUTHORIZATION_HEADER;
		extern QS_SDK_API const char* COOKIE_HEADER;
		extern QS_SDK_API const char* CONTENT_LENGTH_HEADER;
		extern QS_SDK_API const char* CONTENT_TYPE_HEADER;
		extern QS_SDK_API const char* USER_AGENT_HEADER;
		extern QS_SDK_API const char* VIA_HEADER;
		extern QS_SDK_API const char* HOST_HEADER;
		extern QS_SDK_API const char* AMZ_TARGET_HEADER;
		extern QS_SDK_API const char* X_AMZ_EXPIRES_HEADER;
		extern QS_SDK_API const char* CONTENT_MD5_HEADER;


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

			void ReleaseCurlHandle(CURL* handle) const;

        private:
          	//mutable CurlHandleContainer m_curlHandleContainer;
    		    //bool m_isUsingProxy;
    		    //std::string m_proxyUserName;
    		    //std::string m_proxyPassword;
    		    //std::string m_proxyHost;
    		    //unsigned m_proxyPort;
    		    //bool m_verifySSL;
    		    //std::string m_caPath;
    		    //std::string m_caFile;
    		    //bool m_allowRedirects;

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

#endif // QS_HTTP_CLIENT_H
