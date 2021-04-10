
#include "ScanResult.hpp"

ScanResult::ScanResult(const ScanResult::Status &status, const ScanResult::Result &result, const std::list<Detection> &detections)
	: status { status }, result { result }, detections { detections }
{};

ScanResult::Status ScanResult::getStatus()
{
	return status;
}

ScanResult::Result ScanResult::getResult()
{
	return result;
}

std::list<Detection> ScanResult::getDetections()
{
	return detections;
}
