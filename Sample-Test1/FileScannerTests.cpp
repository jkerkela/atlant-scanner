#include "pch.h"
#include "FileScanner.hpp"
#include "MockHTTPCommunication.hpp"
#include "APIException.hpp"

#include <Poco/Net/NetException.h>

#include <string>
#include <set>
#include <memory>
#include <vector>

using ::testing::_;
using ::testing::AnyNumber;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrEq;

TEST(FileScanner, TestConstructorWithNonAccessibleAuthAddress) {
	Authenticator authenticator{ 
		std::make_unique<AuthTokenFetcher>("auth_address", std::make_unique<HTTPClientSessionImpl>("auth_address")), 
		"client_ID", 
		"client_secret", 
		std::set<std::string>{ "scope1" }
	};
	EXPECT_THROW(FileScanner(std::make_unique<HTTPClientSessionImpl>("scan_address"), "scan_address", authenticator), NetException);
}

class TestFileScannerWithMockedAuthResponse : public ::testing::Test
{
protected:

	FileScanner* file_scanner = nullptr;
	std::string test_scan_address = "scan_uri";

	std::unique_ptr<MockHTTPClientSessionImpl> mock_client_session_for_auth;
	std::unique_ptr<MockHTTPClientSessionImpl> mock_client_session_for_scan;
	std::unique_ptr<HTTPResponseImpl> mock_http_response;

	std::ostringstream auth_response_buf;
	std::stringstream auth_response_stream;
	std::string auth_response_content{ "{ \"access_token\": \"Token1\", \"token_type\" : \"bearer\", \"expires_in\" : 3600 }" };

	std::ostringstream scan_response_buf;
	std::stringstream scan_response_stream;
	inline static std::string scan_response_content{ "{ \"status\": \"complete\", \"scan_result\" : \"suspicious\","
			"\"detections\" : [ { \"name\" : \"detection1\", \"category\" : \"suspicious\", \"member_name\" : \"member1\" } ] }" };

	void mock_auth_HTTP_response()
	{
		//PRE: Mock client session, sendRequest, receiveResponse for auth token requests
		mock_client_session_for_auth = std::make_unique<MockHTTPClientSessionImpl>();
		EXPECT_CALL(*mock_client_session_for_auth, sendRequest(_)).Times(1);
		auth_response_stream << auth_response_content;
		EXPECT_CALL(*mock_client_session_for_auth, receiveResponse()).Times(1).WillOnce(ReturnRef(auth_response_stream));

		//PRE: Create Authenticator and FileScanner objects
		Authenticator authenticator{ std::make_unique<AuthTokenFetcher>("auth_address", std::move(mock_client_session_for_auth)),
			"client_ID",
			"client_secret",
			std::set<std::string>{ "scope1" }
		};
		file_scanner = new FileScanner(std::move(mock_client_session_for_scan), test_scan_address, authenticator);
	}

