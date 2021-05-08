
#include "FileScanner.hpp"

#include "APIException.hpp"
#include "MultipartRequestBuilder.hpp"
#include "JSONHandlerImpl.hpp"

#include <fstream>
#include <streambuf>
#include <exception>

namespace {
	constexpr auto API_SCAN_POSTFIX = "/api/scan/v1";
	constexpr auto API_POLL_POSTFIX = "/api/poll/v1";
}

FileScanner::FileScanner(const std::string& scanning_address, Authenticator &authenticator)
	: base_URI { Poco::URI(scanning_address)},
	scan_endpoint{ Poco::URI(scanning_address + API_SCAN_POSTFIX) },
	poll_endpoint{ Poco::URI(scanning_address + API_POLL_POSTFIX) },
	authenticator{ authenticator }
{
	refreshToken();
};

void FileScanner::refreshToken()
{
	auth_token = authenticator.fetchToken();
}

ScanResult FileScanner::scan(ScanMetadata &metadata, std::ifstream& input, std::unique_ptr<IHTTPClientSession> client)
{
	HTTPRequestImpl scan_request = buildScanRequest(metadata, input);
	client->sendRequest(scan_request);

	return processScanResponse(std::move(client));
}

ScanResult FileScanner::poll(std::unique_ptr<IHTTPClientSession> client, const std::string &poll_URI)
{
	auto poll_req = buildPollRequest(poll_URI);
	client->sendRequest(poll_req);
	return processPollResponse(std::move(client));
}

HTTPRequestImpl FileScanner::buildScanRequest(ScanMetadata &metadata, std::ifstream& input)
{
	auto metadata_body = serializeScanMetadata(metadata);
	MultiPartRequestBuilder multipart_request_builder{};
	multipart_request_builder.addPart(MultiPartRequestBuilder::Part{"metadata", "application/json", metadata_body});
	std::string input_str((std::istreambuf_iterator<char>(input)),
		std::istreambuf_iterator<char>());
	multipart_request_builder.addPart(MultiPartRequestBuilder::Part{"data", "application/octet-stream", input_str});
	auto request_body = multipart_request_builder.encode();

	HTTPRequestImpl req(HTTPRequest::HTTP_POST, scan_endpoint.getPathAndQuery());
	req.setContentType("multipart/form-data; boundary=" + multipart_request_builder.getBoundary());
	req.set("Authorization", std::string("Bearer " + auth_token.getToken()));
	req.setContentLength(request_body.length());
	req.setBody(request_body);
	return req;
}

HTTPRequestImpl FileScanner::buildPollRequest(const std::string &poll_URL)
{
	HTTPRequestImpl req(HTTPRequest::HTTP_GET, poll_URL);
	req.set("Authorization", std::string("Bearer " + auth_token.getToken()));
	return req;
}

ScanResult FileScanner::processScanResponse(std::unique_ptr<IHTTPClientSession> client)
{
	ScanResult scan_result{};
	HTTPResponseImpl res;
	std::istream& res_stream = client->receiveResponse(res);
	auto status_code = res.getStatus();
	if ((status_code == HTTPResponse::HTTPStatus::HTTP_OK) || (status_code == HTTPResponse::HTTPStatus::HTTP_PROCESSING)) {
		try {
			scan_result = deserializeScanResponse(res_stream);
		}
		catch (const std::exception &e) {
			throw APIException("Invalid scan response:" + std::string(e.what()));
		}
	}
	else {
		throw APIException("Unexpected response from scanning service");
	}

	if (status_code == HTTPResponse::HTTPStatus::HTTP_PROCESSING) {
		if (!res.has("Location")) {
			throw APIException("Missing poll URL");
		}
		auto location = res.get("Location");

		scan_result.setPollURL(location);

		if (!res.has("Retry-After")) {
			throw APIException("Missing retry after duration");
		}

		auto retry = res.get("Retry-After");
		try {
			scan_result.setRetryAfter(std::stoi(retry));
		}
		catch (const std::invalid_argument &e) {
			throw APIException("Invalid retry after duration: " + std::string(e.what()));
		}
	}

	return scan_result;
}

ScanResult FileScanner::processPollResponse(std::unique_ptr<IHTTPClientSession> client) {
	ScanResult scan_result{};
	HTTPResponseImpl res;
	std::istream& res_stream = client->receiveResponse(res);
	auto status_code = res.getStatus();

	if (status_code == HTTPResponse::HTTPStatus::HTTP_OK) {
		try {
			scan_result = deserializeScanResponse(res_stream);
		}
		catch (const std::exception& e) {
			throw APIException("Invalid scan response:" + std::string(e.what()));
		}
	}
	else {
		throw APIException("Unexpected response from scanning service");
	}

	if (scan_result.getStatus() == ScanResult::Status::PENDING) {

		if (!res.has("Retry-After")) {
			throw APIException("Missing retry after duration");
		}
		auto retry = res.get("Retry-After");
		try {
			scan_result.setRetryAfter(std::stoi(retry));
		}
		catch (const std::invalid_argument& e) {
			throw APIException("Invalid retry after duration: " + std::string(e.what()));
		}
	}
	return scan_result;
}

Detection FileScanner::buildDetection(const std::string& detection_item_json)
{
	JSONHandlerImpl detection_json{ detection_item_json };
	auto detection_category = detection_json.getStringValue("category");
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
		throw APIException("Invalid detection category");
	}

	auto name = detection_json.getStringValue("name");
	auto member = detection_json.getStringValue("member_name");
	return Detection(category, name, member);
}

ScanResult FileScanner::deserializeScanResponse(std::istream& response)
{
	JSONHandlerImpl top_level_handler{ response };
	auto scan_status = top_level_handler.getStringValue("status");

	ScanResult::Status status;
	if (scan_status == "complete") {
		status = ScanResult::Status::COMPLETE;
	}
	else if (scan_status == "pending") {
		status = ScanResult::Status::PENDING;
	}
	else {
		throw APIException("Invalid scan status");
	}

	auto scan_res = top_level_handler.getStringValue("scan_result");
	ScanResult::Result scan_result;
	if (scan_res == "clean") {
		scan_result = ScanResult::Result::CLEAN;
	}
	else if (scan_res == "whitelisted") {
		scan_result = ScanResult::Result::WHITELISTED;
	}
	else if (scan_res == "suspicious") {
		scan_result = ScanResult::Result::SUSPICIOUS;
	}
	else if (scan_res == "PUA") {
		scan_result = ScanResult::Result::PUA;
	}
	else if (scan_res == "UA") {
		scan_result = ScanResult::Result::UA;
	}
	else if (scan_res == "harmful") {
		scan_result = ScanResult::Result::HARMFUL;
	}
	else {
		throw APIException("Invalid scan result category");
	}

	std::list<Detection> detections{};
	auto detections_array = top_level_handler.getArray("detections");
	for (auto const detection_item : detections_array) {
		auto detection = buildDetection(detection_item);
		detections.emplace_back(detection);
	}

	return ScanResult(status, scan_result, detections);
}

std::string FileScanner::serializeScanMetadata(ScanMetadata &metadata) {
	auto json_obj = metadata.toJsonObject();
	std::ostringstream oss;
	Poco::JSON::Stringifier::stringify(json_obj, oss);
	return oss.str();
}
