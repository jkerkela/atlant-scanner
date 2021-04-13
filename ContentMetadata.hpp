#ifndef CONTENTMETADATA_HPP
#define CONTENTMETADATA_HPP

#include<string>
#include<optional>

#include <Poco/JSON/Object.h>

class ContentMetadata {

public:
	ContentMetadata(
		const std::optional<std::string> &SHA1 = std::nullopt,
		const std::optional<std::string> &URI = std::nullopt,
		const std::optional<int> content_length = std::nullopt,
		const std::optional<std::string> &content_type = std::nullopt,
		const std::optional<std::string> &charset = std::nullopt
	);

	std::optional<std::string> getSHA1() { return SHA1; };
	std::optional<std::string> getURI() { return URI; };
	std::optional<int> getContentLength() { return content_length; };
	std::optional<std::string> getContentType() { return content_type;  };
	std::optional<std::string> getCharset() { return charset; };
	Poco::JSON::Object::Ptr toJsonObject();


private:
	std::optional<std::string> SHA1;
	std::optional<std::string> URI;
	std::optional<int> content_length;
	std::optional<std::string> content_type;
	std::optional<std::string> charset;
};
#endif // CONTENTMETADATA_HPP
