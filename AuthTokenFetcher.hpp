#ifndef AUTHTOKENFETCHER_HPP
#define AUTHTOKENFETCHER_HPP

#include <string>
#include <set>
#include <unordered_map>
#include <memory>

#include "HTTPCommunicationImpl.hpp"
#include "HTTPCommunication.hpp"
#include <Poco/URI.h>
#include <Poco/JSON/Parser.h>

#include "AuthToken.hpp"
#include "ScanResult.hpp"

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
	std::string HTTP_request_body;

	HTTPRequestImpl buildTokenRequest(const std::string &client_ID, const std::string &client_secret, const std::set<std::string> &scopes);
	std::string encodeScopes(const std::set<std::string>& scopes);
	std::string encodeQueryParameters(const std::unordered_map<std::string, std::string> &params);
	ScanResult deserializeScanResponse(std::istream& response);
	AuthToken deserializeTokenResponse(std::istream& response);
	Detection buildDetection(Poco::JSON::Array::ConstIterator it);
};

#endif // AUTHTOKENFETCHER_HPP
