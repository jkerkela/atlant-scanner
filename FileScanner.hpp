#ifndef FILESCANNER_HPP
#define FILESCANNER_HPP

#include <string>

#include <Poco/Net/HTTPClientSession.h>

#include "Authenticator.hpp"
#include "AuthToken.hpp"

using namespace Poco::Net;

class FileScanner {

public:
	FileScanner(const std::string &scanningAddress, const Authenticator &authenticator);
	void refreshToken();

private:
	const std::string &base_URI;
	const std::string &scan_endpoint;
	const std::string &poll_endpoint;
	HTTPClientSession client;
	const Authenticator &authenticator;
	AuthToken auth_token;
};
#endif // FILESCANNER_HPP