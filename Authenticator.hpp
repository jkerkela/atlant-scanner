#ifndef AUTHENTICATOR_HPP
#define AUTHENTICATOR_HPP

#include <string>
#include <set>

#include "AuthTokenFetcher.hpp"
#include "AuthToken.hpp"

class Authenticator {

public:
	Authenticator(const std::string &authorization_address,
		const std::string &client_ID,
		const std::string &client_secret,
		const std::set<std::string> &scopes);
	AuthToken fetchToken(const std::string& URI);

private:
	std::unique_ptr<AuthTokenFetcher> token_fetcher;
	std::string client_ID;
	std::string client_secret;
	std::set<std::string> scopes;
};

#endif // AUTHENTICATOR_HPP
