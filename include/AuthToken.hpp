#ifndef AUTHTOKEN_HPP
#define AUTHTOKEN_HPP

#include <string>

class AuthToken {
public:
	AuthToken() = default;
	AuthToken(const std::string &auth_token, const int &expiration)
		: auth_token{ auth_token }, expiration{ expiration } {};
	std::string getToken() { return auth_token; };
	int getExpiration() { return expiration; };;

private:
	std::string auth_token;
	int expiration;
};
#endif // AUTHTOKEN_HPP
