#include "pch.h"

#include "MultiPartRequestBuilder.hpp"

TEST(MultiPartRequestBuilder, TestMultiPartRequestBuilderConstructor) {
	EXPECT_NO_THROW(new MultiPartRequestBuilder());
}

TEST(MultiPartRequestBuilder, TestMultiPartRequestBuildergetBoundary) {
	// PRE: Initialize test constants
	auto expected_bounary_byte_count = requestBuilder::BOUNDARY_BYTES + 54;

	// ACTION: Call getBoundary
	MultiPartRequestBuilder request_builder{};
	auto result = request_builder.getBoundary();

	// VERIFY: Verify that value is expected
	EXPECT_EQ(expected_bounary_byte_count, result.length());
}

class TestMultiPartRequestBuilder : public ::testing::Test
{
protected:

	MultiPartRequestBuilder request_builder{};
	std::string boundary;
	std::string initiator_string;
	std::string separator_string;
	std::string terminator_string;

	void SetUp() override {
		boundary = request_builder.getBoundary();
		initiator_string = "--" + boundary + "\r\n";
		separator_string = "\r\n--" + boundary + "\r\n";
		terminator_string = "\r\n--" + boundary + "--\r\n";
	}

	void TearDown() override {}
};

TEST_F(TestMultiPartRequestBuilder, TestMultiPartRequestencodeWithNoParts) {
	// PRE: Initialize test constants
	auto expected_result_size = initiator_string.length() + terminator_string.length();

	// ACTION: Call encode with empty MultiPartRequestBuilder
	auto result = request_builder.encode();

	// VERIFY: Verify that result is expected
	EXPECT_TRUE(result.find(initiator_string) != std::string::npos);
	EXPECT_TRUE(result.find(terminator_string) != std::string::npos);
	EXPECT_FALSE(result.find(separator_string) != std::string::npos);
	EXPECT_EQ(expected_result_size, result.length());
}

TEST_F(TestMultiPartRequestBuilder, TestMultiPartRequestencodeSinglePart) {
	// PRE: Initialize test constants
	std::string part_name = "part_name";
	std::string content_type = "content_type";
	auto part_name_string = "Content-Disposition: form-data; name=\"" + part_name + "\r\n";
	auto part_content_type_string = "Content - Type: " + content_type + "\r\n\r\n";
	std::string part_content_string = "some content";
	auto expected_result_size = initiator_string.length()
		+ part_name_string.length()
		+ part_content_type_string.length()
		+ part_content_string.length()
		+ terminator_string.length();


	// PRE: Add request parts to request builder
	MultiPartRequestBuilder::Part part{ part_name, content_type, part_content_string };
	request_builder.addPart(part);

	// ACTION: Call MultiPartRequestBuilder encode
	auto result = request_builder.encode();

	// VERIFY: Verify that result is expected
	EXPECT_TRUE(result.find(initiator_string) != std::string::npos);
	EXPECT_TRUE(result.find(terminator_string) != std::string::npos);
	EXPECT_FALSE(result.find(separator_string) != std::string::npos);
	EXPECT_TRUE(result.find(part_name_string) != std::string::npos);
	EXPECT_TRUE(result.find(part_content_type_string) != std::string::npos);
	EXPECT_TRUE(result.find(part_content_string) != std::string::npos);
	EXPECT_EQ(expected_result_size, result.length());
}

TEST_F(TestMultiPartRequestBuilder, TestMultiPartRequestencodePartWithEscapedName) {
	// PRE: Initialize test constants
	std::string part_name_with_to_be_escaped_chars = "part_name\"\"";
	std::string part_name_after_escapes = "part_name\\\"\\\"";
	std::string content_type = "content_type";
	auto part_name_string = "Content-Disposition: form-data; name=\"" + part_name_after_escapes + "\r\n";
	auto part_content_type_string = "Content - Type: " + content_type + "\r\n\r\n";
	std::string part_content_string = "some content";
	auto expected_result_size = initiator_string.length()
		+ part_name_string.length()
		+ part_content_type_string.length()
		+ part_content_string.length()
		+ terminator_string.length();


	// PRE: Add request parts to request builder
	MultiPartRequestBuilder::Part part{ part_name_with_to_be_escaped_chars, content_type, part_content_string };
	request_builder.addPart(part);

	// ACTION: Call MultiPartRequestBuilder encode
	auto result = request_builder.encode();

	// VERIFY: Verify that result is expected
	EXPECT_TRUE(result.find(initiator_string) != std::string::npos);
	EXPECT_TRUE(result.find(terminator_string) != std::string::npos);
	EXPECT_FALSE(result.find(separator_string) != std::string::npos);
	EXPECT_TRUE(result.find(part_name_string) != std::string::npos);
	EXPECT_TRUE(result.find(part_content_type_string) != std::string::npos);
	EXPECT_TRUE(result.find(part_content_string) != std::string::npos);
	EXPECT_EQ(expected_result_size, result.length());
}

TEST_F(TestMultiPartRequestBuilder, TestMultiPartRequestencodeMultipleParts) {
	// PRE: Initialize test constants
	std::string part_name_1 = "part_name";
	std::string content_type_1 = "content_type";
	auto part_name_string_1 = "Content-Disposition: form-data; name=\"" + part_name_1 + "\r\n";
	auto part_content_type_string_1 = "Content - Type: " + content_type_1 + "\r\n\r\n";
	std::string part_content_string_1 = "some content";

	std::string part_name_2 = "part_name2";
	std::string content_type_2 = "content_type2";
	auto part_name_string_2 = "Content-Disposition: form-data; name=\"" + part_name_2 + "\r\n";
	auto part_content_type_string_2 = "Content - Type: " + content_type_2 + "\r\n\r\n";
	std::string part_content_string_2 = "some content2";
	auto expected_result_size = initiator_string.length()
		+ part_name_string_1.length()
		+ part_content_type_string_1.length()
		+ part_content_string_1.length()
		+ separator_string.length()
		+ part_name_string_2.length()
		+ part_content_type_string_2.length()
		+ part_content_string_2.length()
		+ terminator_string.length();


	// PRE: Add request parts to request builder
	MultiPartRequestBuilder::Part part1{ part_name_1, content_type_1, part_content_string_1 };
	MultiPartRequestBuilder::Part part2{ part_name_2, content_type_2, part_content_string_2 };
	request_builder.addPart(part1);
	request_builder.addPart(part2);

	// ACTION: Call MultiPartRequestBuilder encode
	auto result = request_builder.encode();

	// VERIFY: Verify that result is expected
	EXPECT_TRUE(result.find(initiator_string) != std::string::npos);
	EXPECT_TRUE(result.find(terminator_string) != std::string::npos);
	EXPECT_TRUE(result.find(separator_string) != std::string::npos);
	EXPECT_TRUE(result.find(part_name_string_1) != std::string::npos);
	EXPECT_TRUE(result.find(part_content_type_string_1) != std::string::npos);
	EXPECT_TRUE(result.find(part_content_string_1) != std::string::npos);
	EXPECT_TRUE(result.find(part_name_string_2) != std::string::npos);
	EXPECT_TRUE(result.find(part_content_type_string_2) != std::string::npos);
	EXPECT_TRUE(result.find(part_content_string_2) != std::string::npos);
	EXPECT_EQ(expected_result_size, result.length());
}
