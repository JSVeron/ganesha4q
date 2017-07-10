#ifndef QS_HTTP_REQUEST_H
#define QS_HTTP_REQUEST_H


#include <QsSdkConfig.h>

#include <http/HttpURI.h>
#include <http/HttpCommon.h>

//#include <aws/core/utils/stream/ResponseStream.h>
#include <memory>
#include <sstream>

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
          * Abstract class for representing an HttpRequest.
          */
        class QS_SDK_API HttpRequest
        {
        public:
            /**
             * Initializes an HttpRequest object with uri and http method.
             */
			HttpRequest(HttpMethod method, std::string uri);

            virtual ~HttpRequest() {}

            /**
             * Get All headers for this request.
             */
            HeaderValueCollection GetHeaders() const;
            /**
             * Get the value for a Header based on its name.
             */
            const std::string GetHeaderValue(const char* headerName) const;
            /**
             * Add a header pair
             */
            void SetHeaderValue(const char* headerName, const  std::string& headerValue);
            /**
             * Creates a shared_ptr of HttpRequest with uri, method, and closure for how to create a response stream.
             */
            void SetHeaderValue(const std::string& headerName, const  std::string& headerValue);
            /**
             * Deletes a header from the request by name.
             */
            void DeleteHeader(const char* headerName);
            /**
             * Adds a content body stream to the request. This stream will be used to send the body to the endpoint.
             */
            void AddContentBody( std::shared_ptr<std::iostream> strContent) { bodyStream = strContent; };
            /**
             * Gets the content body stream that will be used for this request.
             */
            const std::shared_ptr<std::iostream>& GetContentBody() const { return bodyStream; };
            /**
             * Returns true if a header exists in the request with name
             */
            bool HasHeader(const char* name) const;
            /**
             * Get size in bytes of the request when as it will be going accross the wire.
             */
            int64_t GetSize() const;
            /**
             * Gets the factory for creating the stream that will be used in the http response.
             */
            //const std::iostreamFactory& GetResponseStreamFactory() const;
            ///**
            //* Sets the factory for creating the stream that will be used in the http response.
            //*/
            //void SetResponseStreamFactory(const std::iostreamFactory& factory);
            /**
             * Gets underlying URI object with mutation access.
             */
            inline URI& GetUri()
            {
                return m_uri;
            }
            /**
             * Gets the underlying URI object.
             */
            const URI& GetUri() const { return m_uri; }
            /**
             * Converts the URI into a string and returns it. If includeQueryString is set to true, the query string
             * will be included in the returned value. 
             */
            inline std::string GetURIString(bool includeQueryString = true) const
            {
                return m_uri.GetURIString(includeQueryString);
            }
            /**
             * Get the http method for this request.
             */
            inline HttpMethod GetMethod() const
            {
                return m_method;
            }
            /**
             * Gets the query string from the URI on this request.
             */
            inline const std::string& GetQueryString() const
            {
                return m_uri.GetQueryString();
            }
            /**
             * Normalizes the URI for use with signing.
             */
            inline void CanonicalizeRequest()
            {
                m_uri.CanonicalizeQueryString();
            }
            /**
             * Gets the query string for the underlying URI as a key value mapping
             */
            inline QueryStringParameterCollection GetQueryStringParameters() const
            {
                return m_uri.GetQueryStringParameters();
            }
            /**
             * Adds a query string parameter to the underlying URI by key and value.
             */
            inline void AddQueryStringParameter(const char* key, const std::string& value)
            {
                m_uri.AddQueryStringParameter(key, value);
            }

            ///**
            // * Gets date header.
            // */
            //inline const std::string& GetDate() const
            //{
            //    return GetHeaderValue(DATE_HEADER);
            //}
            ///**
            //* Gets date header.
            //*/
            //inline void SetDate(const std::string& value)
            //{
            //    SetHeaderValue(DATE_HEADER, value);
            //}
            ///**
            // * Gets accept header.
            // */
            //inline const std::string& GetAccept() const
            //{
            //    return GetHeaderValue(ACCEPT_HEADER);
            //}
            ///**
            // * Gets accept header.
            // */
            //inline void SetAccept(const std::string& value)
            //{
            //    SetHeaderValue(ACCEPT_HEADER, value);
            //}
            ///**
            // * Gets Accept CharSet header.
            // */
            //inline const std::string& GetAcceptCharSet() const
            //{
            //    return GetHeaderValue(ACCEPT_CHAR_SET_HEADER);
            //}
            ///**
            // * Sets Accept CharSet header.
            // */
            //inline void SetAcceptCharSet(const std::string& value)
            //{
            //    SetHeaderValue(ACCEPT_CHAR_SET_HEADER, value);
            //}
            ///**
            // * Gets accept encoding header.
            // */
            //inline const std::string GetAcceptEncoding() const
            //{
            //    return GetHeaderValue(ACCEPT_ENCODING_HEADER);
            //}
            ///**
            // * Sets accept encoding header.
            // */
            //inline void SetAcceptEncoding(const std::string& value)
            //{
            //    SetHeaderValue(ACCEPT_ENCODING_HEADER, value);
            //}
            ///**
            // * Gets authorization encoding header.
            // */
            //inline const std::string GetAuthorization() const
            //{
            //    return GetHeaderValue(AUTHORIZATION_HEADER);
            //}
            ///**
            // * Set authorization header.
            // */
            //inline void SetAuthorization(const std::string& value)
            //{
            //    SetHeaderValue(AUTHORIZATION_HEADER, value);
            //}
            ///**
            // * Gets aws authorization header.
            // */
            //inline const std::string GetAwsAuthorization() const
            //{
            //    return GetHeaderValue(AWS_AUTHORIZATION_HEADER);
            //}
            ///**
            //* Sets aws authorization header.
            //*/
            //inline void SetAwsAuthorization(const std::string& value)
            //{
            //    SetHeaderValue(AWS_AUTHORIZATION_HEADER, value);
            //}
            ///**
            //* Gets session token header.
            //*/
            //inline const std::string GetAwsSessionToken() const
            //{
            //    return GetHeaderValue(AWS_SECURITY_TOKEN);
            //}
            ///**
            //* Sets session token header.
            //*/
            //inline void SetAwsSessionToken(const std::string& value)
            //{
            //    SetHeaderValue(AWS_SECURITY_TOKEN, value);
            //}
            ///**
            //* Gets cookie header.
            //*/
            //inline const std::string GetCookie() const
            //{
            //    return GetHeaderValue(COOKIE_HEADER);
            //}
            ///**
            //* Sets cookie header.
            //*/
            //inline void SetCookie(const std::string& value)
            //{
            //    SetHeaderValue(COOKIE_HEADER, value);
            //}
            /**
            * Gets content-length header.
            */
            inline const std::string GetContentLength() const
            {
                return GetHeaderValue(CONTENT_LENGTH_HEADER);
            }
            /**
            * Sets content-length header.
            */
            inline void SetContentLength(const std::string& value)
            {
                SetHeaderValue(CONTENT_LENGTH_HEADER, value);
            }
            ///**
            //* Gets content-type header.
            //*/
            //inline const std::string& GetContentType() const
            //{
            //    return GetHeaderValue(CONTENT_TYPE_HEADER);
            //}
            ///**
            //* sets content-type header.
            //*/
            //inline void SetContentType(const std::string& value)
            //{
            //    SetHeaderValue(CONTENT_TYPE_HEADER, value);
            //}
            ///**
            //* Gets User Agent header.
            //*/
            //inline const std::string GetUserAgent() const
            //{
            //    return GetHeaderValue(USER_AGENT_HEADER);
            //}
            ///**
            //* Sets User Agent header.
            //*/
            //inline void SetUserAgent(const std::string& value)
            //{
            //    SetHeaderValue(USER_AGENT_HEADER, value);
            //}
            ///**
            //* Gets via header header.
            //*/
            //inline const std::string GetVia() const
            //{
            //    return GetHeaderValue(VIA_HEADER);
            //}
            ///**
            // * Sets via header.
            // */
            //inline void SetVia(const std::string& value)
            //{
            //    SetHeaderValue(VIA_HEADER, value);
            //}
			
            ///**
            // * Sets the closure for receiving events when data is received from the server.
            // */
            //inline void SetDataReceivedEventHandler(const DataReceivedEventHandler& dataReceivedEventHandler) { m_onDataReceived = dataReceivedEventHandler; }
            ///**
            // * Sets the closure for receiving events when data is received from the server.
            // */
            //inline void SetDataReceivedEventHandler(DataReceivedEventHandler&& dataReceivedEventHandler) { m_onDataReceived = std::move(dataReceivedEventHandler); }
            ///**
            // * Sets the closure for receiving events when data is sent to the server.
            // */
            //inline void SetDataSentEventHandler(const DataSentEventHandler& dataSentEventHandler) { m_onDataSent = dataSentEventHandler; }
            ///**
            // * Sets the closure for receiving events when data is sent to the server.
            // */
            //inline void SetDataSentEventHandler(DataSentEventHandler&& dataSentEventHandler) { m_onDataSent = std::move(dataSentEventHandler); }
            ///**
            // * Sets the closure for handling whether or not to cancel a request.
            // */
            //inline void SetContinueRequestHandle(const ContinueRequestHandler& continueRequestHandler) { m_continueRequest = continueRequestHandler; }
            ///**
            // * Sets the closure for handling whether or not to cancel a request.
            // */
            //inline void SetContinueRequestHandle(ContinueRequestHandler&& continueRequestHandler) { m_continueRequest = std::move(continueRequestHandler); }

            ///**
            // * Gets the closure for receiving events when data is received from the server.
            // */
            //inline const DataReceivedEventHandler& GetDataReceivedEventHandler() const { return m_onDataReceived; }
            ///**
            // * Gets the closure for receiving events when data is sent to the server.
            // */
            //inline const DataSentEventHandler& GetDataSentEventHandler() const { return m_onDataSent; }

            //inline const ContinueRequestHandler& GetContinueRequestHandler() const { return m_continueRequest; }

        private:
            URI m_uri;
			//std::string m_uri;
            HttpMethod m_method;
            //DataReceivedEventHandler m_onDataReceived;
            //DataSentEventHandler m_onDataSent;
            //ContinueRequestHandler m_continueRequest;
            HeaderValueCollection headerMap;
            std::shared_ptr<std::iostream> bodyStream;
        };

    } // namespace Http
} // namespace Aws

#endif // QS_HTTP_REQUEST_H

