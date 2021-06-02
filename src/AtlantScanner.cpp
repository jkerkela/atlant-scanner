#include <iostream>
#include <string>
#include <set>
#include <exception>
#include <stdlib.h>

#include "Authenticator.hpp"
#include "ScanMetadata.hpp"
#include "FileScanner.hpp"
#include "ScanPoller.hpp"
#include "ResultPrinter.hpp"

int main(int argc, char* argv[])
{
	if (argc != 5) {
		std::cerr << "Invalid command line parameters - "
			"Usage of application: AUTH-URL SCAN-URL CLIENT-ID CLIENT-SECRET INPUT-FILE" << std::endl;
		return 1;
	}
	std::string auth_address = argv[0];
	std::string scan_address = argv[1];
	std::string client_id = argv[2];
	std::string client_secret = argv[3];
	std::string input_file = argv[4];

	std::set<std::string> auth_scope{ "scan" };
	Authenticator authenticator{
		std::make_unique<AuthTokenFetcher>(auth_address, std::make_unique<HTTPClientSessionImpl>(auth_address)),
		client_id,
		client_secret,
		auth_scope};

	ScanMetadata metadata{};

	try {
		FileScanner fileScanner{ std::make_unique<HTTPClientSessionImpl>(scan_address), scan_address, authenticator};
		ScanPoller scanPoller{fileScanner};
		auto result = scanPoller.scan(metadata, input_file);

		ResultPrinter::print(result);
	}
	catch (std::exception e) {
		std::cerr << "Error: " << e.what() << std::endl;
		std::exit(EXIT_FAILURE);
	}
	
	return 0;
}