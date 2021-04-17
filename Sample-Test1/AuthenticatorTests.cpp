#include "pch.h"
#include "Authenticator.hpp"

#include "MockHTTPCommunication.hpp"
#include "APIException.hpp"

#include <sstream>

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;

TEST(Authenticator, TestConstructor) {
	Authenticator authenticator{ std::make_unique<AuthTokenFetcher>("auth_address", std::make_unique<HTTPClientSessionImpl>("auth_address")),
		"client_ID",
		"client_secret",
		std::set<std::string>{ "scope1" }
	};
}

TEST(AuthTokenFetcher, fetchTokenValidResponseJSON) {
	//PRE: mock client session, sendRequest and receiveResponse
	auto mockHTTPClientSessionImpl = std::make_unique<MockHTTPClientSessionImpl>();
	std::ostringstream buf;
	EXPECT_CALL(*mockHTTPClientSessionImpl, sendRequest(_)).Times(1);
	std::string response_content{ "{ \"access_token\": \"Token1\", \"token_type\" : \"bearer\", \"expires_in\" : 3600 }" };
	std::istringstream is{ response_content };
	EXPECT_CALL(*mockHTTPClientSessionImpl, receiveResponse(_)).Times(1).WillOnce(ReturnRef(is));

	//ACTION: create Authenticator and call fetchToken()
	Authenticator authenticator{ std::make_unique<AuthTokenFetcher>("auth_address", std::move(mockHTTPClientSessionImpl)),
		"client_ID",
		"client_secret",
		std::set<std::string>{ "scope1" }
	};
	auto result_token = authenticator.fetchToken();

	//ASSERT: Verify that received token is expected
	AuthToken expected_token{ "Token1", 3600 };
	EXPECT_EQ(result_token.getToken(), expected_token.getToken());
	EXPECT_EQ(result_token.getExpiration(), expected_token.getExpiration());
}

TEST(AuthTokenFetcher, fetchTokenInvalidResponseJSON) {
	//PRE: mock client session, sendRequest and receiveResponse
	auto mockHTTPClientSessionImpl = std::make_unique<MockHTTPClientSessionImpl>();
	std::ostringstream buf;
	EXPECT_CALL(*mockHTTPClientSessionImpl, sendRequest(_)).Times(1);
	std::string invalid_response_content{ "" };
	std::istringstream is{ invalid_response_content };
	EXPECT_CALL(*mockHTTPClientSessionImpl, receiveResponse(_)).Times(1).WillOnce(ReturnRef(is));

	//ACTION: create Authenticator
	Authenticator authenticator{ std::make_unique<AuthTokenFetcher>("auth_address", std::move(mockHTTPClientSessionImpl)),
		"client_ID",
		"client_secret",
		std::set<std::string>{ "scope1" }
	};

	//ASSERT: Verify that fetchToken throws expected exception
	EXPECT_THROW(authenticator.fetchToken(), APIException);
}