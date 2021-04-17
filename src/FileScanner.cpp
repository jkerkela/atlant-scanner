
#include "FileScanner.hpp"


#include "APIException.hpp"
#include "MultipartRequestBuilder.hpp"

#include <fstream>
#include <streambuf>
#include <exception>

using Poco::Net::HTTPResponse;

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

Detection FileScanner::buildDetection(Poco::JSON::Array::ConstIterator it) 
{
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
		throw APIException("Invalid detection category");
	}

	auto name = object->getValue<std::string>("name");
	auto member = object->getValue<std::string>("member_name");
	return Detection(category, name, member);
}

ScanResult FileScanner::deserializeScanResponse(std::istream& response)
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
		throw APIException("Invalid scan status");
	}

	auto scan_res = object->getValue<std::string>("scan_result");
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
	auto array = object->getArray("detections");
	for (Poco::JSON::Array::ConstIterator it = array->begin(); it != array->end(); ++it)
	{
		auto detection = buildDetection(it);
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
