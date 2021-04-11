
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

std::optional<std::string> ScanResult::getPollURL() 
{
	return poll_URL;
}

void ScanResult::setPollURL(const std::string& poll_url)
{
	poll_URL = poll_url;
}

std::optional<int> ScanResult::getRetryAfter() {
	return retry_after;
}

void ScanResult::setRetryAfter(const int time) {
	retry_after = time;
}
