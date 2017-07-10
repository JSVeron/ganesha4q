#pragma once

#include "common/QsErrors.h"
#include "QsSdkConfig.h"
#include "config/QsConfig.h"
#include "signer/QsSigner.h"
#include "http/HttpCommon.h"
#include "http/HttpClient.h"
#include "service/Types.h"

#include "request/QsRequestBuilder.h"
#include "request/QsResponseUnparker.h"

#include  <memory>

using namespace QingStor;
using namespace QingStor::Http;


class Operation{
public:
	Operation(QsConfig *pConfig, Properties properties, std::string strApiName, HttpMethod strReqMethod, std::string  strReqURI) :
    m_pConfig(pConfig), m_properties(properties), m_strApiName(strApiName), 
    m_strReqMethod(strReqMethod),m_strReqURI(strReqURI)
    {

    };

    virtual ~Operation () {};

    inline const QsConfig* GetConfig() const{ return m_pConfig; };
    inline const Properties GetProperties() const{ return m_properties; };
    inline const std::string GetApiName() const{ return m_strApiName; }
	inline const HttpMethod GetReqMethod() const { return m_strReqMethod; }
		//{ return HttpMethodMapper::GetNameForHttpMethod(m_strReqMethod); };
    inline const std::string GetReqURI() const{ return m_strReqURI; };

private:
    QsConfig *      m_pConfig;
    Properties      m_properties;
    std::string     m_strApiName;
	HttpMethod      m_strReqMethod;
    std::string     m_strReqURI;

    //StatusCodes []int
};

   //template <typename PAYLOAD_TYPE>
   // class AmazonWebServiceResult
   // {
   // public:
   //     AmazonWebServiceResult() {}

   //     /**
   //      * Sets payload, header collection and a response code.
   //      */
   //     AmazonWebServiceResult(const PAYLOAD_TYPE& payload, const Http::HeaderValueCollection& headers, Http::HttpResponseCode responseCode = Http::HttpResponseCode::OK) :
   //         m_payload(payload),
   //         m_responseHeaders(headers),
   //         m_responseCode(responseCode)
   //     {}

   //     /**
   //     * Sets payload, header collection and a response code, but transfers ownership of payload and headers (for move only operations).
   //     */
   //     AmazonWebServiceResult(PAYLOAD_TYPE&& payload, Http::HeaderValueCollection&& headers, Http::HttpResponseCode responseCode = Http::HttpResponseCode::OK) :
   //         m_payload(std::forward<PAYLOAD_TYPE>(payload)),
   //         m_responseHeaders(std::forward<Http::HeaderValueCollection>(headers)),
   //         m_responseCode(responseCode)
   //     {}

   //     AmazonWebServiceResult(const AmazonWebServiceResult& result) :
   //         m_payload(result.m_payload),
   //         m_responseHeaders(result.m_responseHeaders),
   //         m_responseCode(result.m_responseCode)
   //     {}

   //     AmazonWebServiceResult(AmazonWebServiceResult&& result) :
   //         m_payload(std::move(result.m_payload)),
   //         m_responseHeaders(std::move(result.m_responseHeaders)),
   //         m_responseCode(result.m_responseCode)
   //     {}

   //     /**
   //      * Get the payload from the response
   //      */
   //     inline const PAYLOAD_TYPE& GetPayload() const { return m_payload; }
   //     /**
   //      * Get the payload from the response and take ownership of it.
   //      */
   //     inline PAYLOAD_TYPE TakeOwnershipOfPayload() { return std::move(m_payload); }
   //     /**
   //     * Get the headers from the response
   //     */
   //     inline const Http::HeaderValueCollection& GetHeaderValueCollection() const { return m_responseHeaders; }
   //     /**
   //     * Get the http response code from the response
   //     */
   //     inline Http::HttpResponseCode GetResponseCode() const { return m_responseCode; }

   // private:
   //     PAYLOAD_TYPE m_payload;
   //     Http::HeaderValueCollection m_responseHeaders;
   //     Http::HttpResponseCode m_responseCode;
   // };



class QS_SDK_API QsRequest
{
public:
	/**
	 * configuration will be used for http client settings, retry strategy, throttles, and signing information.
	 * supplied signer will be used for all requests.
	 * errorMarshaller tells the client how to convert error payloads into AWSError objects.
	 */
QsRequest(const Operation& operation, QsRequestBuilder * pReqBuilder, QsResponseUnparker * pRespUnparker) 
  :m_operation(operation), m_reqErr(QsError::QS_ERR_NO_ERROR), 
  m_signer(operation.GetConfig()->m_AccessKeyId, operation.GetConfig()->m_SecretAccessKey),
  m_pReqBuilder(pReqBuilder), m_pRespUnpacker(pRespUnparker){};

	virtual ~QsRequest(){};

