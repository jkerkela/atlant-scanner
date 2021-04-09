#ifndef FILESCANNER_HPP
#define FILESCANNER_HPP

#include <string>

#include <Poco/Net/HTTPClientSession.h>

#include "Authenticator.hpp"

using namespace Poco::Net;

class FileScanner {

public:
	FileScanner(const std::string &scanningAddress, const Authenticator &authenticator);

private:
	std::string base_URI; //was URI
	std::string scan_endpoint; //was URI
	std::string poll_endpoint; //was URI
	HTTPClientSession client;
	const Authenticator &authenticator;
	// Token token; TODO
};
#endif // FILESCANNER_HPP