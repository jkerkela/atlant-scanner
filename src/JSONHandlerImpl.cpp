#include <JSONHandlerImpl.hpp>

JSONHandlerImpl::JSONHandlerImpl(std::istream& json)
{
	Poco::JSON::Parser parser;
	auto result = parser.parse(json);
	json_obj = result.extract<Poco::JSON::Object::Ptr>();
}

JSONHandlerImpl::JSONHandlerImpl(const std::string& json)
{
	Poco::JSON::Parser parser;
	auto result = parser.parse(json);
	json_obj = result.extract<Poco::JSON::Object::Ptr>();
}

std::string JSONHandlerImpl::getStringValue(const std::string& key)
{
	return json_obj->getValue<std::string>(key);
}

int JSONHandlerImpl::getIntValue(const std::string& key)
{
	return json_obj->getValue<int>(key);
}

std::vector<std::string> JSONHandlerImpl::getArray(const std::string& key)
{
	std::vector<std::string> result;
	auto jsonArray = json_obj->getArray("detections");
	for (Poco::JSON::Array::ConstIterator it = jsonArray->begin(); it != jsonArray->end(); ++it) {
		result.emplace_back(it->convert<std::string>());
	}
	return result;
}
