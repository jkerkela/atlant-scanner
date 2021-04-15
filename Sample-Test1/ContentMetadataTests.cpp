#include "pch.h"
#include "ContentMetadata.hpp"

#include <iostream>

#include<Poco/JSON/Stringifier.h>

TEST(ContentMetadata, TestConstructorWithDefaultParams) {
	ContentMetadata content_metadata{};
}

TEST(ContentMetadata, TestConstructorWithFullParams) {
	ContentMetadata content_metadata{ "SHA1", "URI", 1, "content_type", "charset" };
}

TEST(ContentMetadata, TestgetSHA1) {
	std::string expected_result = "SHA1";
	ContentMetadata content_metadata{ expected_result, "URI", 1, "content_type", "charset" };
	EXPECT_EQ(content_metadata.getSHA1().value(), expected_result);
}

TEST(ContentMetadata, TestgetURI) {
	std::string expected_result = "URI";
	ContentMetadata content_metadata{ "SHA1", expected_result, 1, "content_type", "charset" };
	EXPECT_EQ(content_metadata.getURI().value(), expected_result);
}

TEST(ContentMetadata, TestgetContentLength) {
	auto expected_result = 1;
	ContentMetadata content_metadata{ "SHA1", "URI", expected_result, "content_type", "charset" };
	EXPECT_EQ(content_metadata.getContentLength().value(), expected_result);
}

TEST(ContentMetadata, TestgetContentType) {
	std::string expected_result = "content_type";
	ContentMetadata content_metadata{ "SHA1", "URI", 1, expected_result, "charset" };
	EXPECT_EQ(content_metadata.getContentType().value(), expected_result);
}

TEST(ContentMetadata, TesttoJsonObjectWithEmptyItems) {
	ContentMetadata content_metadata{};
	auto result_object = content_metadata.toJsonObject();
	std::stringstream result_json_steam;
	Poco::JSON::Stringifier::stringify(result_object, result_json_steam);
	std::string exexpected_result = "{}";
	EXPECT_EQ(result_json_steam.str(), exexpected_result);
}

TEST(ContentMetadata, TesttoJsonObjectWithExistingItems) {
	ContentMetadata content_metadata{ "SHA1", "URI", 1, "content_type", "charset" };
	auto result_object = content_metadata.toJsonObject();
	std::stringstream result_json_steam;
	Poco::JSON::Stringifier::stringify(result_object, result_json_steam);
	std::string exexpected_result = "{\"charset\":\"charset\",\"content_length\":1,\"content_type\":\"content_type\",\"sha1\":\"SHA1\",\"uri\":\"URI\"}";
	EXPECT_EQ(result_json_steam.str(), exexpected_result);
}