#ifndef SCANRESULT_HPP
#define SCANRESULT_HPP

#include<list>

#include "Detection.hpp"

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

	ScanResult(const ScanResult::Status& status, const ScanResult::Result& result, const std::list<Detection>& detections);
	Status getStatus();
	Result getResult();
	std::list<Detection> getDetections();

private:
	const Status& status;
	const Result& result;
	const std::list<Detection>& detections;
	//Optional<Duration> retryAfter;
	//Optional<String> pollURL;
};

#endif // SCANRESULT_HPP
