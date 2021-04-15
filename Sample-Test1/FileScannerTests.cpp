#include "pch.h"
#include "FileScanner.hpp"
#include "MockHTTPCommunication.hpp"

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
TEST(FileScanner, Testpoll) {
	//PRE: Mock client session, sendRequest, receiveResponse for auth token requests
	auto mock_client_session_for_auth = std::make_unique<MockHTTPClientSessionImpl>();
	std::ostringstream buf;
	EXPECT_CALL(*mock_client_session_for_auth, sendRequest(_)).Times(1).WillOnce(ReturnRef(buf));
	std::string response_content{ "{ \"access_token\": \"Token1\", \"token_type\" : \"bearer\", \"expires_in\" : 3600 }" };
	std::istringstream is{ response_content };
	EXPECT_CALL(*mock_client_session_for_auth, receiveResponse(_)).Times(1).WillOnce(ReturnRef(is));

	//PRE: Mock client session, sendRequest, receiveResponse for file scanning
	auto mock_client_session_for_scan = std::make_unique<MockHTTPClientSessionImpl>();
	std::ostringstream buf2;
	EXPECT_CALL(*mock_client_session_for_scan, sendRequest(_)).Times(1).WillOnce(ReturnRef(buf2));
	std::string scan_response_content{ "{ \"status\": \"complete\", \"scan_result\" : \"suspicious\","
		"\"detections\" : [ { \"name\" : \"detection1\", \"category\" : \"suspicious\", \"member_name\" : \"member1\" } ] }" };
	std::istringstream is2{ scan_response_content };
	EXPECT_CALL(*mock_client_session_for_scan, receiveResponse(_)).Times(1).WillOnce(ReturnRef(is2));

	//PRE: Create Authenticator and FileScanner objects
	Authenticator authenticator{ std::make_unique<AuthTokenFetcher>("auth_address", std::move(mock_client_session_for_auth)),
		"client_ID",
		"client_secret",
		std::set<std::string>{ "scope1" }
	};
	FileScanner file_scanner{ "some_scannig_address", authenticator };

	//ACTION: Call FileScanner poll()
	std::string file_to_scan{ "/some_path/some_file" };
	auto poll_result = file_scanner.poll(std::move(mock_client_session_for_scan), std::string("poll_uri"));
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

TEST(FileScanner, Testscan) {
	//PRE: mock client session, sendRequest, receiveResponse for auth token requests
	auto mock_client_session_for_auth = std::make_unique<MockHTTPClientSessionImpl>();
	std::ostringstream buf;
	EXPECT_CALL(*mock_client_session_for_auth, sendRequest(_)).Times(1).WillOnce(ReturnRef(buf));
	std::string response_content{ "{ \"access_token\": \"Token1\", \"token_type\" : \"bearer\", \"expires_in\" : 3600 }" };
	std::istringstream is{ response_content };
	EXPECT_CALL(*mock_client_session_for_auth, receiveResponse(_)).Times(1).WillOnce(ReturnRef(is));

	//PRE: mock client session, sendRequest, receiveResponse for file scanning
	auto mock_client_session_for_scan = std::make_unique<MockHTTPClientSessionImpl>();
	std::ostringstream buf2;
	EXPECT_CALL(*mock_client_session_for_scan, sendRequest(_)).Times(1).WillOnce(ReturnRef(buf2));
	std::string scan_response_content{ "{ \"status\": \"complete\", \"scan_result\" : \"suspicious\","
		"\"detections\" : [ { \"name\" : \"detection1\", \"category\" : \"suspicious\", \"member_name\" : \"member1\" } ] }" };
	std::istringstream is2{ scan_response_content };
	EXPECT_CALL(*mock_client_session_for_scan, receiveResponse(_)).Times(1).WillOnce(ReturnRef(is2));

	//PRE: create Authenticator and FileScanner objects
	Authenticator authenticator{ std::make_unique<AuthTokenFetcher>("auth_address", std::move(mock_client_session_for_auth)),
		"client_ID",
		"client_secret",
		std::set<std::string>{ "scope1" }
	};
	FileScanner file_scanner{ "some_scannig_address", authenticator };
	ScanMetadata scan_metadata{};

	//ACTION: Call FileScanner scan()
	std::string file_to_scan{ "/some_path/some_file" };
	auto scan_result = file_scanner.scan(scan_metadata, std::ifstream(file_to_scan), std::move(mock_client_session_for_scan));
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
