#ifndef SCANSETTINGS_HPP
#define SCANSETTINGS_HPP

class ScanSettings {

public:
	ScanSettings(
		const bool do_scan_archives = false,
		const int max_nested = 0,
		const bool do_stop_on_first = false,
		const bool allow_upstream_metadata = false
		//TODO: Optional<SecurityCloudSettings> securityCloudSettings
	);

	bool getScanArchives() { return do_scan_archives };
	int getMaxNested() { return max_nested };
	bool getStopOnFirst() { return do_stop_on_first };
	bool getAllowUpstreamMetadata() { return allow_upstream_metadata };

private:
	bool do_scan_archives;
	int max_nested;
	bool do_stop_on_first;
	bool allow_upstream_metadata;
};
#endif // SCANSETTINGS_HPP