	void mock_scan_HTTP_response(const HTTPResponse::HTTPStatus http_status = HTTPResponse::HTTPStatus::HTTP_OK,
		const std::string& scan_resp = scan_response_content,
		const std::string& expected_http_retry_after = "",
		const std::string& expected_http_location = "",
		const ScanResult::Status  scan_status = ScanResult::Status::COMPLETE
	)
	{
		//PRE: Mock client session, sendRequest, receiveResponse for auth token requests
		mock_client_session_for_auth = std::make_unique<MockHTTPClientSessionImpl>();
		EXPECT_CALL(*mock_client_session_for_auth, sendRequest(_)).Times(1);
		auth_response_stream << auth_response_content;
		EXPECT_CALL(*mock_client_session_for_auth, receiveResponse()).Times(1).WillOnce(ReturnRef(auth_response_stream));

		//PRE: Create Authenticator and FileScanner objects
		Authenticator authenticator{ std::make_unique<AuthTokenFetcher>("auth_address", std::move(mock_client_session_for_auth)),
			"client_ID",
			"client_secret",
			std::set<std::string>{ "scope1" }
		};

		//PRE: Mock client session, sendRequest, receiveResponse for scan requests
		mock_client_session_for_scan = std::make_unique<MockHTTPClientSessionImpl>();
		EXPECT_CALL(*mock_client_session_for_scan, sendRequest(_)).Times(1);
		mock_http_response = std::make_unique<HTTPResponseImpl>();
		scan_response_stream << scan_resp;
		EXPECT_CALL(*mock_client_session_for_scan, receiveResponse()).Times(1).WillOnce(ReturnRef(scan_response_stream));
		EXPECT_CALL(*mock_client_session_for_scan, getResponseStatus()).Times(1).WillOnce(Return(http_status));
		if (expected_http_retry_after != "") {
			EXPECT_CALL(*mock_client_session_for_scan, responseContains(StrEq("Retry-After"))).Times(AnyNumber()).WillRepeatedly(Return(true));
			EXPECT_CALL(*mock_client_session_for_scan, getFromResponse(StrEq("Retry-After"))).Times(AnyNumber()).WillRepeatedly(Return(expected_http_retry_after));
		} 
		else {
			EXPECT_CALL(*mock_client_session_for_scan, responseContains(StrEq("Retry-After"))).Times(AnyNumber()).WillRepeatedly(Return(false));
		}
		if (expected_http_location != "") {
			EXPECT_CALL(*mock_client_session_for_scan, responseContains(StrEq("Location"))).Times(AnyNumber()).WillRepeatedly(Return(true));
			EXPECT_CALL(*mock_client_session_for_scan, getFromResponse(StrEq("Location"))).Times(AnyNumber()).WillRepeatedly(Return(expected_http_location));
		}
		else {
			EXPECT_CALL(*mock_client_session_for_scan, responseContains(StrEq("Location"))).Times(AnyNumber()).WillRepeatedly(Return(false));
		}
		file_scanner = new FileScanner(std::move(mock_client_session_for_scan), test_scan_address, authenticator);
	}

	void SetUp() override {}

	void TearDown() override
	{
		delete file_scanner;
	}

};

TEST_F(TestFileScannerWithMockedAuthResponse, TestgetAddress) {
	//PRE: mock auth response, initialize expected address
	mock_auth_HTTP_response();
	auto expected_address = test_scan_address;

	//VERIFY: Verify that can adress is expected
	EXPECT_EQ(file_scanner->getAddress(), test_scan_address);
}
TEST_F(TestFileScannerWithMockedAuthResponse, TestgetScanAddress) {
	//PRE: mock auth response, initialize expected address
	mock_auth_HTTP_response();
	auto expected_address = test_scan_address + filescanner::API_SCAN_POSTFIX;

	//VERIFY: Verify that can adress is expected
	EXPECT_EQ(file_scanner->getScanAddress(), expected_address);
}
TEST_F(TestFileScannerWithMockedAuthResponse, TestpollWithValidResponseJSON) {

	//PRE: mock client session, sendRequest, receiveResponse for file scanning
	mock_scan_HTTP_response();

	//ACTION: Call FileScanner poll()
	auto poll_result = file_scanner->poll(std::string("poll_uri"));
	auto detection_result = poll_result.getDetections().front();

	//VERIFY: Verify that poll result is expected
	Detection expected_detection{ Detection::Category::SUSPICIOUS, std::string("detection1"), std::string("member1") };
	ScanResult expected_scan_result{ ScanResult::Status::COMPLETE, ScanResult::Result::SUSPICIOUS, std::vector<Detection>{expected_detection} };

	EXPECT_EQ(poll_result.getResult(), expected_scan_result.getResult());
	EXPECT_EQ(poll_result.getStatus(), expected_scan_result.getStatus());
	EXPECT_EQ(detection_result.getCategory(), expected_detection.getCategory());
	EXPECT_EQ(detection_result.getName(), expected_detection.getName());
	EXPECT_EQ(detection_result.getMemberName(), expected_detection.getMemberName());
}

TEST_F(TestFileScannerWithMockedAuthResponse, TestpollWithInvalidResponseJSON) {

	//PRE: mock client session, sendRequest, receiveResponse for file scanning
	std::string invalid_response_content{ "{ \"status\": \"INVALID\", \"scan_result\" : \"suspicious\","
			"\"detections\" : [ { \"name\" : \"detection1\", \"category\" : \"suspicious\", \"member_name\" : \"member1\" } ] }" };
	mock_scan_HTTP_response(HTTPResponse::HTTPStatus::HTTP_OK, invalid_response_content);
	
	//VERIFY: Verify that poll call throw is expected
	EXPECT_THROW(file_scanner->poll(std::string("poll_uri")), APIException);
}

