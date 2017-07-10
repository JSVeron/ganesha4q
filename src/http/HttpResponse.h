#ifndef QS_HTTP_RESPONSE_H
#define QS_HTTP_RESPONSE_H

#include "QsSdkConfig.h"
#include "http/HttpRequest.h"
#include "utils/QsStream.h"


namespace QingStor
{
    namespace Utils
    {
        namespace Stream
        {
            class ResponseStream;
        }
    }
    namespace Http
    {
        ///**
        // * Enum of Http response Codes. The integer values of the response codes coorespond to the values in the RFC.
        // */
        //enum class HttpResponseCode
        //{
        //    REQUEST_NOT_MADE = -1,
        //    CONTINUE = 100,
        //    SWITCHING_PROTOCOLS = 101,
        //    PROCESSING = 102,
        //    OK = 200,
        //    CREATED = 201,
        //    ACCEPTED = 202,
        //    NON_AUTHORITATIVE_INFORMATION = 203,
        //    NO_CONTENT = 204,
        //    RESET_CONTENT = 205,
        //    PARTIAL_CONTENT = 206,
        //    MULTI_STATUS = 207,
        //    ALREADY_REPORTED = 208,
        //    IM_USED = 226,
        //    MULTIPLE_CHOICES = 300,
        //    MOVED_PERMANENTLY = 301,
        //    FOUND = 302,
        //    SEE_OTHER = 303,
        //    NOT_MODIFIED = 304,
        //    USE_PROXY = 305,
        //    SWITCH_PROXY = 306,
        //    TEMPORARY_REDIRECT = 307,
        //    PERMANENT_REDIRECT = 308,
        //    BAD_REQUEST = 400,
        //    UNAUTHORIZED = 401,
        //    PAYMENT_REQUIRED = 402,
        //    FORBIDDEN = 403,
        //    NOT_FOUND = 404,
        //    METHOD_NOT_ALLOWED = 405,
        //    NOT_ACCEPTABLE = 406,
        //    PROXY_AUTHENTICATION_REQUIRED = 407,
        //    REQUEST_TIMEOUT = 408,
        //    CONFLICT = 409,
        //    GONE = 410,
        //    LENGTH_REQUIRED = 411,
        //    PRECONDITION_FAILED = 412,
        //    REQUEST_ENTITY_TOO_LARGE = 413,
        //    REQUEST_URI_TOO_LONG = 414,
        //    UNSUPPORTED_MEDIA_TYPE = 415,
        //    REQUESTED_RANGE_NOT_SATISFIABLE = 416,
        //    EXPECTATION_FAILED = 417,
        //    IM_A_TEAPOT = 418,
        //    AUTHENTICATION_TIMEOUT = 419,
        //    METHOD_FAILURE = 420,
        //    UNPROC_ENTITY = 422,
        //    LOCKED = 423,
        //    FAILED_DEPENDENCY = 424,
        //    UPGRADE_REQUIRED = 426,
        //    PRECONDITION_REQUIRED = 427,
        //    TOO_MANY_REQUESTS = 429,
        //    REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
        //    LOGIN_TIMEOUT = 440,
        //    NO_RESPONSE = 444,
        //    RETRY_WITH = 449,
        //    BLOCKED = 450,
        //    REDIRECT = 451,
        //    REQUEST_HEADER_TOO_LARGE = 494,
        //    CERT_ERROR = 495,
        //    NO_CERT = 496,
        //    HTTP_TO_HTTPS = 497,
        //    CLIENT_CLOSED_TO_REQUEST = 499,
        //    INTERNAL_SERVER_ERROR = 500,
        //    NOT_IMPLEMENTED = 501,
        //    BAD_GATEWAY = 502,
        //    SERVICE_UNAVAILABLE = 503,
        //    GATEWAY_TIMEOUT = 504,
        //    HTTP_VERSION_NOT_SUPPORTED = 505,
        //    VARIANT_ALSO_NEGOTIATES = 506,
        //    INSUFFICIENT_STORAGE = 506,
        //    LOOP_DETECTED = 508,
        //    BANDWIDTH_LIMIT_EXCEEDED = 509,
        //    NOT_EXTENDED = 510,
        //    NETWORK_AUTHENTICATION_REQUIRED = 511,
        //    NETWORK_READ_TIMEOUT = 598,
        //    NETWORK_CONNECT_TIMEOUT = 599
        //};

        /**
         * Abstract class for representing an Http Response.
         */
        class QS_SDK_API HttpResponse
        {
        public:
            /**
             * Initializes an http response with the originalRequest and the response code.
             */
            HttpResponse(const HttpRequest&  originatingRequest) :
                httpRequest(originatingRequest),
                responseCode(HttpResponseCode::REQUEST_NOT_MADE)//,
				//bodyStream(std::make_shared<std::iostream>)
            {
				bodyStream = std::make_shared<QsStream>();
			}

            virtual ~HttpResponse() = default;

            /**
             * Get the request that originated this response
             */
            inline const HttpRequest& GetOriginatingRequest() const { return httpRequest; }

            /**
             * Get the headers from this response
             */
          	HeaderValueCollection GetHeaders() const;
            /**
             * Returns true if the response contains a header by headerName
             */
            bool HasHeader(const char* headerName) const;
            /**
             * Returns the value for a header at headerName if it exists.
             */
            std::string GetHeader(const std::string&) const;
            /**
             * Gets response code for this http response.
             */
            inline HttpResponseCode GetResponseCode() const { return responseCode; }
            /**
             * Sets the response code for this http response.
             */
            inline void SetResponseCode(HttpResponseCode httpResponseCode) { responseCode = httpResponseCode; }
            /**
             * Gets the content-type of the response body
             */
            //const std::string& GetContentType() const { return GetHeader(Http::CONTENT_TYPE_HEADER); };
            /**
             * Gets the response body of the response.
             */
            std::shared_ptr<std::iostream> GetResponseBody() { return bodyStream; }// bodyStream.GetUnderlyingStream();
            /**
             * Gives full control of the memory of the ResponseBody over to the caller. At this point, it is the caller's
             * responsibility to clean up this object.
             */
            //inline Utils::Stream::ResponseStream&& SwapResponseStreamOwnership() { return std::move(bodyStream); }
            /**
             * Adds a header to the http response object.
             */
            void AddHeader(const std::string&, const std::string&);
            /**
             * Sets the content type header on the http response object.
             */
            void SetContentType(const std::string& contentType) { AddHeader("content-type", contentType); };

        private:
            HttpResponse(const HttpResponse&);
            HttpResponse& operator = (const HttpResponse&);

            const HttpRequest& httpRequest;
            HttpResponseCode responseCode;

            std::map<std::string, std::string> headerMap;
			//std::iostream bodyStream;
			std::shared_ptr<QsStream> bodyStream;
            //Utils::Stream::ResponseStream bodyStream;
        };


    } // namespace Http
} // namespace QingStor


#endif // QS_HTTP_RESPONSE_H