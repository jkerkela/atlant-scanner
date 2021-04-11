#ifndef SCANSETTINGS_HPP
#define SCANSETTINGS_HPP

#include<optional>
#include <Poco/JSON/Object.h>

class ScanSettings {

public:
	ScanSettings(
		const std::optional<bool> do_scan_archives = std::nullopt,
		const std::optional<int> max_nested = std::nullopt,
		const std::optional<bool> do_stop_on_first = std::nullopt,
		const std::optional<bool> allow_upstream_metadata = std::nullopt
		//TODO: Optional<SecurityCloudSettings> securityCloudSettings
	);

	std::optional<bool> getScanArchives() { return do_scan_archives };
	std::optional<int> getMaxNested() { return max_nested };
	std::optional<bool> getStopOnFirst() { return do_stop_on_first };
	std::optional<bool> getAllowUpstreamMetadata() { return allow_upstream_metadata };
	Poco::JSON::Object::Ptr to_json_object();


private:
	std::optional<bool> do_scan_archives;
	std::optional<int> max_nested;
	std::optional<bool> do_stop_on_first;
	std::optional<bool> allow_upstream_metadata;
};
#endif // SCANSETTINGS_HPP
