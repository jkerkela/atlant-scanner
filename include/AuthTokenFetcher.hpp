#ifndef AUTHTOKENFETCHER_HPP
#define AUTHTOKENFETCHER_HPP

#include "AuthToken.hpp"
#include "ScanResult.hpp"
#include "HTTPCommunicationImpl.hpp"
#include "HTTPCommunication.hpp"

#include <string>
#include <set>
#include <unordered_map>
#include <memory>

#include <Poco/URI.h>

using namespace Poco::Net;

class AuthTokenFetcher {

public:
	AuthTokenFetcher(const std::string &authorization_address, std::unique_ptr<IHTTPClientSession> client);
	AuthToken fetch(
		const std::string &client_ID,
		const std::string &client_secret,
		const std::set<std::string> &scopes);


private:
	Poco::URI token_endpoint;
	std::unique_ptr<IHTTPClientSession> client;

	HTTPRequestImpl buildTokenRequest(const std::string &client_ID, const std::string &client_secret, const std::set<std::string> &scopes);
	std::string encodeScopes(const std::set<std::string>& scopes);
	std::string encodeQueryParameters(const std::unordered_map<std::string, std::string> &params);
	ScanResult deserializeScanResponse(std::istream& response);
	AuthToken deserializeTokenResponse(std::istream& response);
	Detection buildDetection(const std::string& detection_item_json);
};

#endif // AUTHTOKENFETCHER_HPP
