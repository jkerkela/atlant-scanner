#include "pch.h"
#include "../ScanSettings.hpp"

#include <iostream>

#include<Poco/JSON/Stringifier.h>

TEST(ScanSettings, TestConstructorWithDefaultParams) {
	ScanSettings scan_settings{};
}

TEST(ScanSettings, TestConstructorWithFullParams) {
	ScanSettings scan_settings{ true, 1, true, true };
}

TEST(ScanSettings, TestgetScanArchives) {
	ScanSettings scan_settings{ true, 0, false, false };
	EXPECT_EQ(scan_settings.getScanArchives(), true);
}

TEST(ScanSettings, TestgetMaxNested) {
	ScanSettings scan_settings{ false, 6, false, false };
	EXPECT_EQ(scan_settings.getMaxNested(), 6);
}

TEST(ScanSettings, TestgetStopOnFirst) {
	ScanSettings scan_settings{ false, 0, true, false };
	EXPECT_EQ(scan_settings.getStopOnFirst(), true);
}

TEST(ScanSettings, TestgetAllowUpstreamMetadata) {
	ScanSettings scan_settings{ false, 0, false, true };
	EXPECT_EQ(scan_settings.getAllowUpstreamMetadata(), true);
}

TEST(ScanSettings, TesttoJsonObjectWithEmptyScanSettings) {
	ScanSettings scan_settings{};
	auto result_object = scan_settings.toJsonObject();
	std::stringstream result_json_steam;
	Poco::JSON::Stringifier::stringify(result_object, result_json_steam);
	std::string exexpected_result = "{}";
	EXPECT_EQ(result_json_steam.str(), exexpected_result);
}

TEST(ScanSettings, TesttoJsonObjectWithExistingScanSettings) {
	ScanSettings scan_settings{ true, 1, true, true };
	auto result_object = scan_settings.toJsonObject();
	std::stringstream result_json_steam;
	Poco::JSON::Stringifier::stringify(result_object, result_json_steam);
	std::string exexpected_result = "{\"allow_upstream_metadata\":true,\"max_nested\":1,\"scan_settings\":true,\"stop_on_first\":true}";
	EXPECT_EQ(result_json_steam.str(), exexpected_result);
}
