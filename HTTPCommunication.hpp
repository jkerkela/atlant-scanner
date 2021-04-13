#ifndef HTTPCOMMUNICATION_HPP
#define HTTPCOMMUNICATION_HPP

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPClientSession.h>


class IHTTPRequest
{
public:
	IHTTPRequest(const std::string& method, const std::string& uri) : request{ Poco::Net::HTTPRequest{ method, uri } } {};
	Poco::Net::HTTPRequest getRequest() { return request; };

	virtual void setContentType(const std::string& media_type) = 0;
	virtual void setContentLength(std::streamsize length) = 0;

private:
	Poco::Net::HTTPRequest request;
};

class IHTTPResponse
{
public:
	IHTTPResponse() : response{ Poco::Net::HTTPResponse{} } {};
	Poco::Net::HTTPResponse getResponse() { return response; };

private:
	Poco::Net::HTTPResponse response;
};

class IHTTPClientSession
{
public:
	IHTTPClientSession(const std::string& host) : session{ Poco::Net::HTTPClientSession{ host } } {};
	virtual std::ostream& sendRequest(IHTTPRequest& request) = 0;
	virtual std::istream& receiveResponse(IHTTPResponse& response) = 0;
	//TODO: Fix getter
	//Poco::Net::HTTPClientSession getSession() { return session; };

protected:
	Poco::Net::HTTPClientSession session;
};

#endif // HTTPCOMMUNICATION_HPP
