#ifndef AUTHTOKEN_HPP
#define AUTHTOKEN_HPP

#include <string>

class AuthToken {
public:
	AuthToken(const std::string& auth_token, int expiration);
	std::string getToken();
	int getExpiration();

private:
	const std::string &auth_token;
	const int expiration;
};
#endif // AUTHTOKEN_HPP
