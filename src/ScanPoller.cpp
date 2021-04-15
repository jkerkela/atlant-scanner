#include "ScanPoller.hpp"

#include <chrono>
#include <thread>
#include <memory>

ScanPoller::ScanPoller(FileScanner &file_scanner) : file_scanner{ file_scanner }
{}

ScanResult ScanPoller::scan(ScanMetadata &metadata, std::string &path)
{
	return scan(metadata, std::ifstream(path));
}

ScanResult ScanPoller::getResultByPoll(const std::optional<int> &retry_after, const std::optional<std::string> &poll_URL)
{
	std::this_thread::sleep_for(std::chrono::seconds(retry_after.value()));
	return file_scanner.poll(std::make_unique<HTTPClientSessionImpl>(file_scanner.getAddress().getHost()), poll_URL.value());
}

ScanResult ScanPoller::scan(ScanMetadata &metadata, std::ifstream file_stream)
{
	ScanResult result = file_scanner.scan(metadata, 
		file_stream,
		std::make_unique<HTTPClientSessionImpl>(file_scanner.getAddress().getHost())
	);
	auto status = result.getStatus();
	switch (status) {
	case ScanResult::Status::COMPLETE:
		return result;
	case ScanResult::Status::PENDING:
		auto retry_time = result.getRetryAfter();
		auto poll_URL = result.getPollURL();
		while (status == ScanResult::Status::PENDING) {
			auto result = getResultByPoll(retry_time, poll_URL);
			status = result.getStatus();
		}
		return result;
	}
	throw new std::runtime_error("Unknown scan status");
}