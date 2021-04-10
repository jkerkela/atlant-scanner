
#include "FileScanner.hpp"

namespace {
	constexpr auto API_PREFIX = "/api/poll/v1";
}

FileScanner::FileScanner(const std::string& scanning_address, Authenticator& authenticator) 
	: base_URI{ scanning_address },
	scan_endpoint{ std::string(scanning_address + API_PREFIX) },
	poll_endpoint{ std::string(scanning_address + API_PREFIX) },
	authenticator{ authenticator }
{
	refreshToken();
	//TODO: implement filescanner as:
// https://github.com/F-Secure/atlant-api/blob/master/java/scanner/src/main/com/fsecure/atlant/examples/scanner/FileScanner.java
	
};

void FileScanner::refreshToken()
{
	auth_token = authenticator.fetchToken();
}
