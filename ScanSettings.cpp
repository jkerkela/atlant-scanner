#include "ScanSettings.hpp"

ScanSettings::ScanSettings(const bool do_scan_archives,
	const int max_nested,
	const bool do_stop_on_first,
	const bool allow_upstream_metadata
) : do_scan_archives{ do_scan_archives },
	max_nested{ max_nested },
	do_stop_on_first{ do_stop_on_first },
	allow_upstream_metadata{ allow_upstream_metadata }
{}
