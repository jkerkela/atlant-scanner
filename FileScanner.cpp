
#include "FileScanner.hpp"
#include "MultipartRequestBuilder.hpp"
#include "APIException.hpp"

#include <fstream>
#include <streambuf>
#include <exception>

#include <Poco/Net/HTTPResponse.h>
#include <Poco/JSON/Parser.h>

using Poco::Net::HTTPResponse;

namespace {
	constexpr auto API_PREFIX = "/api/poll/v1";
}

FileScanner::FileScanner(const Poco::URI &scanning_address, Authenticator &authenticator)
	: base_URI{ scanning_address },
	scan_endpoint{ Poco::URI(scanning_address + API_PREFIX) },
	poll_endpoint{ Poco::URI(scanning_address + API_PREFIX) },
	authenticator{ authenticator }
{
	refreshToken();
};

void FileScanner::refreshToken()
{
	auth_token = authenticator.fetchToken();
}

std::optional<ScanResult> FileScanner::scan(ScanMetadata &metadata, std::ifstream& input)
{
	HTTPRequest scan_request = buildScanRequest(metadata, input);

	HTTPClientSession client{ scan_endpoint.getHost() };
	std::ostream& req_stream = client.sendRequest(scan_request);
	req_stream << HTTP_reques_body;

	return processScanResponse(client);
}

HTTPRequest FileScanner::buildScanRequest(ScanMetadata &metadata, std::ifstream& input)
{
	auto metadata_body = serializeScanMetadata(metadata);
	MultiPartRequestBuilder multipart_request_builder{};
	multipart_request_builder.addPart(MultiPartRequestBuilder::Part{"metadata", "application/json", metadata_body});
	std::string input_str((std::istreambuf_iterator<char>(input)),
		std::istreambuf_iterator<char>());
	multipart_request_builder.addPart(MultiPartRequestBuilder::Part{"data", "application/octet-stream", input_str});
	HTTP_reques_body = multipart_request_builder.encode();

	//TODO: is this correct http req
	std::string path(scan_endpoint.getPathAndQuery());
	HTTPRequest req(HTTPRequest::HTTP_POST, path, HTTPMessage::HTTP_1_1);
	req.setContentType("multipart/form-data; boundary=" + multipart_request_builder.getBoundary());
	req.set("Authorization", std::string("Bearer " + auth_token.getToken()));
	req.setContentLength(HTTP_reques_body.length());
	return req;
}

std::optional<ScanResult> FileScanner::processScanResponse(HTTPClientSession &client)
{
	std::optional<ScanResult> scan_result = std::nullopt;
	HTTPResponse res;
	std::istream& res_stream = client.receiveResponse(res);
	auto status_code = res.getStatus();
	if ((status_code == HTTPResponse::HTTPStatus::HTTP_OK) || (status_code == HTTPResponse::HTTPStatus::HTTP_PROCESSING)) {
		try {
			scan_result.value = deserializeScanResponse(res_stream);
		}
		catch (const std::exception &e) {
			throw new APIException("Invalid scan response:" + std::string(e.what()));
		}
	}
	else {
		throw new APIException("Unexpected response from scanning service");
	}

	if (status_code == HTTPResponse::HTTPStatus::HTTP_PROCESSING) {
		if (!res.has("Location")) {
			throw new APIException("Missing poll URL");
		}
		auto location = res.get("Location");

		scan_result.value.setPollURL(location);

		if (!res.has("Retry-After")) {
			throw new APIException("Missing retry after duration");
		}

		auto retry = res.get("Retry-After");
		try {
			scan_result.value.setRetryAfter(std::stoi(retry));
		}
		catch (const std::invalid_argument &e) {
			throw new APIException("Invalid retry after duration: " + std::string(e.what()));
		}
	}

	return scan_result;
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

std::optional<ScanResult> FileScanner::deserializeScanResponse(std::istream& response)
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


std::string FileScanner::serializeScanMetadata(ScanMetadata &metadata) {
	auto json_obj = metadata.to_json_object();
	std::ostringstream oss;
	Poco::JSON::Stringifier::stringify(json_obj, oss);
	return oss.str();
}
