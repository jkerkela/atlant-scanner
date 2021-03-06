#ifndef FILESCANNER_HPP
#define FILESCANNER_HPP

#include "Authenticator.hpp"
#include "AuthToken.hpp"
#include "HTTPCommunicationImpl.hpp"
#include "ScanResult.hpp"
#include "ScanMetadata.hpp"

#include <string>
#include <fstream>
#include <optional>

#include <Poco/URI.h>

namespace filescanner {
	constexpr auto API_SCAN_POSTFIX = "/api/scan/v1";
	constexpr auto API_POLL_POSTFIX = "/api/poll/v1";
}

class FileScanner {

public:
	FileScanner(std::unique_ptr<IHTTPClientSession> http_client_session_impl,
		const std::string &scanning_address, Authenticator &authenticator);
	void refreshToken();
	ScanResult scan(ScanMetadata &metadata, std::ifstream &input);
	ScanResult poll(const std::string &poll_URL);
	Poco::URI getScanAddress() { return scan_endpoint; };
	Poco::URI getAddress() { return base_URI; };

private:
	std::unique_ptr<IHTTPClientSession> client;
	Poco::URI base_URI;
	Poco::URI scan_endpoint;
	Poco::URI poll_endpoint;
	Authenticator &authenticator;
	AuthToken auth_token;

	HTTPRequestImpl buildScanRequest(ScanMetadata &metadata, std::ifstream &input);
	HTTPRequestImpl buildPollRequest(const std::string& poll_URL);
	std::string serializeScanMetadata(ScanMetadata &metadata);
	ScanResult processScanResponse();
	ScanResult processPollResponse();
	ScanResult deserializeScanResponse(std::istream &response);
	Detection buildDetection(const std::string& detection_item_json);
};
#endif // FILESCANNER_HPP