#ifndef HTTPCOMMUNICATION_HPP
#define HTTPCOMMUNICATION_HPP

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPClientSession.h>

class IHTTPRequest
{
public:
	virtual void setContentType(const std::string& media_type) = 0;
	virtual void setContentLength(std::streamsize length) = 0;
	virtual void set(const std::string& name, const std::string& value) = 0;
	virtual Poco::Net::HTTPRequest getRequest() = 0;
	virtual ~IHTTPRequest() {};
};

class IHTTPResponse
{
public:
	virtual Poco::Net::HTTPResponse getResponse() = 0;
	virtual Poco::Net::HTTPResponse::HTTPStatus getStatus() = 0;
	virtual bool has(const std::string& name) = 0;
	virtual const std::string& get(const std::string& name) = 0;
	virtual ~IHTTPResponse() {};

};

class IHTTPClientSession
{
public:
	virtual std::ostream& sendRequest(IHTTPRequest& request) = 0;
	virtual std::istream& receiveResponse(IHTTPResponse& response) = 0;
	virtual std::string getURI() = 0;
	virtual ~IHTTPClientSession() {};

};

#endif // HTTPCOMMUNICATION_HPP
