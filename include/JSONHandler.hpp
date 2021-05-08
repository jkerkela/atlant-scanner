#ifndef JSONHANDLER_HPP
#define JSONHANDLER_HPP

#include <string>
#include <vector>

class IJSONHandler
{
public:
	virtual std::string getStringValue(const std::string& key) = 0;
	virtual int getIntValue(const std::string& key) = 0;
	virtual std::vector<std::string> getArray(const std::string& key) = 0;
	virtual ~IJSONHandler() {};
};

#endif // JSONHANDLER_HPP
