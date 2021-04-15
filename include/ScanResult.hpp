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
		const std::optional<std::string> poll_URL = std::nullopt);
	Status getStatus();
	Result getResult();
	std::list<Detection> getDetections();
	std::optional<std::string> getPollURL();
	std::optional<int> getRetryAfter();
	void setPollURL(const std::string& poll_url);
	void setRetryAfter(const int time);

private:
	Status status;
	Result result;
	std::list<Detection> detections;
	std::optional<int> retry_after;
	std::optional<std::string> poll_URL;
};

#endif // SCANRESULT_HPP
