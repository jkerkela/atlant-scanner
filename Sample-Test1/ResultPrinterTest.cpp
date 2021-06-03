#include "pch.h"

#include "ResultPrinter.hpp"

#include "ScanResult.hpp"
#include "Detection.hpp"

#include <string>
#include <vector>

class TestResultPrinter : public ::testing::Test
{
protected:
	void SetUp() override {
		testing::internal::CaptureStdout();
	}

	void TearDown() override {}
};

TEST_F(TestResultPrinter, TestValidSigleDetectionResultPrint) {
	// PRE: initialize result to print and expected prints
	std::vector<Detection> detections{ Detection{Detection::Category::SUSPICIOUS, "some_detection", "member_name"} };
	ScanResult scan_result{ ScanResult::Status::COMPLETE, ScanResult::Result::SUSPICIOUS, detections };
	auto expected_result_print = "Result: suspicious";
	auto expected_detection_name_print = "Name: some_detection";
	auto expected_detection_category = "Category: suspicious";
	auto expected_detection_member_name = "Member name: member_name";

	// ACTION: Call ResultPrinter with result
	ResultPrinter::print(scan_result);

	// VERIFY: Verify that result printing contains expected items
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_TRUE(output.find(expected_result_print) != std::string::npos);
	EXPECT_TRUE(output.find(expected_detection_name_print) != std::string::npos);
	EXPECT_TRUE(output.find(expected_detection_category) != std::string::npos);
	EXPECT_TRUE(output.find(expected_detection_member_name) != std::string::npos);
}

TEST_F(TestResultPrinter, TestValidSigleDetectionResultPrintWithoutMemberName) {
	// PRE: initialize result to print and expected prints
	std::vector<Detection> detections{ Detection{Detection::Category::SUSPICIOUS, "some_detection"} };
	ScanResult scan_result{ ScanResult::Status::COMPLETE, ScanResult::Result::SUSPICIOUS, detections };
	auto expected_result_print = "Result: suspicious";
	auto expected_detection_name_print = "Name: some_detection";
	auto expected_detection_category = "Category: suspicious";
	auto not_expected_detection_member_name = "Member name:";

	// ACTION: Call ResultPrinter with result
	ResultPrinter::print(scan_result);

	// VERIFY: Verify that result printing contains expected items
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_TRUE(output.find(expected_result_print) != std::string::npos);
	EXPECT_TRUE(output.find(expected_detection_name_print) != std::string::npos);
	EXPECT_TRUE(output.find(expected_detection_category) != std::string::npos);
	EXPECT_FALSE(output.find(not_expected_detection_member_name) != std::string::npos);
}

TEST_F(TestResultPrinter, TestValidMultipleDetectionsResultPrint) {
	// PRE: initialize result to print and expected prints
	std::vector<Detection> detections{ 
		Detection{Detection::Category::SUSPICIOUS, "some_detection"},
		Detection{Detection::Category::HARMFUL, "another_detection", "member_name"}
	};
	ScanResult scan_result{ ScanResult::Status::COMPLETE, ScanResult::Result::HARMFUL, detections };
	auto expected_result_print = "Result: harmful";
	auto expected_detection_name_print_1 = "Name: some_detection";
	auto expected_detection_category_1 = "Category: suspicious";
	auto expected_detection_name_print_2 = "Name: another_detection";
	auto expected_detection_category_2 = "Category: harmful";
	auto expected_detection_member_name_2 = "Member name: member_name";

	// ACTION: Call ResultPrinter with result
	ResultPrinter::print(scan_result);

	// VERIFY: Verify that result printing contains expected items
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_TRUE(output.find(expected_result_print) != std::string::npos);
	EXPECT_TRUE(output.find(expected_detection_name_print_1) != std::string::npos);
	EXPECT_TRUE(output.find(expected_detection_category_1) != std::string::npos);
	EXPECT_TRUE(output.find(expected_detection_name_print_2) != std::string::npos);
	EXPECT_TRUE(output.find(expected_detection_category_2) != std::string::npos);
	EXPECT_TRUE(output.find(expected_detection_member_name_2) != std::string::npos);
}