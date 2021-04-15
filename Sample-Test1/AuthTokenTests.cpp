#include "pch.h"
#include "AuthToken.hpp"

TEST(AuthToken, TestConstructor) {
	AuthToken auth_token{ "token", 1 };
}

TEST(AuthToken, TestgetToken) {
	std::string token{ "token" };
	AuthToken auth_token{ token, 1 };
	auto result_token = auth_token.getToken();
	EXPECT_EQ(token, result_token);
}

TEST(AuthToken, TestgetExpiration) {
	int expr = 1;
	AuthToken auth_token{ "token" , expr };
	auto result_expiration = auth_token.getExpiration();
	EXPECT_EQ(expr, result_expiration);
}