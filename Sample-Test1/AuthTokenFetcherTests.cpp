#include "pch.h"
#include "AuthTokenFetcher.hpp"
#include "MockHTTPCommunication.hpp"

#include <sstream>

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;

TEST(AuthTokenFetcher, TestConstructor) {
	AuthTokenFetcher auth_token_fetcher{ std::string("authorization_address"), std::make_unique<HTTPClientSessionImpl>(std::string("host"))};
}

TEST(AuthTokenFetcher, Testfetch) {
	//PRE: mock client session, sendRequest and receiveResponse
	auto mockHTTPClientSessionImpl = std::make_unique<MockHTTPClientSessionImpl>() ;
	std::ostringstream buf;
	EXPECT_CALL(*mockHTTPClientSessionImpl, sendRequest(_)).Times(1).WillOnce(ReturnRef(buf));
	std::string response_content{ "{ \"access_token\": \"Token1\", \"token_type\" : \"bearer\", \"expires_in\" : 3600 }" };
	std::istringstream is{ response_content };
	EXPECT_CALL(*mockHTTPClientSessionImpl, receiveResponse(_)).Times(1).WillOnce(ReturnRef(is));
	
	//ACTION: create AuthTokenFetcher and call fetch()
	AuthTokenFetcher auth_token_fetcher(std::string{ "auth_address" }, std::move(mockHTTPClientSessionImpl));
	auto result_token = auth_token_fetcher.fetch("client_ID", "secret", std::set<std::string>{ "scope1" });

	//ASSERT: Verify that received token is expected
	AuthToken expected_token{ "Token1", 3600 };
	EXPECT_EQ(result_token.getToken(), expected_token.getToken());
	EXPECT_EQ(result_token.getExpiration(), expected_token.getExpiration());
}