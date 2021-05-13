#include <iostream>
#include <string>
#include <set>
#include <exception>
#include <stdlib.h>
#include <optional>

#include "Authenticator.hpp"
#include "ScanMetadata.hpp"
#include "FileScanner.hpp"
#include "ScanPoller.hpp"

int main(int argc, char* argv[])
{
	std::cout << "Starting to execute Atlant scanner" << std::endl;
	//TODO: implement cmd parser as:
	// https://github.com/F-Secure/atlant-api/blob/master/java/scanner/src/main/com/fsecure/atlant/examples/scanner/AtlantScannerRunner.java

	//temporary hard coded string(s), to be read from command line
	std::set<std::string> auth_scope{ "scan" };
	std::string auth_address{ "commandLine.getAuthorizationAddress" };
	Authenticator authenticator{
		std::make_unique<AuthTokenFetcher>(auth_address, std::make_unique<HTTPClientSessionImpl>(auth_address, std::make_unique<HTTPResponseImpl>())),
		std::string("commandLine.getClientID"),
		std::string("commandLine.getClientSecret"),
		auth_scope};

	ScanMetadata metadata{};

	try {
		FileScanner fileScanner{std::string("commandLine.getScanAddress"), authenticator};
		ScanPoller scanPoller{fileScanner};
		std::string file{"commandLine.getInputFile"};
		auto result = scanPoller.scan(metadata, file);
		//TODO: implement result printer as:
		//https://github.com/F-Secure/atlant-api/blob/9bfac23491f555fceea7485fe9bf4edb35485893/java/scanner/src/main/com/fsecure/atlant/examples/scanner/AtlantScannerRunner.java#L31
	}
	catch (std::exception e) {
		std::cerr << "Error: " << e.what() << std::endl;
		std::exit(EXIT_FAILURE);
	}
	
	return 0;
}