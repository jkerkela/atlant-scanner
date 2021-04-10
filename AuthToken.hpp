#ifndef AUTHTOKEN_HPP
#define AUTHTOKEN_HPP

#include <string>

class AuthToken {
public:
	AuthToken() = default;
	AuthToken(const std::string& auth_token, const int expiration);
	std::string getToken();
	int getExpiration();

private:
	std::string auth_token;
	int expiration;
};
#endif // AUTHTOKEN_HPP
