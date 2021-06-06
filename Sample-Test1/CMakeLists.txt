# Add googletest sources
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

set(SOURCE_LIBS
  gtest_main
  gmock_main
  scanner
)

add_executable(atlant-scanner-tests
  AuthenticatorTests.cpp
  AuthTokenFetcherTests.cpp
  AuthTokenTests.cpp
  ContentMetadataTests.cpp
  DetectionTests.cpp
  FileScannerTests.cpp
  MultiPartRequestBuilderTests.cpp
  ResultPrinterTest.cpp
  ScanMetadataTests.cpp
  ScanPollerTests.cpp
  ScanResultTests.cpp
  ScanSettingsTests.cpp
)

target_link_libraries(atlant-scanner-tests
  ${SOURCE_LIBS}
)

target_include_directories(atlant-scanner-tests PRIVATE
	${CMAKE_SOURCE_DIR}/Sample-Test1/
)

gtest_discover_tests(atlant-scanner-tests)

