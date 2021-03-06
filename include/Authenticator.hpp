#ifndef AUTHENTICATOR_HPP
#define AUTHENTICATOR_HPP

#include "AuthToken.hpp"
#include "AuthTokenFetcher.hpp"

#include <string>
#include <set>
#include <memory>


class Authenticator {

public:
	Authenticator(
		std::unique_ptr<AuthTokenFetcher> auth_token_fetcher,
		const std::string &client_ID,
		const std::string &client_secret,
		const std::set<std::string> &scopes);
	AuthToken fetchToken();

private:
	std::unique_ptr<AuthTokenFetcher> token_fetcher;
	std::string client_ID;
	std::string client_secret;
	std::set<std::string> scopes;
};

#endif // AUTHENTICATOR_HPP
