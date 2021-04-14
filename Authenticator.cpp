#include "Authenticator.hpp"

Authenticator::Authenticator(
	std::unique_ptr<AuthTokenFetcher> auth_token_fetcher,
	const std::string &client_ID,
	const std::string &client_secret,
	const std::set<std::string> &scopes) :
		token_fetcher { std::move(auth_token_fetcher) },
		client_ID{ client_ID },
		client_secret{ client_secret },
		scopes{ scopes } 
{};

AuthToken Authenticator::fetchToken()
{
	return token_fetcher->fetch(client_ID, client_secret, scopes);
}

