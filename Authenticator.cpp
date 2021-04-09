#include "Authenticator.hpp"

Authenticator::Authenticator(const std::string &authorization_address,
	const std::string &client_ID,
	const std::string &client_secret,
	const std::set<std::string> &scopes) :
	token_fetcher{ std::make_unique<TokenFetcher>(authorization_address) },
		client_ID{ client_ID },
		client_secret{ client_secret },
		scopes{ scopes } {
	//TODO: implement as https://github.com/F-Secure/atlant-api/blob/master/java/scanner/src/main/com/fsecure/atlant/examples/scanner/Authenticator.java
};

