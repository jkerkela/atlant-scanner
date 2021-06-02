#include "ResultPrinter.hpp"

#include <iostream>
#include <vector>

#include "APIException.hpp"

void ResultPrinter::print(const ScanResult& scan_result)
{
	std::cout << "Result: " + resultToString(scan_result.getResult()) << std::endl;
	std::vector<Detection> detections = scan_result.getDetections();
	if (!detections.empty()) {
		std::cout << "Detections: " << std::endl;
		for (auto detection : detections) {
			std::string detection_item = "Name: " + detection.getName()
				+ " Category: " + categoryToString(detection.getCategory());
			if (detection.getMemberName().has_value()) {
				detection_item + std::string(" Member name: ") + detection.getMemberName().value();
			}
			std::cout << detection_item << std::endl;
		}
	}
}

std::string ResultPrinter::resultToString(const ScanResult::Result& result) {
	switch (result) {
	case ScanResult::Result::CLEAN:
		return "clean";
	case ScanResult::Result::WHITELISTED:
		return "whitelisted";
	case ScanResult::Result::SUSPICIOUS:
		return "suspicious";
	case ScanResult::Result::PUA:
		return "PUA";
	case ScanResult::Result::UA:
		return "UA";
	case ScanResult::Result::HARMFUL:
		return "harmful";
	default:
		throw APIException("Invalid scan result");
	}
}

std::string ResultPrinter::categoryToString(const Detection::Category& category) {
	switch (category) {
	case Detection::Category::SUSPICIOUS:
		return "suspicious";
	case Detection::Category::PUA:
		return "PUA";
	case Detection::Category::UA:
		return "UA";
	case Detection::Category::HARMFUL:
		return "harmful";
	default:
		throw APIException("Invalid detection category");
	}
}