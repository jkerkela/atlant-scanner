#include "ScanMetadata.hpp"

ScanMetadata::ScanMetadata(const std::optional<ScanSettings>& scan_settings, const std::optional<ContentMetadata>& content_metadata)
	: scan_settings{ scan_settings }, content_metadata{ content_metadata }
{}

Poco::JSON::Object::Ptr ScanMetadata::to_json_object()
{
	Poco::JSON::Object::Ptr json_object = new Poco::JSON::Object();
	if (scan_settings != std::nullopt) {
		json_object->set("scan_settings", scan_settings.value().toJsonObject());
	}

	if (content_metadata != std::nullopt) {
		json_object->set("content_meta", content_metadata.value().to_json_object());
	}

	return json_object;
}