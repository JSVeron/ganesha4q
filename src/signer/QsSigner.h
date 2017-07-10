#pragma once

#include "http/HttpRequest.h"
#include "common/QsErrors.h" 
#include <string>


namespace QingStor
{

class QS_SDK_API QsSigner
{
public:
	QsSigner(std::string strAccessKeyID, std::string strSecretAccessKey) :m_AccessKeyID(strAccessKeyID), m_strSecretAccessKey(strSecretAccessKey){};// : m_clockSkew() { m_clockSkew.store(std::chrono::milliseconds(0L)); }
	virtual ~QsSigner(){};// = default;

	std::string GetTimestamp() const;

	QsError SignRequest(std::shared_ptr<Http::HttpRequest> request) const;

	std::string BuildStringToSign(std::shared_ptr<Http::HttpRequest> request) const;
	
	std::string BuildCanonicalizedHeaders(Http::HeaderValueCollection headers) const;
    
	std::string BuildCanonicalizedResource(std::shared_ptr<Http::HttpRequest> request) const;
    //protected:            
    //virtual std::chrono::milliseconds GetClockSkewOffset() const { return m_clockSkew.load(); }

    //std::atomic<std::chrono::milliseconds> m_clockSkew;
private:
    std::string m_AccessKeyID;
    std::string m_strSecretAccessKey;
};

} // namespace QingStor
