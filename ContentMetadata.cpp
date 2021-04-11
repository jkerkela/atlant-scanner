#include "ContentMetadata.hpp"

ContentMetadata::ContentMetadata(
	const std::optional<std::string>& SHA1,
	const std::optional<std::string>& UR,
	const std::optional<int> content_length,
	const std::optional<std::string>& content_type,
	const std::optional<std::string>& charset
) : SHA1{ SHA1 }, URI{ URI }, content_length{ content_length }, content_type{ content_type }, charset{ charset }
{}

Poco::JSON::Object::Ptr ContentMetadata::to_json_object()
{
	Poco::JSON::Object::Ptr json_object = new Poco::JSON::Object();

	if (SHA1 != std::nullopt) {
		json_object->set("sha1", SHA1.value());
	}

	if (URI != std::nullopt) {
		json_object->set("uri", URI.value());
	}
	if (content_length != std::nullopt) {
		json_object->set("content_length", content_length.value());
	}
	if (content_type != std::nullopt) {
		json_object->set("content_type", content_type.value());
	}
	if (charset != std::nullopt) {
		json_object->set("charset", charset.value());
	}
	return json_object;
}