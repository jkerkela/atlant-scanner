#ifndef MOCKHTTPCOMMUNICATION_HPP
#define MOCKHTTPCOMMUNICATION_HPP
#include "pch.h"
#include "gmock/gmock.h"

#include "HTTPCommunication.hpp"

#include <string>

class MockHTTPClientSessionImpl : public IHTTPClientSession
{
public:
	MockHTTPClientSessionImpl() {};
	MOCK_METHOD0(getResponseStatus, Poco::Net::HTTPResponse::HTTPStatus()) {};
	MOCK_METHOD0(receiveResponse, std::istream&()) {};
	MOCK_METHOD1(sendRequest, void(IHTTPRequest& request)) {};
	MOCK_METHOD1(responseContains, bool(const std::string& value)) {};
	MOCK_METHOD1(getFromResponse, std::string(const std::string& value)) {};

	virtual ~MockHTTPClientSessionImpl() {};
};

class MockHTTPResponseImpl : public IHTTPResponse
{
	MockHTTPResponseImpl() {};
	MOCK_METHOD0(getResponse, Poco::Net::HTTPResponse()) {};
	MOCK_METHOD0(getStatus, Poco::Net::HTTPResponse::HTTPStatus()) {};
	MOCK_METHOD1(has, bool(const std::string& name)) {};
	MOCK_METHOD1(get, std::string&(const std::string& name)) {};

	virtual ~MockHTTPResponseImpl();
};

#endif // HTTPCOMMUNICATIONIMPL_HPP
