
#include "TokenFetcher.hpp"

namespace {
	constexpr auto AUDIENCE = "f-secure-atlant";
	constexpr auto API_PREFIX = "f-secure-atlant";
}

TokenFetcher::TokenFetcher(const std::string &authorization_address) 
	: client{ HTTPClientSession() }, token_endpoint{ std::string(API_PREFIX + authorization_address) }
{}
