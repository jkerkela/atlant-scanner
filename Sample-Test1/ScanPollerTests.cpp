#include "pch.h"
#include "ScanPoller.hpp"
#include "MockHTTPCommunication.hpp"
#include "APIException.hpp"

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrEq;
using ::testing::AnyNumber;


class TestScanPollerWithMockedAuthResponse : public ::testing::Test
{
protected:

	FileScanner* file_scanner = nullptr;
	std::string test_scan_address = "scan_uri";

	std::unique_ptr<MockHTTPClientSessionImpl> mock_client_session_for_auth;
	std::unique_ptr<MockHTTPClientSessionImpl> mock_client_session_for_scan;
	std::unique_ptr<HTTPResponseImpl> mock_http_response;

	std::stringstream auth_response_stream;
	std::string auth_response_content{ "{ \"access_token\": \"Token1\", \"token_type\" : \"bearer\", \"expires_in\" : 3600 }" };

	std::stringstream scan_response_stream;
	inline static std::string complete_scan_response_content{ "{ \"status\": \"complete\", \"scan_result\" : \"suspicious\","
			"\"detections\" : [ { \"name\" : \"detection1\", \"category\" : \"suspicious\", \"member_name\" : \"member1\" } ] }" };

	std::stringstream poll_response_stream;

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

