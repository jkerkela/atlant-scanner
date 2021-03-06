#ifndef SCANMETADATA_HPP
#define SCANMETADATA_HPP

#include "ContentMetadata.hpp"
#include "ScanSettings.hpp"

#include<optional>

#include <Poco/JSON/Object.h>

class ScanMetadata {
public:
	ScanMetadata(const std::optional<ScanSettings> &scan_settings = std::nullopt,
		const std::optional<ContentMetadata> &content_metadata = std::nullopt);
	std::optional<ScanSettings> getScanSettings() { return scan_settings; };
	std::optional<ContentMetadata> getContentMetadata() { return content_metadata; };
	Poco::JSON::Object::Ptr toJsonObject();

private:
	std::optional<ScanSettings> scan_settings;
	std::optional<ContentMetadata> content_metadata;
};
#endif // SCANMETADATA_HPP
