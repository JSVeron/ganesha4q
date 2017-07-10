

class QS_SDK_API QsResponseUnparker
{
public:
	/**
	 * Defult and Base Unparker Class
	 */
	QsResponseUnparker(){};

	virtual ~QsResponseUnparker(){};

	//std::string GetRequestID(){ return m_pOutput->GetRequestID(); };

	//Http::HttpResponseCode GetResponseCode(){ return m_pOutput->GetResponseCode(); };
	virtual void ParseResponseCode(Http::HttpResponseCode responseCode) = 0;

	virtual void ParseRequestID(std::string requestID) = 0;

	virtual bool CkeckIfOutputIsVaild(){ return true; };

	virtual void ParseResponseHeaders(const Http::HeaderValueCollection & headerValueCollection){ return; };//return QsError::QS_ERR_NO_ERROR; 
	
	virtual void ParseResponseBody(const std::shared_ptr<std::iostream> & responseBody){ return; };//return QsError::QS_ERR_NO_ERROR; 

	//virtual void ParseResponseElements(const std::shared_ptr<HttpResponse> httpResponse){ return; };//return QsError::QS_ERR_NO_ERROR; 
};


template<typename OutputType>
class QS_SDK_API QsDefaultResponseUnparker :public QsResponseUnparker
{
public:
	/**
	 * Defult and Base Unparker Class
	 */
	QsDefaultResponseUnparker(OutputType* output) :QsResponseUnparker(), m_output(output){};

	virtual ~QsDefaultResponseUnparker(){};

	//std::string GetRequestID(){ return mm_output.GetRequestID(); };

	//Http::HttpResponseCode GetResponseCode(){ return m_output.GetResponseCode(); };

	virtual void ParseRequestID(std::string requestID) override { m_output->SetRequestID(requestID); };

	virtual void ParseResponseCode(Http::HttpResponseCode responseCode) override { m_output->SetResponseCode(responseCode); };

private:
	OutputType* m_output;
	//std::string m_requestID;
	//std::shared_ptr<std::iostream> m_responseStream;
	//Http::HeaderValueCollection m_responseHeaders;
	//Http::QueryParamCollection m_responseQueryParam;
	//Http::HttpResponseCode m_responseCode;
};
