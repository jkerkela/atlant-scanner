#ifndef SCANMETADATA_HPP
#define SCANMETADATA_HPP

#include "ScanSettings.hpp"
#include "ContentMetadata.hpp"
class ScanMetadata {
public:
	ScanMetadata(const ScanSettings& scan_settings = ScanSettings{}, const ContentMetadata& content_metadata = ContentMetadata{});
	ScanSettings getScanSettings();
	ContentMetadata getContentMetadata();

private:
	ScanSettings scan_settings;
	ContentMetadata content_metadata;
};
#endif // SCANMETADATA_HPP
