#include "pch.h"
#include "ScanResult.hpp"

TEST(ScanResult, TestConstructorWithDefaultParams) {
	std::list<Detection> detections{ Detection{Detection::Category::SUSPICIOUS, "some_detection"} };
	ScanResult scan_result{ ScanResult::Status::COMPLETE, ScanResult::Result::SUSPICIOUS, detections };
}

TEST(ScanResult, TestConstructorWithFullParams) {
	std::list<Detection> detections{ Detection{Detection::Category::SUSPICIOUS, "some_detection"} };
	ScanResult scan_result{ ScanResult::Status::COMPLETE, ScanResult::Result::SUSPICIOUS, detections, 1, "poll_url" };
}

TEST(ScanResult, TestgetStatus) {
	auto expected_result = ScanResult::Status::COMPLETE;
	std::list<Detection> detections{ Detection{Detection::Category::SUSPICIOUS, "some_detection"} };
	ScanResult scan_result{ expected_result, ScanResult::Result::SUSPICIOUS, detections };
	EXPECT_EQ(scan_result.getStatus(), expected_result);
}

TEST(ScanResult, TestgetResult) {
	auto expected_result = ScanResult::Result::SUSPICIOUS;
	std::list<Detection> detections{ Detection{Detection::Category::SUSPICIOUS, "some_detection"} };
	ScanResult scan_result{ ScanResult::Status::COMPLETE, expected_result, detections };
	EXPECT_EQ(scan_result.getResult(), expected_result);
}

TEST(ScanResult, TestgetDetections) {
	std::list<Detection> detections{ Detection{Detection::Category::SUSPICIOUS, "some_detection"} };
	ScanResult scan_result{ ScanResult::Status::COMPLETE, ScanResult::Result::SUSPICIOUS, detections };
	auto result_detection_item = scan_result.getDetections().front();
	auto expected_detection_item = detections.front();

	EXPECT_EQ(scan_result.getDetections().size(), detections.size());
	EXPECT_EQ(result_detection_item.getCategory(), expected_detection_item.getCategory());
	EXPECT_EQ(result_detection_item.getName(), expected_detection_item.getName());
}

TEST(ScanResult, TestgetPollURL) {
	std::string expected_result = "poll_url";
	std::list<Detection> detections{ Detection{Detection::Category::SUSPICIOUS, "some_detection"} };
	ScanResult scan_result{ ScanResult::Status::COMPLETE, ScanResult::Result::SUSPICIOUS, detections, 1, expected_result };
	EXPECT_EQ(scan_result.getPollURL(), expected_result);
}

TEST(ScanResult, TestsetPollURL) {
	std::string expected_result = "poll_url_set";
	std::list<Detection> detections{ Detection{Detection::Category::SUSPICIOUS, "some_detection"} };
	ScanResult scan_result{ ScanResult::Status::COMPLETE, ScanResult::Result::SUSPICIOUS, detections, 1, "poll_url" };
	scan_result.setPollURL(expected_result);
	EXPECT_EQ(scan_result.getPollURL(), expected_result);
}

TEST(ScanResult, TestgetRetryAfter) {
	auto expected_result = 1;
	std::list<Detection> detections{ Detection{Detection::Category::SUSPICIOUS, "some_detection"} };
	ScanResult scan_result{ ScanResult::Status::COMPLETE, ScanResult::Result::SUSPICIOUS, detections, expected_result, "poll_url" };
	EXPECT_EQ(scan_result.getRetryAfter(), expected_result);
}

TEST(ScanResult, TestsetRetryAfter) {
	auto expected_result = 3;
	std::list<Detection> detections{ Detection{Detection::Category::SUSPICIOUS, "some_detection"} };
	ScanResult scan_result{ ScanResult::Status::COMPLETE, ScanResult::Result::SUSPICIOUS, detections, 1, "poll_url" };
	scan_result.setRetryAfter(expected_result);
	EXPECT_EQ(scan_result.getRetryAfter(), expected_result);
}
