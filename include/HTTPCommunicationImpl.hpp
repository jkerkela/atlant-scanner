#ifndef HTTPCOMMUNICATIONIMPL_HPP
#define HTTPCOMMUNICATIONIMPL_HPP

#include "HTTPCommunication.hpp"

#include <ostream>

class HTTPRequestImpl : public IHTTPRequest
{
public:
	HTTPRequestImpl(const std::string& method, const std::string& uri) : request{ Poco::Net::HTTPRequest{ method, uri } } {};
	void setContentType(const std::string& media_type) override;
	void setContentLength(const std::streamsize& length) override;
	void set(const std::string& name, const std::string& value) override;
	void setBody(const std::string& request_body) override;
	std::string getBody() override;
	Poco::Net::HTTPRequest getRequest() override;
	virtual ~HTTPRequestImpl() {};

private:
	Poco::Net::HTTPRequest request;
	std::string request_body_;
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
	HTTPClientSessionImpl(const std::string& host, 
		std::unique_ptr<IHTTPResponse> http_response_impl = std::make_unique<HTTPResponseImpl>()
	) :
		session{ Poco::Net::HTTPClientSession{ host } },
		http_response_ { std::move(http_response_impl)}
		{};
	void sendRequest(IHTTPRequest& request) override;
	Poco::Net::HTTPResponse::HTTPStatus getResponseStatus() override;
	std::istream& receiveResponse() override;
	bool responseContains(const std::string& value) override;
	std::string getFromResponse(const std::string& value) override;
	virtual ~HTTPClientSessionImpl() {};

protected:
	Poco::Net::HTTPClientSession session;
	std::unique_ptr<IHTTPResponse> http_response_;
};

#endif // HTTPCOMMUNICATIONIMPL_HPP
