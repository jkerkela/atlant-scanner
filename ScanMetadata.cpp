#include "ScanMetadata.hpp"

ScanMetadata::ScanMetadata(const ScanSettings& scan_settings, const ContentMetadata& content_metadata)
	: scan_settings{ scan_settings }, content_metadata{ content_metadata }
{}

ScanSettings ScanMetadata::getScanSettings()
{
	return scan_settings;
}

ContentMetadata ScanMetadata::getContentMetadata()
{
	return content_metadata;
}