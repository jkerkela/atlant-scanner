#include "pch.h"
#include "FileScanner.hpp"
#include "MockHTTPCommunication.hpp"
#include "APIException.hpp"

#include <Poco/Net/NetException.h>

#include <string>
#include <set>
#include <memory>

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;

TEST(FileScanner, TestConstructorWithNonAccessibleAuthAddress) {
	Authenticator authenticator{ std::make_unique<AuthTokenFetcher>("auth_address", std::make_unique<HTTPClientSessionImpl>("auth_address")), 
		"client_ID", 
		"client_secret", 
		std::set<std::string>{ "scope1" }
	};
	EXPECT_THROW(FileScanner("some_scannig_address", authenticator), NetException);
}

class TestFileScannerWithPreMockedAuthResponse : public ::testing::Test
{
protected:

	FileScanner* file_scanner;
	std::unique_ptr<MockHTTPClientSessionImpl> mock_client_session_for_auth;
	std::unique_ptr<MockHTTPClientSessionImpl> mock_client_session_for_scan;

	std::ostringstream auth_response_buf;
	std::stringstream auth_response_stream;
	std::string auth_response_content{ "{ \"access_token\": \"Token1\", \"token_type\" : \"bearer\", \"expires_in\" : 3600 }" };
	
	std::ostringstream scan_response_buf;
	std::stringstream scan_response_stream;
	inline static std::string scan_response_content{ "{ \"status\": \"complete\", \"scan_result\" : \"suspicious\","
			"\"detections\" : [ { \"name\" : \"detection1\", \"category\" : \"suspicious\", \"member_name\" : \"member1\" } ] }" };

	void mock_scan_response(const std::string& scan_resp = scan_response_content) 
	{
		mock_client_session_for_scan = std::make_unique<MockHTTPClientSessionImpl>();
		EXPECT_CALL(*mock_client_session_for_scan, sendRequest(_)).Times(1);
		scan_response_stream << scan_resp;
		EXPECT_CALL(*mock_client_session_for_scan, receiveResponse(_)).Times(1).WillOnce(ReturnRef(scan_response_stream));
	}

	void SetUp() override
	{
		//PRE: Mock client session, sendRequest, receiveResponse for auth token requests
		mock_client_session_for_auth = std::make_unique<MockHTTPClientSessionImpl>();
		EXPECT_CALL(*mock_client_session_for_auth, sendRequest(_)).Times(1);
		auth_response_stream << auth_response_content;
		EXPECT_CALL(*mock_client_session_for_auth, receiveResponse(_)).Times(1).WillOnce(ReturnRef(auth_response_stream));

		//PRE: Create Authenticator and FileScanner objects
		Authenticator authenticator{ std::make_unique<AuthTokenFetcher>("auth_address", std::move(mock_client_session_for_auth)),
			"client_ID",
			"client_secret",
			std::set<std::string>{ "scope1" }
		};
		file_scanner = new FileScanner("some_scannig_address", authenticator);
	}
	
	void TearDown() override 
	{
		delete file_scanner;
	}

};

TEST_F(TestFileScannerWithPreMockedAuthResponse, TestpollWithValidResponseJSON) {

	//PRE: mock client session, sendRequest, receiveResponse for file scanning
	mock_scan_response();

	//ACTION: Call FileScanner poll()
	std::string file_to_scan{ "/some_path/some_file" };
	auto poll_result = file_scanner->poll(std::move(mock_client_session_for_scan), std::string("poll_uri"));
	auto detection_result = poll_result.getDetections().front();

	//VERIFY: Verify that poll result is expected
	Detection expected_detection{ Detection::Category::SUSPICIOUS, std::string("detection1"), std::string("member1") };
	ScanResult expected_scan_result{ ScanResult::Status::COMPLETE, ScanResult::Result::SUSPICIOUS, std::list<Detection>{expected_detection} };

	EXPECT_EQ(poll_result.getResult(), expected_scan_result.getResult());
	EXPECT_EQ(poll_result.getStatus(), expected_scan_result.getStatus());
	EXPECT_EQ(detection_result.getCategory(), expected_detection.getCategory());
	EXPECT_EQ(detection_result.getName(), expected_detection.getName());
	EXPECT_EQ(detection_result.getMemberName(), expected_detection.getMemberName());
}

TEST_F(TestFileScannerWithPreMockedAuthResponse, TestpollWithInvalidResponseJSON) {

	//PRE: mock client session, sendRequest, receiveResponse for file scanning
	std::string invalid_response_content{ "{ \"status\": \"INVALID\", \"scan_result\" : \"suspicious\","
			"\"detections\" : [ { \"name\" : \"detection1\", \"category\" : \"suspicious\", \"member_name\" : \"member1\" } ] }" };
	mock_scan_response(invalid_response_content);
	
	//VERIFY: Verify that poll call throw is expected
	std::string file_to_scan{ "/some_path/some_file" };
	EXPECT_THROW(file_scanner->poll(std::move(mock_client_session_for_scan), std::string("poll_uri")), APIException);
}

TEST_F(TestFileScannerWithPreMockedAuthResponse, TestscanWithValidResponseJSON) {
	//PRE: mock client session, sendRequest, receiveResponse for file scanning
	mock_scan_response();
	ScanMetadata scan_metadata{};

	//ACTION: Call FileScanner scan()
	std::string file_to_scan{ "/some_path/some_file" };
	auto scan_result = file_scanner->scan(scan_metadata, std::ifstream(file_to_scan), std::move(mock_client_session_for_scan));
	auto detection_result = scan_result.getDetections().front();

	//VERIFY: Verify that scan result is expected
	Detection expected_detection{ Detection::Category::SUSPICIOUS, std::string("detection1"), std::string("member1") };
	ScanResult expected_scan_result{ ScanResult::Status::COMPLETE, ScanResult::Result::SUSPICIOUS, std::list<Detection>{expected_detection} };

	EXPECT_EQ(scan_result.getResult(), expected_scan_result.getResult());
	EXPECT_EQ(scan_result.getStatus(), expected_scan_result.getStatus());
	EXPECT_EQ(detection_result.getCategory(), expected_detection.getCategory());
	EXPECT_EQ(detection_result.getName(), expected_detection.getName());
	EXPECT_EQ(detection_result.getMemberName(), expected_detection.getMemberName());
}

TEST_F(TestFileScannerWithPreMockedAuthResponse, TestscanWithInvalidResponseJSON) {
	//PRE: mock client session, sendRequest, receiveResponse for file scanning
	std::string invalid_response_content{ "{ \"status\": \"INVALID\", \"scan_result\" : \"suspicious\","
			"\"detections\" : [ { \"name\" : \"detection1\", \"category\" : \"suspicious\", \"member_name\" : \"member1\" } ] }" };
	mock_scan_response(invalid_response_content);
	ScanMetadata scan_metadata{};

	//VERIFY: Verify that poll call throw is expected
	std::string file_to_scan{ "/some_path/some_file" };
	EXPECT_THROW(file_scanner->scan(scan_metadata, std::ifstream(file_to_scan), std::move(mock_client_session_for_scan)), APIException);
}
