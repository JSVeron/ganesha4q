#pragma once

#include  "QsSdkConfig.h"
#include  <string>
#include  <vector>

class QS_SDK_API QsBuilder{
public:
	QsBuilder(){};

	virtual ~QsBuilder() {};

	std::shared_ptr<HttpRequest> BuildHttpRequest(QsBaseRequest* request);

    inline  bool IsPropHasBeenSet(std::string strPropName)
    {
        auto it = find( m_propsHasBeenSet.begin(), m_propsHasBeenSet.end(), strPropName );
        if( it != m_propsHasBeenSet.end() ) // finded
        {
            return true;
        }
        else
        {
            return false;
        }
    }

private:


};


std::shared_ptr<HttpRequest> BuildHttpRequest(QsBaseRequest* request)
{
	auto httpRequest = Aws::MakeShared<Standard::StandardHttpRequest>(uri, method);

	request->AddQueryStringParameters(httpRequest);
    //do headers first since the request likely will set content-length as it's own header.
    request->AddHeadersToRequest(httpRequest);
    request->AddContentBodyToRequest(httpRequest, GetBody());//ShouldComputeContentMd5
}
