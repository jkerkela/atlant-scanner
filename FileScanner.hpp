#ifndef FILESCANNER_HPP
#define FILESCANNER_HPP

#include <string>
#include <fstream>
#include <optional>

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/URI.h>
#include <Poco/JSON/Parser.h>

#include "Authenticator.hpp"
#include "AuthToken.hpp"
#include "ScanResult.hpp"
#include "ScanMetadata.hpp"

using namespace Poco::Net;

class FileScanner {

public:
	FileScanner(Poco::URI &scanningAddress, Authenticator &authenticator);
	void refreshToken();
	ScanResult scan(ScanMetadata &metadata, std::ifstream &input);
	ScanResult poll(const std::string& poll_URL);

private:
	Poco::URI base_URI;
	Poco::URI scan_endpoint;
	Poco::URI poll_endpoint;
	HTTPClientSession client;
	Authenticator &authenticator;
	AuthToken auth_token;
	std::string HTTP_reques_body;

	HTTPRequest buildScanRequest(ScanMetadata &metadata, std::ifstream &input);
	HTTPRequest buildPollRequest(const Poco::URI& poll_URL);
	std::string serializeScanMetadata(ScanMetadata &metadata);
	ScanResult processScanResponse(HTTPClientSession &client);
	ScanResult processPollResponse(HTTPClientSession& client);
	ScanResult deserializeScanResponse(std::istream &response);
	Detection buildDetection(Poco::JSON::Array::ConstIterator it);
};
#endif // FILESCANNER_HPP