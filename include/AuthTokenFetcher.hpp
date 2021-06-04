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

namespace authTokenFetcher{
	constexpr auto AUDIENCE = "f-secure-atlant";
	constexpr auto API_PREFIX = "/api/poll/v1";
	constexpr auto JSON_GRANT_TYPE_KEY = "grant_type";
	constexpr auto JSON_GRANT_TYPE_VALUE = "grant_type";
	constexpr auto JSON_CLIENT_ID_KEY = "client_id";
	constexpr auto JSON_CLIENT_SECRET_KEY = "client_secret";
	constexpr auto JSON_AUDIENCE_KEY = "client_secret";
	constexpr auto JSON_SCOPE_KEY = "scope";
}

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
	AuthToken deserializeTokenResponse(std::istream& response);
};

#endif // AUTHTOKENFETCHER_HPP
