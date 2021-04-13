#include "pch.h"
#include "../ScanMetadata.hpp"

#include <iostream>

#include<Poco/JSON/Stringifier.h>

TEST(ScanMetadata, TestConstructorWithDefaultParams) {
	ScanMetadata scan_metadata{};
}

TEST(ScanMetadata, TestConstructorWithFullParams) {
	ScanSettings scan_settings{};
	ContentMetadata content_metadata{};
	ScanMetadata scan_metadata{ scan_settings, content_metadata };
}

TEST(ScanMetadata, TestgetScanSettings) {
	ScanSettings scan_settings{ true, 1, true, true };
	ContentMetadata content_metadata{};
	ScanMetadata scan_metadata{ scan_settings, content_metadata };
	auto result = scan_metadata.getScanSettings().value();
	EXPECT_EQ(result.getAllowUpstreamMetadata(), scan_settings.getAllowUpstreamMetadata());
	EXPECT_EQ(result.getMaxNested(), scan_settings.getMaxNested());
	EXPECT_EQ(result.getScanArchives(), scan_settings.getScanArchives());
	EXPECT_EQ(result.getStopOnFirst(), scan_settings.getStopOnFirst());
}

TEST(ScanMetadata, TestgetContentMetadata) {
	ScanSettings scan_settings{};
	ContentMetadata content_metadata{ "SHA1", "URI", 1, "content_type", "charset" };
	ScanMetadata scan_metadata{ scan_settings, content_metadata };
	auto result = scan_metadata.getContentMetadata().value();
	EXPECT_EQ(result.getCharset(), content_metadata.getCharset());
	EXPECT_EQ(result.getContentLength(), content_metadata.getContentLength());
	EXPECT_EQ(result.getContentType(), content_metadata.getContentType());
	EXPECT_EQ(result.getSHA1(), content_metadata.getSHA1());
	EXPECT_EQ(result.getURI(), content_metadata.getURI());
}

TEST(ScanMetadata, TesttoJsonObjectWithEmptyItems) {
	ScanSettings scan_settings{};
	ContentMetadata content_metadata{};
	ScanMetadata scan_metadata{ scan_settings, content_metadata };
	auto result_object = scan_metadata.toJsonObject();
	std::stringstream result_json_steam;
	Poco::JSON::Stringifier::stringify(result_object, result_json_steam);
	std::string exexpected_result = "{\"content_meta\":{},\"scan_settings\":{}}";
	EXPECT_EQ(result_json_steam.str(), exexpected_result);
}
TEST(ScanMetadata, TesttoJsonObjectWithExistingItems) {
	ScanSettings scan_settings{ true, 1, true, true };
	ContentMetadata content_metadata{ "SHA1", "URI", 1, "content_type", "charset" };
	ScanMetadata scan_metadata{ scan_settings, content_metadata };
	auto result_object = scan_metadata.toJsonObject();
	std::stringstream result_json_steam;
	Poco::JSON::Stringifier::stringify(result_object, result_json_steam);
	std::string exexpected_result = "{\"content_meta\":{\"charset\":\"charset\",\"content_length\":1,\"content_type\":"
		"\"content_type\",\"sha1\":\"SHA1\",\"uri\":\"URI\"},\"scan_settings\":{\"allow_upstream_metadata\":"
		"true,\"max_nested\":1,\"scan_settings\":true,\"stop_on_first\":true}}";
	EXPECT_EQ(result_json_steam.str(), exexpected_result);
}