TEST_F(TestFileScannerWithMockedAuthResponse, TestpollWithOtherThanHTTP_OKResponse) {

	//PRE: mock client session, sendRequest, receiveResponse for file scanning
	mock_scan_HTTP_response(HTTPResponse::HTTPStatus::HTTP_FORBIDDEN, "");

	//VERIFY: Verify that poll call throw is expected
	EXPECT_THROW(file_scanner->poll(std::string("poll_uri")), APIException);
}

TEST_F(TestFileScannerWithMockedAuthResponse, TestpollWithScanStatusPending) {

	//PRE: mock client session, sendRequest, receiveResponse for file scanning
	std::string pending_response_content{ "{ \"status\": \"pending\", \"scan_result\" : \"clean\"}" };
	mock_scan_HTTP_response(HTTPResponse::HTTPStatus::HTTP_OK, pending_response_content, "100", "", ScanResult::Status::PENDING);

	//ACTION: Call FileScanner poll()
	auto poll_result = file_scanner->poll(std::string("poll_uri"));

	//VERIFY: Verify that poll result is expected
	ScanResult expected_scan_result{ ScanResult::Status::PENDING, ScanResult::Result::CLEAN, std::vector<Detection>{},  100};

	EXPECT_EQ(poll_result.getStatus(), expected_scan_result.getStatus());
	EXPECT_EQ(poll_result.getRetryAfter(), expected_scan_result.getRetryAfter());
}

TEST_F(TestFileScannerWithMockedAuthResponse, TestpollWithScanStatusPendingMissingRetryAfter) {

	//PRE: mock client session, sendRequest, receiveResponse for file scanning
	std::string pending_response_content{ "{ \"status\": \"pending\", \"scan_result\" : \"clean\"}" };
	mock_scan_HTTP_response(HTTPResponse::HTTPStatus::HTTP_OK, pending_response_content);

	//VERIFY: Verify that poll call throw is expected
	EXPECT_THROW(file_scanner->poll(std::string("poll_uri")), APIException);
}

TEST_F(TestFileScannerWithMockedAuthResponse, TestpollWithScanStatusPendingInvalidRetryAfter) {

	//PRE: mock client session, sendRequest, receiveResponse for file scanning
	std::string pending_response_content{ "{ \"status\": \"pending\", \"scan_result\" : \"clean\"}" };
	mock_scan_HTTP_response(HTTPResponse::HTTPStatus::HTTP_OK, pending_response_content, "invalid");

	//VERIFY: Verify that poll call throw is expected
	EXPECT_THROW(file_scanner->poll(std::string("poll_uri")), APIException);
}

TEST_F(TestFileScannerWithMockedAuthResponse, TestscanWithValidResponseJSON) {
	//PRE: mock client session, sendRequest, receiveResponse for file scanning
	mock_scan_HTTP_response();
	ScanMetadata scan_metadata{};
	std::string file_to_scan{ "/some_path/some_file" };

	//ACTION: Call FileScanner scan()
	auto scan_result = file_scanner->scan(scan_metadata, std::ifstream(file_to_scan));
	auto detection_result = scan_result.getDetections().front();

	//VERIFY: Verify that scan result is expected
	Detection expected_detection{ Detection::Category::SUSPICIOUS, std::string("detection1"), std::string("member1") };
	ScanResult expected_scan_result{ ScanResult::Status::COMPLETE, ScanResult::Result::SUSPICIOUS, std::vector<Detection>{expected_detection} };

	EXPECT_EQ(scan_result.getResult(), expected_scan_result.getResult());
	EXPECT_EQ(scan_result.getStatus(), expected_scan_result.getStatus());
	EXPECT_EQ(detection_result.getCategory(), expected_detection.getCategory());
	EXPECT_EQ(detection_result.getName(), expected_detection.getName());
	EXPECT_EQ(detection_result.getMemberName(), expected_detection.getMemberName());
}

