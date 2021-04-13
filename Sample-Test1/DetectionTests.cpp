#include "pch.h"
#include "../Detection.hpp"

TEST(Detection, TestConstructorWithDefaultParams) {
	Detection detection{ Detection::Category::SUSPICIOUS, "name" };
}

TEST(Detection, TestConstructorWithFullParams) {
	Detection detection{ Detection::Category::SUSPICIOUS, "name", "member_name" };
}

TEST(Detection, TestgetCategory) {
	auto expected_result = Detection::Category::SUSPICIOUS;
	Detection detection{ expected_result, "name", "member_name" };
	EXPECT_EQ(detection.getCategory(), expected_result);
}

TEST(Detection, TestgetName) {
	std::string expected_result = "name";
	Detection detection{ Detection::Category::SUSPICIOUS, expected_result, "member_name" };
	EXPECT_EQ(detection.getName(), expected_result);
}

TEST(Detection, TestgetMemberName) {
	std::string expected_result = "member_name";
	Detection detection{ Detection::Category::SUSPICIOUS, "name", expected_result };
	EXPECT_EQ(detection.getMemberName().value(), expected_result);
}