#include "HTTPCommunicationImpl.hpp"

void HTTPRequestImpl::setContentType(const std::string& media_type)
{
	getRequest().setContentType(media_type);
}

void HTTPRequestImpl::setContentLength(std::streamsize length)
{
	getRequest().setContentLength(length);
}

void HTTPRequestImpl::set(const std::string& name, const std::string& value)
{
	getRequest().set(name, value);
}

Poco::Net::HTTPRequest HTTPRequestImpl::getRequest()
{
	return request;
}

Poco::Net::HTTPResponse HTTPResponseImpl::getResponse()
{
	return response;
}

Poco::Net::HTTPResponse::HTTPStatus HTTPResponseImpl::getStatus()
{
	return response.getStatus();
}

bool HTTPResponseImpl::has(const std::string& name)
{
	return response.has(name);
}

const std::string& HTTPResponseImpl::get(const std::string& name)
{
	return response.get(name);
}

std::ostream& HTTPClientSessionImpl::sendRequest(IHTTPRequest& request)
{
	auto req = request.getRequest();
	return session.sendRequest(req);
}

std::istream& HTTPClientSessionImpl::receiveResponse(IHTTPResponse& response)
{
	auto res = response.getResponse();
	return session.receiveResponse(res);
}