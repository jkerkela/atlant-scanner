#ifndef MOCKHTTPCOMMUNICATION_HPP
#define MOCKHTTPCOMMUNICATION_HPP
#include "pch.h"
#include "gmock/gmock.h"

#include "../HTTPCommunication.hpp"

#include <string>

class MockHTTPClientSessionImpl : public IHTTPClientSession
{
public:
	MockHTTPClientSessionImpl() {};
	MOCK_METHOD1(sendRequest, std::ostream& (IHTTPRequest& request)) {};
	MOCK_METHOD1(receiveResponse, std::istream& (IHTTPResponse& response)) {};

	virtual ~MockHTTPClientSessionImpl() {};
};

#endif // HTTPCOMMUNICATIONIMPL_HPP