	void mock_scan_HTTP_response(
		const std::string& scan_resp = complete_scan_response_content,
		const HTTPResponse::HTTPStatus http_status = HTTPResponse::HTTPStatus::HTTP_OK,
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

	void mock_scan_to_poll_HTTP_responses(
		const std::string& scan_resp,
		const std::string& poll_resp = complete_scan_response_content,
		const HTTPResponse::HTTPStatus poll_http_status = HTTPResponse::HTTPStatus::HTTP_OK,
		const HTTPResponse::HTTPStatus scan_http_status = HTTPResponse::HTTPStatus::HTTP_PROCESSING,
		const std::string& expected_http_retry_after = "1",
		const std::string& expected_http_location = "some_location"
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
		EXPECT_CALL(*mock_client_session_for_scan, sendRequest(_)).Times(2);
		mock_http_response = std::make_unique<HTTPResponseImpl>();
		scan_response_stream << scan_resp;
		poll_response_stream  << poll_resp;
		EXPECT_CALL(*mock_client_session_for_scan, receiveResponse()).Times(2)
			.WillOnce(ReturnRef(scan_response_stream))
			.WillOnce(ReturnRef(poll_response_stream));

		EXPECT_CALL(*mock_client_session_for_scan, getResponseStatus()).Times(2)
			.WillOnce(Return(scan_http_status))
			.WillOnce(Return(poll_http_status));
	
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

TEST_F(TestScanPollerWithMockedAuthResponse, TestConstructor) {
	//PRE: Mock client session, sendRequest, receiveResponse for auth token requests
	mock_auth_HTTP_response();
	//VERIFY: verify that constructor is callable with valid parameters
	EXPECT_NO_THROW(ScanPoller(*file_scanner));
}


TEST_F(TestScanPollerWithMockedAuthResponse, TestscanWithCompleteResult) {
	//PRE: Mock client session, sendRequest, receiveResponse for scan requests 
	mock_scan_HTTP_response();

	//ACTION: Create ScanPoller object, call ScanPoller scan ()
	ScanPoller scan_poller{ *file_scanner };
	ScanMetadata metadata{};
	auto scan_result = scan_poller.scan(metadata, std::string("scan_path"));
	auto detection_result = scan_result.getDetections().front();

	//VERIFY: verify that result
	Detection expected_detection {Detection::Category::SUSPICIOUS, std::string("detection1"), std::string("member1")};
	ScanResult expected_scan_result{ ScanResult::Status::COMPLETE, ScanResult::Result::SUSPICIOUS, std::vector<Detection>{expected_detection} };
	EXPECT_EQ(scan_result.getResult(), expected_scan_result.getResult());
	EXPECT_EQ(scan_result.getStatus(), expected_scan_result.getStatus());
	EXPECT_EQ(detection_result.getCategory(), expected_detection.getCategory());
	EXPECT_EQ(detection_result.getName(), expected_detection.getName());
	EXPECT_EQ(detection_result.getMemberName(), expected_detection.getMemberName());
}

TEST_F(TestScanPollerWithMockedAuthResponse, TestscanWithPendingResponseMissingRetryAfter)
{
	//PRE: Mock client session, sendRequest, receiveResponse for scan requests
	std::string scan_pending_response_content{ "{ \"status\": \"pending\", \"scan_result\" : \"suspicious\","
			"\"detections\" : [ { \"name\" : \"detection1\", \"category\" : \"suspicious\", \"member_name\" : \"member1\" } ] }" };
	mock_scan_HTTP_response(scan_pending_response_content,
		HTTPResponse::HTTPStatus::HTTP_PROCESSING,
		"",
		"location");

	//ACTION: Create ScanPoller object, call ScanPoller scan ()
	ScanPoller scan_poller{ *file_scanner };
	ScanMetadata metadata{};

	//VERIFY: Verify that scan call throw is expected
	EXPECT_THROW(scan_poller.scan(metadata, std::string("scan_path")), APIException);
}

TEST_F(TestScanPollerWithMockedAuthResponse, TestscanWithPendingResponseMissingLocation)
{
	//PRE: Mock client session, sendRequest, receiveResponse for scan requests
	std::string scan_pending_response_content{ "{ \"status\": \"pending\", \"scan_result\" : \"suspicious\","
			"\"detections\" : [ { \"name\" : \"detection1\", \"category\" : \"suspicious\", \"member_name\" : \"member1\" } ] }" };
	mock_scan_HTTP_response(scan_pending_response_content,
		HTTPResponse::HTTPStatus::HTTP_PROCESSING,
		"1",
		"");

	//ACTION: Create ScanPoller object, call ScanPoller scan ()
	ScanPoller scan_poller{ *file_scanner };
	ScanMetadata metadata{};

	//VERIFY: Verify that scan call throw is expected
	EXPECT_THROW(scan_poller.scan(metadata, std::string("scan_path")), APIException);
}
TEST_F(TestScanPollerWithMockedAuthResponse, TestscanWithPendingToCompleteResult) {
	//PRE: Mock client session, sendRequest, receiveResponse for scan requests
	std::string scan_pending_response_content{ "{ \"status\": \"pending\", \"scan_result\" : \"suspicious\","
			"\"detections\" : [ { \"name\" : \"detection1\", \"category\" : \"suspicious\", \"member_name\" : \"member1\" } ] }" };
	mock_scan_to_poll_HTTP_responses(scan_pending_response_content);

	//ACTION: Create ScanPoller object, call ScanPoller scan ()
	ScanPoller scan_poller{ *file_scanner };
	ScanMetadata metadata{};
	auto scan_result = scan_poller.scan(metadata, std::string("scan_path"));
	auto detection_result = scan_result.getDetections().front();

	//VERIFY: verify that result
	Detection expected_detection{ Detection::Category::SUSPICIOUS, std::string("detection1"), std::string("member1") };
	ScanResult expected_scan_result{ ScanResult::Status::COMPLETE, ScanResult::Result::SUSPICIOUS, std::vector<Detection>{expected_detection} };
	EXPECT_EQ(scan_result.getResult(), expected_scan_result.getResult());
	EXPECT_EQ(scan_result.getStatus(), expected_scan_result.getStatus());
	EXPECT_EQ(detection_result.getCategory(), expected_detection.getCategory());
	EXPECT_EQ(detection_result.getName(), expected_detection.getName());
	EXPECT_EQ(detection_result.getMemberName(), expected_detection.getMemberName());
}

TEST_F(TestScanPollerWithMockedAuthResponse, TestscanWithPendingToInvalidResult) {
	//PRE: Mock client session, sendRequest, receiveResponse for scan requests
	std::string scan_pending_response_content{ "{ \"status\": \"pending\", \"scan_result\" : \"suspicious\","
			"\"detections\" : [ { \"name\" : \"detection1\", \"category\" : \"suspicious\", \"member_name\" : \"member1\" } ] }" };
	std::string poll_invalid_response_content{ "{ \"status\": \"invalid\", \"scan_result\" : \"suspicious\","
			"\"detections\" : [ { \"name\" : \"detection1\", \"category\" : \"suspicious\", \"member_name\" : \"member1\" } ] }" };
	mock_scan_to_poll_HTTP_responses(scan_pending_response_content, poll_invalid_response_content);

	//ACTION: Create ScanPoller object, call ScanPoller scan ()
	ScanPoller scan_poller{ *file_scanner };
	ScanMetadata metadata{};

	//VERIFY: Verify that scan call throw is expected
	EXPECT_THROW(scan_poller.scan(metadata, std::string("scan_path")), APIException);
}

TEST_F(TestScanPollerWithMockedAuthResponse, TestscanWithPendingToOtherThanHTTP_OKResponse) 
{
	//PRE: Mock client session, sendRequest, receiveResponse for scan requests
	std::string scan_pending_response_content{ "{ \"status\": \"pending\", \"scan_result\" : \"suspicious\","
			"\"detections\" : [ { \"name\" : \"detection1\", \"category\" : \"suspicious\", \"member_name\" : \"member1\" } ] }" };
	std::string poll_valid_response_content{ "{ \"status\": \"complete\", \"scan_result\" : \"suspicious\","
			"\"detections\" : [ { \"name\" : \"detection1\", \"category\" : \"suspicious\", \"member_name\" : \"member1\" } ] }" };
	mock_scan_to_poll_HTTP_responses(scan_pending_response_content, poll_valid_response_content, HTTPResponse::HTTPStatus::HTTP_FORBIDDEN);

	//ACTION: Create ScanPoller object, call ScanPoller scan ()
	ScanPoller scan_poller{ *file_scanner };
	ScanMetadata metadata{};

	//VERIFY: Verify that scan call throw is expected
	EXPECT_THROW(scan_poller.scan(metadata, std::string("scan_path")), APIException);
}