TEST_F(TestFileScannerWithMockedAuthResponse, TestscanWithInvalidResponseJSON) {
	//PRE: mock client session, sendRequest, receiveResponse for file scanning
	std::string invalid_response_content{ "{ \"status\": \"INVALID\", \"scan_result\" : \"suspicious\","
			"\"detections\" : [ { \"name\" : \"detection1\", \"category\" : \"suspicious\", \"member_name\" : \"member1\" } ] }" };
	mock_scan_HTTP_response(HTTPResponse::HTTPStatus::HTTP_OK, invalid_response_content);
	ScanMetadata scan_metadata{};
	std::string file_to_scan{ "/some_path/some_file" };

	//VERIFY: Verify that poll call throw is expected
	EXPECT_THROW(file_scanner->scan(scan_metadata, std::ifstream(file_to_scan)), APIException);
}

TEST_F(TestFileScannerWithMockedAuthResponse, TestscanWithHTTP_PROCESSINGResponse) {
	//PRE: mock client session, sendRequest, receiveResponse for file scanning
	std::string pending_response_content{ "{ \"status\": \"pending\", \"scan_result\" : \"clean\"}" };
	mock_scan_HTTP_response(HTTPResponse::HTTPStatus::HTTP_PROCESSING, pending_response_content, "100", "poll_uri");
	ScanMetadata scan_metadata{};

	//VERIFY: Verify that poll call throw is expected
	std::string file_to_scan{ "/some_path/some_file" };
	auto scan_result = file_scanner->scan(scan_metadata, std::ifstream(file_to_scan));
	
	//VERIFY: Verify that poll result is expected
	ScanResult expected_scan_result{ ScanResult::Status::PENDING, ScanResult::Result::CLEAN, std::vector<Detection>{},  100, "poll_uri" };

	EXPECT_EQ(scan_result.getRetryAfter(), expected_scan_result.getRetryAfter());
	EXPECT_EQ(scan_result.getPollURL(), expected_scan_result.getPollURL());
}

TEST_F(TestFileScannerWithMockedAuthResponse, TestscanWithHTTP_PROCESSINGResponseMissingRetryAfter) {
	//PRE: mock client session, sendRequest, receiveResponse for file scanning
	std::string pending_response_content{ "{ \"status\": \"pending\", \"scan_result\" : \"clean\"}" };
	mock_scan_HTTP_response(HTTPResponse::HTTPStatus::HTTP_PROCESSING, pending_response_content, "", "poll_uri");
	ScanMetadata scan_metadata{};
	std::string file_to_scan{ "/some_path/some_file" };

	//VERIFY: Verify that poll call throw is expected
	EXPECT_THROW(file_scanner->scan(scan_metadata, std::ifstream(file_to_scan)), APIException);
}
TEST_F(TestFileScannerWithMockedAuthResponse, TestscanWithHTTP_PROCESSINGResponseMissingLocation) {
	//PRE: mock client session, sendRequest, receiveResponse for file scanning
	std::string pending_response_content{ "{ \"status\": \"pending\", \"scan_result\" : \"clean\"}" };
	mock_scan_HTTP_response(HTTPResponse::HTTPStatus::HTTP_PROCESSING, pending_response_content, "");
	ScanMetadata scan_metadata{};
	std::string file_to_scan{ "/some_path/some_file" };

	//VERIFY: Verify that poll call throw is expected
	EXPECT_THROW(file_scanner->scan(scan_metadata, std::ifstream(file_to_scan)), APIException);
}

TEST_F(TestFileScannerWithMockedAuthResponse, TestscanWithHTTP_PROCESSINGResponseInvalidRetryAfter) {

	//PRE: mock client session, sendRequest, receiveResponse for file scanning
	std::string pending_response_content{ "{ \"status\": \"pending\", \"scan_result\" : \"clean\"}" };
	mock_scan_HTTP_response(HTTPResponse::HTTPStatus::HTTP_PROCESSING, pending_response_content, "invalid", "poll_uri");
	ScanMetadata scan_metadata{};
	std::string file_to_scan{ "/some_path/some_file" };

	//VERIFY: Verify that poll call throw is expected
	EXPECT_THROW(file_scanner->scan(scan_metadata, std::ifstream(file_to_scan)), APIException);
}
