#include "pch.h"
#include "../AuthTokenFetcher.hpp"

TEST(AuthTokenFetcher, TestConstructor) {
	AuthTokenFetcher auth_token_fetcher{ "authorization_address" };
}


//TODO: finalize: implement mocks using HTTPCommunication.hpp, use it, add required assert(s) and enable test
//TEST(AuthTokenFetcher, Testfetch) {
//	AuthTokenFetcher auth_token_fetcher{ "http://example.com" };
//	std::set<std::string> scopes{ "scope1" };
//	auto result_token = auth_token_fetcher.fetch("client_ID", "secret", scopes);
//}