#pragma once

#include "common/QsErrors.h"
#include "QsSdkConfig.h"
#include "config/QsConfig.h"
#include "signer/QsSigner.h"
#include "http/HttpCommon.h"
#include "http/HttpClient.h"
#include "service/Types.h"

#include  <memory>

using namespace QingStor;

class QS_SDK_API QsRequestBuilder
{
public:
	/**
	 * Defult and Base Builder Class
	 */
	QsRequestBuilder(){};

	virtual ~QsRequestBuilder(){};

	virtual bool CkeckIfInputIsVaild(){ return true; };
	virtual Http::HeaderValueCollection GetHeaderValueCollection(){
		Http::HeaderValueCollection emptyCollection;
		return emptyCollection;
	}

	virtual Http::QueryParamCollection  GetQueryParamCollection(){
		Http::QueryParamCollection emptyCollection;
		return emptyCollection;
	}

	virtual std::shared_ptr<std::iostream> GetRequestBody(){return nullptr;};

};


template<typename InputType>
class QS_SDK_API QsDefaultRequestBuilder :public QsRequestBuilder
{
public:
	/**
	* Defult and Base Unparker Class
	*/
	QsDefaultRequestBuilder(InputType * input) :QsRequestBuilder(), m_intput(input){};

	virtual ~QsDefaultRequestBuilder(){};

private:
	InputType * m_intput;

};
