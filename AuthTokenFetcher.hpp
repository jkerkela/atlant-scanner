#ifndef AUTHTOKENFETCHER_HPP
#define AUTHTOKENFETCHER_HPP

#include <string>
#include <set>
#include <unordered_map>

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>

#include "AuthToken.hpp"
#include "ScanResult.hpp"

using namespace Poco::Net;

class AuthTokenFetcher {

public:
	AuthTokenFetcher(const std::string &authorization_address);
	AuthToken fetch(const std::string &URI,
		const std::string &client_ID,
		const std::string &client_secret,
		const std::set<std::string> &scopes);

private:
	const std::string &token_endpoint;
	const HTTPClientSession &client;
	std::string HTTP_request_body;

	HTTPRequest buildTokenRequest(const std::string &client_ID, const std::string &client_secret, const std::set<std::string> &scopes);
	std::string encodeScopes(const std::set<std::string>& scopes);
	std::string encodeQueryParameters(const std::unordered_map<std::string, std::string> &params);
	AuthToken deserializeTokenResponse(std::istream& response);
	ScanResult deserializeScanResponse(std::istream& response);



};

#endif // AUTHTOKENFETCHER_HPP
