#ifndef FILESCANNER_HPP
#define FILESCANNER_HPP

#include <string>
#include <fstream>
#include <optional>

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/URI.h>

#include "Authenticator.hpp"
#include "AuthToken.hpp"
#include "ScanResult.hpp"
#include "ScanMetadata.hpp"

using namespace Poco::Net;

class FileScanner {

public:
	FileScanner(const Poco::URI &scanningAddress, Authenticator &authenticator);
	void refreshToken();
	std::optional<ScanResult> scan(ScanMetadata &metadata, std::ifstream &input);

private:
	Poco::URI base_URI;
	Poco::URI scan_endpoint;
	Poco::URI poll_endpoint;
	HTTPClientSession client;
	Authenticator &authenticator;
	AuthToken auth_token;
	std::string HTTP_reques_body;

	HTTPRequest buildScanRequest(ScanMetadata &metadata, std::ifstream &input);
	std::string serializeScanMetadata(ScanMetadata &metadata);
	std::optional<ScanResult> processScanResponse(HTTPClientSession& client);
	std::optional<ScanResult> deserializeScanResponse(std::istream &response);
};
#endif // FILESCANNER_HPP