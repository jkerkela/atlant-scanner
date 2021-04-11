#include "ScanSettings.hpp"

ScanSettings::ScanSettings(
	const std::optional<bool> do_scan_archives,
	const std::optional<int> max_nested,
	const std::optional<bool> do_stop_on_first,
	const std::optional<bool> allow_upstream_metadata
) : do_scan_archives{ do_scan_archives },
	max_nested{ max_nested },
	do_stop_on_first{ do_stop_on_first },
	allow_upstream_metadata{ allow_upstream_metadata }
{}

Poco::JSON::Object::Ptr ScanSettings::to_json_object()
{
	Poco::JSON::Object::Ptr json_object = new Poco::JSON::Object();

	if (do_scan_archives != std::nullopt) {
		json_object->set("scan_settings", do_scan_archives.value());
	}

	if (max_nested != std::nullopt) {
		json_object->set("max_nested", max_nested.value());
	}

	if (do_stop_on_first != std::nullopt) {
		json_object->set("stop_on_first", do_stop_on_first.value());
	}

	if (allow_upstream_metadata != std::nullopt) {
		json_object->set("allow_upstream_metadata", allow_upstream_metadata.value());
	}
	return json_object;
}