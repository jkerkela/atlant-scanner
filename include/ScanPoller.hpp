#ifndef SCANPOLLER_HPP
#define SCANPOLLER_HPP

#include "FileScanner.hpp"
#include "ScanMetadata.hpp"
#include "ScanResult.hpp"

#include <string>
#include <fstream>
#include <optional>

class ScanPoller {

public:
	ScanPoller(FileScanner& file_scanner) : file_scanner{ file_scanner } {};
	ScanResult scan(ScanMetadata &metadata, std::string &path);

private:
	FileScanner& file_scanner;

	ScanResult scan(ScanMetadata &metadata, std::ifstream file_stream);
	ScanResult getResultByPoll(const std::optional<int>& retry_after, const std::optional<std::string>& poll_URL);
};

#endif // SCANPOLLER_HPP
