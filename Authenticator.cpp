#include "Authenticator.hpp"

Authenticator::Authenticator(
	const std::string &authorization_address,
	const std::string &client_ID,
	const std::string &client_secret,
	const std::set<std::string> &scopes) :
	token_fetcher{ std::make_unique<AuthTokenFetcher>(authorization_address) },
		client_ID{ client_ID },
		client_secret{ client_secret },
		scopes{ scopes } 
{};

AuthToken Authenticator::fetchToken()
{
	return token_fetcher->fetch(client_ID, client_secret, scopes);
}

