#ifndef HTTPCOMMUNICATIONIMPL_HPP
#define HTTPCOMMUNICATIONIMPL_HPP

#include "HTTPCommunication.hpp"

#include <ostream>

class HTTPRequestImpl : public IHTTPRequest
{
public:
	HTTPRequestImpl(const std::string& method, const std::string& uri) : request{ Poco::Net::HTTPRequest{ method, uri } } {};
	void setContentType(const std::string& media_type) override;
	void setContentLength(std::streamsize length) override;
	void set(const std::string& name, const std::string& value) override;
	Poco::Net::HTTPRequest getRequest() override;
	virtual ~HTTPRequestImpl() {};

private:
	Poco::Net::HTTPRequest request;
};

class HTTPResponseImpl : public IHTTPResponse
{
public:
	HTTPResponseImpl() : response{ Poco::Net::HTTPResponse{} } {};
	Poco::Net::HTTPResponse getResponse() override;
	Poco::Net::HTTPResponse::HTTPStatus getStatus() override;
	bool has(const std::string& name) override;
	const std::string& get(const std::string& name) override;
	virtual ~HTTPResponseImpl() {};

private:
	Poco::Net::HTTPResponse response;
};

class HTTPClientSessionImpl : public IHTTPClientSession
{
public:
	HTTPClientSessionImpl(const std::string& host) : session{ Poco::Net::HTTPClientSession{ host } } {};
	std::ostream& sendRequest(IHTTPRequest& request) override;
	std::istream& receiveResponse(IHTTPResponse& response) override;
	std::string getURI() override;
	virtual ~HTTPClientSessionImpl() {};

protected:
	Poco::Net::HTTPClientSession session;
};

#endif // HTTPCOMMUNICATIONIMPL_HPP