	QsError GetResponse();

protected:
  QsRequestBuilder * m_pReqBuilder;
  QsResponseUnparker * m_pRespUnpacker;

	Operation m_operation;
	std::shared_ptr<HttpClient> m_httpClient;
	QingStor::QsSigner m_signer;
	//std::shared_ptr<std::iostream> m_responseStream;

	QsError m_reqErr;

	void BuildHttpRequest(const std::shared_ptr<Http::HttpRequest> httpRequest);
	void AddHeadersToRequest(const std::shared_ptr<Http::HttpRequest> httpRequest);
	void AddContentBodyToRequest(const std::shared_ptr<Http::HttpRequest> httpRequest);
	void AddQueryStringParameters(const std::shared_ptr<HttpRequest> httpRequest);
	QsError SendReqAndWait4Resp(const std::shared_ptr<HttpRequest>& httpRequest, std::shared_ptr<HttpResponse>& httpResponse);
	void PareseResponse(const std::shared_ptr<HttpResponse> httpResponse);
	std::string  ParseUri(const std::shared_ptr<HttpRequest>& httpRequest) const;

	//std::shared_ptr<std::iostream> GetBody();


// ========================================================

//protected:
    /**
     * Calls AttemptOnRequest until it either, succeeds, runs out of retries from the retry strategy,
     * or encounters and error that is not retryable.
     */
    /*HttpResponseOutcome AttemptExhaustively(const std::string& uri,
        const Aws::AmazonWebServiceRequest& request,
        Http::HttpMethod httpMethod) const;
*/
    /**
     * Calls AttemptOnRequest until it either, succeeds, runs out of retries from the retry strategy,
     * or encounters and error that is not retryable. This method is for payloadless requests e.g. GET, DELETE, HEAD
     */
   /* HttpResponseOutcome AttemptExhaustively(const std::string& uri, Http::HttpMethod httpMethod) const;*/

    /**
     * Constructs and Http Request from the uri and AmazonWebServiceRequest object. Signs the request, sends it accross the wire
     * then reports the http response.
     */
    //HttpResponseOutcome AttemptOneRequest(const std::string& uri,
    //    const Aws::AmazonWebServiceRequest& request,
    //    Http::HttpMethod httpMethod) const;

    /**
    * Constructs and Http Request from the uri and AmazonWebServiceRequest object. Signs the request, sends it accross the wire
    * then reports the http response. This method is for payloadless requests e.g. GET, DELETE, HEAD
    */
    //HttpResponseOutcome AttemptOneRequest(const std::string& uri, Http::HttpMethod httpMethod) const;

    ///**
    // * This is used for structureless response payloads (file streams, binary data etc...). It calls AttemptExhaustively, but upon
    // * return transfers ownership of the underlying stream for the http response to the caller.
    // */
    //StreamOutcome MakeRequestWithUnparsedResponse(const std::string& uri,
    //    const Aws::AmazonWebServiceRequest& request,
    //    Http::HttpMethod method = Http::HttpMethod::HTTP_POST) const;

    ///**
    // * Abstract.  Subclassing clients should override this to tell the client how to marshall error payloads
    // */
    //virtual AWSError<CoreErrors> BuildAWSError(const std::shared_ptr<Qs::Http::HttpResponse>& response) const = 0;


    ///**
    // *  Gets the underlying ErrorMarshaller for subclasses to use.
    // */
    //const std::shared_ptr<AWSErrorMarshaller>& GetErrorMarshaller() const
    //{
    //    return m_errorMarshaller;
    //}

//private:
    //void AddContentBodyToRequest(const std::shared_ptr<HttpRequest>& httpRequest,
    //                             const std::shared_ptr<std::iostream>& body, bool needsContentMd5 = false) const;
    

    //void AddCommonHeaders(HttpRequest& httpRequest) const;
    //void InitializeGlobalStatics();
    //void CleanupGlobalStatics();


    //std::shared_ptr<HttpRequest> ConvertToRequestForPresigning(const Aws::AmazonWebServiceRequest& request, Qs::Http::URI& uri,
	//	HttpMethod method, const QueryStringParameterCollection& extraParams) const;

    //std::shared_ptr<QingStor::QsSigner> m_signer;
    //std::shared_ptr<AWSErrorMarshaller> m_errorMarshaller;
    //std::shared_ptr<RetryStrategy> m_retryStrategy;
    //std::shared_ptr<Aws::Utils::RateLimits::RateLimiterInterface> m_writeRateLimiter;
    //std::shared_ptr<Aws::Utils::RateLimits::RateLimiterInterface> m_readRateLimiter;
    //std::string m_userAgent;
    //std::shared_ptr<Aws::Utils::Crypto::Hash> m_hash;
    //static std::atomic<int> s_refCount;

};