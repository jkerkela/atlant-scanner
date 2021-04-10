#include "AuthToken.hpp"

AuthToken::AuthToken(const std::string& auth_token, int expiration)
	: auth_token{ auth_token }, expiration { expiration }
{};

std::string AuthToken::getToken()
{
	return auth_token;
}

int AuthToken::getExpiration()
{
	return expiration;
}
