#include <iostream>
#include <string>
#include <set>

#include "Authenticator.hpp"
#include "ScanMetadata.hpp"

int main(int argc, char* argv[])
{
	std::cout << "Starting to execute Atlant scanner" << std::endl;
	//TODO: implement cmd parser as:
	// https://github.com/F-Secure/atlant-api/blob/master/java/scanner/src/main/com/fsecure/atlant/examples/scanner/AtlantScannerRunner.java

	//temporary hard coded string, to be read from command line
	std::set<std::string> auth_scope{ "scan" };
	Authenticator authenticator{
		std::string("commandLine.getAuthorizationAddress"),
		std::string("commandLine.getClientID"),
		std::string("commandLine.getClientSecret"),
		auth_scope};

	ScanMetadata metadata{};
	//TODO: continue here
	
	return 0;
}