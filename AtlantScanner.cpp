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
	Authenticator authenticator{
		std::string("commandLine.getAuthorizationAddress"),
		std::string("commandLine.getClientID"),
		std::string("commandLine.getClientSecret"),
		auth_scope};

	ScanMetadata metadata{};

	try {
		auto uri = Poco::URI("commandLine.getScanAddress");
		FileScanner fileScanner{uri, authenticator};
		ScanPoller scanPoller{fileScanner};
		std::string file{"commandLine.getInputFile"};
		std::optional<ScanResult> result = scanPoller.scan(metadata, file);
		//TODO: continue here (finalize scanPoller.scan), AFTER tests
	}
	catch (std::exception e) {
		std::cerr << "Error: " << e.what() << std::endl;
		std::exit(EXIT_FAILURE);
	}
	
	return 0;
}