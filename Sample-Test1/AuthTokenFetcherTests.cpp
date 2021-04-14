#include "pch.h"
#include "../AuthTokenFetcher.hpp"
#include "MockHTTPCommunication.hpp"
#include "../HTTPCommunicationImpl.hpp"

#include <sstream>

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;

TEST(AuthTokenFetcher, TestConstructor) {
	AuthTokenFetcher auth_token_fetcher{ std::string("authorization_address"), std::make_unique<HTTPClientSessionImpl>(std::string("host"))};
}


//TODO: finalize: implement mocks using HTTPCommunication.hpp, use it, add required assert(s) and enable test
TEST(AuthTokenFetcher, Testfetch) {
	//PRE: mock client session, sendRequest and receiveResponse
	auto mockHTTPClientSessionImpl = std::make_unique<MockHTTPClientSessionImpl>() ;
	std::ostringstream buf;
	//std::ostream os(std::cout.rdbuf());
	EXPECT_CALL(*mockHTTPClientSessionImpl, sendRequest(_)).Times(1).WillOnce(ReturnRef(buf));
	std::string response_content{ "\"access_token\": \"Token1\", \"token_type\" : \"bearer\", \"expires_in\" : 3600" };
	std::istringstream is{ response_content };
	//TODO: fails at AuthtokenFetcher.cpp::108 with memory access (?) issue, should be test implementation issue
	EXPECT_CALL(*mockHTTPClientSessionImpl, receiveResponse(_)).Times(1).WillOnce(ReturnRef(is));
	
	//ACTION: create AuthTokenFetcher and call fetch()
	std::set<std::string> scopes{ "scope1" };
	AuthTokenFetcher auth_token_fetcher(std::string{ "http://example.com" }, std::move(mockHTTPClientSessionImpl));
	auto result_token = auth_token_fetcher.fetch("client_ID", "secret", scopes);

	//ASSERT: Verify that received token is expected
	AuthToken expected_token{ "Token1", 3600 };
	EXPECT_EQ(result_token.getToken(), expected_token.getToken());
	EXPECT_EQ(result_token.getExpiration(), expected_token.getExpiration());
}