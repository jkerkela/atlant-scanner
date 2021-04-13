#include "pch.h"
#include "../FileScanner.hpp"

#include <Poco/Net/NetException.h>
#include <string>
#include <set>

TEST(FileScanner, TestConstructorWithNonAccessibleAuthAddress) {
	std::set<std::string> scopes{ "scope1" };
	Authenticator authenticator{ "non_accessible_address", "client_ID", "client_secret", scopes };
	EXPECT_THROW(FileScanner("some_scannig_address", authenticator), NetException);
}

//TODO: continue here, mock httprequest to pass
TEST(FileScanner, TestConstructorWithAccessibleAuthAddress) {
	std::set<std::string> scopes{ "scope1" };
	Authenticator authenticator{ "http://accessible_domain.com", "client_ID", "client_secret", scopes };
	EXPECT_NO_THROW(new FileScanner("some_scanning_adress", authenticator));
}