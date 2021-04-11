#include "ScanPoller.hpp"

ScanPoller::ScanPoller(FileScanner &file_scanner) : file_scanner{ file_scanner }
{}

std::optional<ScanResult> ScanPoller::scan(ScanMetadata &metadata, std::string &path)
{
	return scan(metadata, std::ifstream(path));
}

std::optional<ScanResult> getResultByPolling(const int retry_after)
{
}
std::optional<ScanResult> ScanPoller::scan(ScanMetadata &metadata, std::ifstream file_stream)
{
	std::optional<ScanResult> result = file_scanner.scan(metadata, file_stream);
	if (result.has_value) {
		switch (result.value.getStatus()) {
		case ScanResult::Status::COMPLETE:
			return result.value;
		case ScanResult::Status::PENDING:
			auto poll_URL = result.value.getPollURL();
			//TODO: continue here, implement as:
			//https://github.com/F-Secure/atlant-api/blob/master/java/scanner/src/main/com/fsecure/atlant/examples/scanner/ScanPoller.java
			return getResultByPolling(result.value.getRetryAfter());
		}
	}
	throw new std::runtime_error("Unknown scan status");
}