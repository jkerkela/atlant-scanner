#ifndef JSONHANDLERIMPL_HPP
#define JSONHANDLERIMPL_HPP

#include <JSONHandler.hpp>
#include <Poco/JSON/Parser.h>

class JSONHandlerImpl : public IJSONHandler
{
public:

	JSONHandlerImpl(std::istream& json);
	JSONHandlerImpl(const std::string& json);
	std::string getStringValue(const std::string& key) override;
	int getIntValue(const std::string& key) override;
	std::vector<std::string> getArray(const std::string& key) override;

private:
	Poco::JSON::Object::Ptr json_obj;
};

#endif // JSONHANDLERIMPL_HPP
