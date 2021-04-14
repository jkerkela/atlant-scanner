#ifndef FILESCANNER_HPP
#define FILESCANNER_HPP

#include <string>
#include <fstream>
#include <optional>

#include <Poco/JSON/Parser.h>
#include <Poco/URI.h>

#include "Authenticator.hpp"
#include "AuthToken.hpp"
#include "ScanResult.hpp"
#include "ScanMetadata.hpp"
#include "HTTPCommunicationImpl.hpp"

using namespace Poco::Net;

class FileScanner {

public:
	FileScanner(const std::string &scanning_address, Authenticator &authenticator);
	void refreshToken();
	ScanResult scan(ScanMetadata &metadata, std::ifstream &input, std::unique_ptr<IHTTPClientSession> client);
	ScanResult poll(std::unique_ptr<IHTTPClientSession> client, const std::string &poll_URL);
	Poco::URI getScanAddress() { return scan_endpoint; };
	Poco::URI getAddress() { return base_URI; };

private:
	Poco::URI base_URI;
	Poco::URI scan_endpoint;
	Poco::URI poll_endpoint;
	Authenticator &authenticator;
	AuthToken auth_token;
	std::string HTTP_reques_body;

	HTTPRequestImpl buildScanRequest(ScanMetadata &metadata, std::ifstream &input);
	HTTPRequestImpl buildPollRequest(const std::string& poll_URL);
	std::string serializeScanMetadata(ScanMetadata &metadata);
	ScanResult processScanResponse(std::unique_ptr<IHTTPClientSession> client);
	ScanResult processPollResponse(std::unique_ptr<IHTTPClientSession> client);
	ScanResult deserializeScanResponse(std::istream &response);
	Detection buildDetection(Poco::JSON::Array::ConstIterator it);
};
#endif // FILESCANNER_HPP