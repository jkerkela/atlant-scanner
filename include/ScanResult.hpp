#ifndef SCANRESULT_HPP
#define SCANRESULT_HPP

#include "Detection.hpp"

#include <list>
#include <optional>

class ScanResult
{

public:
	enum Status {
		COMPLETE,
		PENDING
	};

	enum Result {
		CLEAN,
		WHITELISTED,
		SUSPICIOUS,
		PUA,
		UA,
		HARMFUL
	};

	ScanResult() = default;
	ScanResult(const ScanResult::Status &status,
		const ScanResult::Result &result,
		const std::list<Detection> &detections,
		const std::optional<int> retry_after = std::nullopt,
		const std::optional<std::string> poll_URL = std::nullopt)
		: status{ status }, result{ result }, detections{ detections }, retry_after{ retry_after }, poll_URL{ poll_URL } {};

	Status getStatus() { return status; };
	Result getResult() { return result; };
	std::list<Detection> getDetections() { return detections; };
	std::optional<std::string> getPollURL() { return poll_URL; };
	std::optional<int> getRetryAfter() { return retry_after; };
	void setPollURL(const std::string& poll_url) { poll_URL = poll_url; };
	void setRetryAfter(const int time) { retry_after = time; };

private:
	Status status;
	Result result;
	std::list<Detection> detections;
	std::optional<int> retry_after;
	std::optional<std::string> poll_URL;
};

#endif // SCANRESULT_HPP
