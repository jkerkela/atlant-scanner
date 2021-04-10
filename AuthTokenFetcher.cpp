
#include "AuthTokenFetcher.hpp"

#include "APIException.hpp"
#include "Detection.hpp"

#include <stdexcept>
#include <unordered_map>

#include <Poco/Net/HTTPResponse.h>
#include <Poco/UTF8Encoding.h>
#include <Poco/JSON/Parser.h>


namespace {
	constexpr auto AUDIENCE = "f-secure-atlant";
	constexpr auto API_PREFIX = "/api/poll/v1";
	constexpr auto JSON_GRANT_TYPE_KEY = "grant_type";
	constexpr auto JSON_GRANT_TYPE_VALUE = "grant_type";
	constexpr auto JSON_CLIENT_ID_KEY = "client_id";
	constexpr auto JSON_CLIENT_SECRET_KEY = "client_secret";
	constexpr auto JSON_AUDIENCE_KEY = "client_secret";
	constexpr auto JSON_SCOPE_KEY = "scope";
}

AuthTokenFetcher::AuthTokenFetcher(const std::string &authorization_address)
	: client{ HTTPClientSession() }, token_endpoint{ std::string(API_PREFIX + authorization_address) }
{}

AuthToken AuthTokenFetcher::fetch(const std::string &URI,
	const std::string &client_ID,
	const std::string &client_secret,
	const std::set<std::string> &scopes)
{
	HTTPRequest tokenRequest = buildTokenRequest(client_ID, client_secret, scopes);
	HTTPClientSession session(URI);
	std::ostream& req_stream = session.sendRequest(tokenRequest);
	req_stream << HTTP_request_body;

	HTTPResponse res;
	std::istream& res_stream = session.receiveResponse(res);
    return deserializeTokenResponse(res_stream);
}

HTTPRequest AuthTokenFetcher::buildTokenRequest(const std::string &client_ID, const std::string &client_secret, const std::set<std::string> &scopes) 
{
	std::unordered_map<std::string, std::string> params;
	params.insert(JSON_GRANT_TYPE_KEY, JSON_GRANT_TYPE_VALUE);
	params.insert(std::string(JSON_CLIENT_ID_KEY), client_ID);
	params.insert(std::string(JSON_CLIENT_SECRET_KEY), client_secret);
	params.insert(JSON_AUDIENCE_KEY, AUDIENCE);

	if (!scopes.empty()) {
		params.insert(std::string(JSON_SCOPE_KEY), encodeScopes(scopes));
	}

	HTTP_request_body =
		encodeQueryParameters(params);

	//TODO: check that this HTTP request is valid
	HTTPRequest req(HTTPRequest::HTTP_POST, token_endpoint, HTTPMessage::HTTP_1_1);
	req.setContentType("application/x-www-form-urlencoded");
	req.setContentLength(HTTP_request_body.length());
	return req;
}

std::string AuthTokenFetcher::encodeScopes(const std::set<std::string>& scopes) 
{
	std::string result;
	std::string separator = " ";
	for (auto const &s : scopes) {
		result += s + separator;
	}
	result.pop_back(); // remove trailing separator
	return result;
}

std::string AuthTokenFetcher::encodeQueryParameters(const std::unordered_map<std::string, std::string> &params)
{

	std::string result;
	for (auto const& entry : params) {
		auto key_to_encode = reinterpret_cast<unsigned char*>(const_cast<char*>(entry.first.c_str()));
		auto val_to_encode = reinterpret_cast<unsigned char*>(const_cast<char*>(entry.second.c_str()));
		auto encoder = Poco::UTF8Encoding();
		auto malformed_encoding_result = -1;
		auto key_encode_result = encoder.convert(key_to_encode);
		auto val_encode_result = encoder.convert(val_to_encode);
		if ((key_encode_result == malformed_encoding_result) || (val_encode_result == malformed_encoding_result)) {
			throw std::invalid_argument("Unsupported encoding to UTF-8");
		}
		std::string encoded_key(reinterpret_cast<char const*>(key_to_encode));
		std::string encoded_val(reinterpret_cast<char const*>(val_to_encode));
		result += std::string(encoded_key + "=" + encoded_val + "&");
	}
	return result;
}

AuthToken AuthTokenFetcher::deserializeTokenResponse(std::istream& response)
{
	oco::JSON::Parser parser;
	auto result = parser.parse(response);
	auto object = result.extract<Poco::JSON::Object::Ptr>();
	auto token = object->getValue<std::string>("access_token");
	auto expires_in = object->getValue<std::string>("expires_in");
	return AuthToken(token, expires_in);
}

ScanResult AuthTokenFetcher::deserializeScanResponse(std::istream& response) 
{

	Poco::JSON::Parser parser;
	auto result = parser.parse(response);
	auto object = result.extract<Poco::JSON::Object::Ptr>();
	auto scan_status = object->getValue<std::string>("status");
	ScanResult::Status status;
	if (scan_status == "complete") {
		status = ScanResult::Status::COMPLETE;
	}
	else if (scan_status == "pending") {
		status = ScanResult::Status::PENDING;
	} 
	else {
		throw new APIException("Invalid scan status");
	}

	auto scan_result = object->getValue<std::string>("scan_result");
	ScanResult::Result result;
	if (scan_result == "clean") {
		result = ScanResult::Result::CLEAN;
	}
	else if (scan_result == "whitelisted") {
		result = ScanResult::Result::WHITELISTED;
	}
	else if (scan_result == "suspicious") {
		result = ScanResult::Result::SUSPICIOUS;
	}
	else if (scan_result == "PUA") {
		result = ScanResult::Result::PUA;
	}
	else if (scan_result == "UA") {
		result = ScanResult::Result::UA;
	}
	else if (scan_result == "harmful") {
		result = ScanResult::Result::HARMFUL;
	}
	else {
		throw new APIException("Invalid detection category");
	}

	std::list<Detection> detections{};
	auto array = object->getArray("detections");
	for (Poco::JSON::Array::ConstIterator it = array->begin(); it != array->end(); ++it)
	{
		auto detection = buildDetection(it);
		detections.emplace_back(detection);

	}

	return ScanResult(status, result, detections);
}

Detection buildDetection(Poco::JSON::Array::ConstIterator it) {
	auto object = it->extract<Poco::JSON::Object::Ptr>();
	auto detection_category = object->getValue<std::string>("category");
	Detection::Category category;

	if (detection_category == "suspicious") {
		category = Detection::Category::SUSPICIOUS;
	}
	else if (detection_category == "PUA") {
		category = Detection::Category::PUA;
	}
	else if (detection_category == "UA") {
		category = Detection::Category::UA;
	}
	else if (detection_category == "harmful") {
		category = Detection::Category::HARMFUL;
	}
	else {
		throw new APIException("Invalid detection category");
	}

	auto name = object->getValue<std::string>("name");
	auto member = object->getValue<std::string>("member_name");
	return Detection(category, name, member);
}
