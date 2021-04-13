#ifndef HTTPCOMMUNICATIONIMPL_HPP
#define HTTPCOMMUNICATIONIMPL_HPP

#include "HTTPCommunication.hpp"
#include <ostream>

class HTTPRequestImpl : public IHTTPRequest
{
public:
	HTTPRequestImpl(const std::string& method, const std::string& uri) : IHTTPRequest{ method, uri } {};
	void setContentType(const std::string& media_type) override;
	void setContentLength(std::streamsize length) override;

};

class HTTPResponseImpl : public IHTTPResponse
{
public:
	HTTPResponseImpl() : IHTTPResponse{} {};

};

class HTTPClientSessionImpl : public IHTTPClientSession
{
public:
	HTTPClientSessionImpl(const std::string& host) : IHTTPClientSession{ host } {};
	std::ostream& sendRequest(IHTTPRequest& request) override;
	std::istream& receiveResponse(IHTTPResponse& response) override;
};

#endif // HTTPCOMMUNICATIONIMPL_HPP
