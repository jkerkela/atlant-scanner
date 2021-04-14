#include "pch.h"
#include "../FileScanner.hpp"

#include <Poco/Net/NetException.h>

#include <string>
#include <set>
#include <memory>

TEST(FileScanner, TestConstructorWithNonAccessibleAuthAddress) {
	Authenticator authenticator{ std::make_unique<AuthTokenFetcher>("auth_address", std::make_unique<HTTPClientSessionImpl>("auth_address")), 
		"client_ID", 
		"client_secret", 
		std::set<std::string>{ "scope1" }
	};
	EXPECT_THROW(FileScanner("some_scannig_address", authenticator), NetException);
}
