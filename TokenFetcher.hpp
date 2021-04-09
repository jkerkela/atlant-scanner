#ifndef TOKENFETCHER_HPP
#define TOKENFETCHER_HPP

#include <string>

#include <Poco/Net/HTTPClientSession.h>

using namespace Poco::Net;

class TokenFetcher {

public:
	TokenFetcher(const std::string& authorization_address);

private:
	const std::string &token_endpoint;
	const HTTPClientSession &client;

};

#endif // TOKENFETCHER_HPP
