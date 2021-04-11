#include "ScanPoller.hpp"

ScanPoller::ScanPoller(FileScanner &file_scanner) : file_scanner{ file_scanner }
{}

ScanResult ScanPoller::scan(ScanMetadata &metadata, std::string &path)
{
	return scan(metadata, std::ifstream(path));
}

ScanResult getResultByPolling(const std::optional<int> retry_after)
{
	//TODO: temporary return
	return ScanResult{};
}

ScanResult ScanPoller::scan(ScanMetadata &metadata, std::ifstream file_stream)
{
	ScanResult result = file_scanner.scan(metadata, file_stream);
	switch (result.getStatus()) {
	case ScanResult::Status::COMPLETE:
		return result;
	case ScanResult::Status::PENDING:
		auto poll_URL = result.getPollURL();
		//TODO: continue here, implement as:
		//https://github.com/F-Secure/atlant-api/blob/master/java/scanner/src/main/com/fsecure/atlant/examples/scanner/ScanPoller.java
		return getResultByPolling(result.getRetryAfter());
	}
	throw new std::runtime_error("Unknown scan status");
}