#include "HTTPCommunicationImpl.hpp"

void HTTPRequestImpl::setContentType(const std::string& media_type)
{
	getRequest().setContentType(media_type);
}

void HTTPRequestImpl::setContentLength(std::streamsize length)
{
	getRequest().setContentLength(length);
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