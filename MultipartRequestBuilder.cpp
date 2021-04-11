#include "MultipartRequestBuilder.hpp"

#include <random>
#include <regex>
#include <stdexcept>

#include <Poco/UTF8Encoding.h>

namespace {
	constexpr int BOUNDARY_BYTES = 32;
}

MultiPartRequestBuilder::MultiPartRequestBuilder()
{
	parts = std::vector<Part>{};
	boundary = makeBoundary();
}

void MultiPartRequestBuilder::addPart(const Part &part)
{
	//TODO: ok to not return MultiPartRequestBuilder object?
	parts.emplace_back(part);
}

std::string encodeToUTF8(std::string to_encode)
{
	auto val_to_encode = reinterpret_cast<unsigned char*>(const_cast<char*>(to_encode.c_str()));
	auto encoder = Poco::UTF8Encoding();
	auto malformed_encoding_result = -1;
	auto val_encode_result = encoder.convert(val_to_encode);
	if (val_encode_result == malformed_encoding_result) {
		throw std::invalid_argument("Unsupported encoding to UTF-8");
	}
	return std::string(reinterpret_cast<char const*>(val_to_encode));
}
std::string MultiPartRequestBuilder::encode()
{
	std::string result;
	auto initiator = encodeToUTF8(std::string("--" + boundary));
	auto separator = encodeToUTF8(std::string("\r\n--" + boundary + "\r\n"));
	auto terminator = encodeToUTF8(std::string("\r\n--" + boundary + "--\r\n"));

	result += initiator;
	for (int i = 0; i < parts.size; i ++) {
		result += parts.at(i).encode();
		if (i != (parts.size - 1)) {
			result += separator
		}
	}
	return result + terminator;
}


std::string MultiPartRequestBuilder::makeBoundary()
{
	using random_bytes_engine = std::independent_bits_engine<
		std::default_random_engine, CHAR_BIT, unsigned char>;
	random_bytes_engine rbe;
	std::vector<unsigned char> data(BOUNDARY_BYTES);
	std::generate(begin(data), end(data), std::ref(rbe));

	std::string result = "Boundary";
	
	//TODO: do we get correct boundary here?
	for (const item : data) {
		result += item;
	}
	return result;
}

MultiPartRequestBuilder::Part::Part(const std::string& name, const std::string& content_type, const std::string& content)
	: name { name }, content_type { content_type }, content { content }
{}

std::string MultiPartRequestBuilder::Part::encode()
{
	auto name_with_escaped_chars = std::regex_replace(name, std::regex("\""), "\\\"");
	auto content_disposition = std::string("Content-Disposition: form-data; name=\"" + name_with_escaped_chars + "\r\n");
	auto content_t = std::string("Content - Type: " + content_type + "\r\n\r\n");

	auto encoded_content_disposition = encodeToUTF8(content_disposition);
	auto encoded_content_type = encodeToUTF8(content_t);
	return encoded_content_disposition + encoded_content_type + content;
}

