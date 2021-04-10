#include "ContentMetadata.hpp"

ContentMetadata::ContentMetadata(
	const std::string& SHA1,
	const std::string& URI,
	const int content_length,
	const std::string & content_type,
	const std::string & charset
) : SHA1{ SHA1 }, URI{ URI }, content_length{ content_length }, content_type{ content_type }, charset{ charset }
{}