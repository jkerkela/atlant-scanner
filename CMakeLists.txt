cmake_minimum_required(VERSION 3.14.4)

set (CMAKE_CXX_STANDARD 17)

project(atlant-scanner)

# Add test sources directory
add_subdirectory(Sample-Test1)

# Set source files
set(SOURCE_FILES 
	src/AtlantScanner.cpp
	src/Authenticator.cpp
	src/AuthTokenFetcher.cpp
	src/ContentMetadata.cpp
	src/FileScanner.cpp
	src/HTTPCommunicationImpl.cpp
	src/JSONHandlerImpl.cpp
	src/MultipartRequestBuilder.cpp
	src/ResultPrinter.cpp
	src/ScanMetadata.cpp
	src/ScanPoller.cpp
	src/ScanSettings.cpp
)

# Create implementation library
add_library(scanner STATIC
	${SOURCE_FILES}
)

# Find and copy 3rd party libs
find_library(PocoNet PocoNetd.lib HINTS ${CMAKE_SOURCE_DIR}/external/libs/Debug/)
if(NOT PocoNet)
    message(FATAL_ERROR "Library PocoNetd was not found!")
endif()
file(COPY ${CMAKE_SOURCE_DIR}/external/libs/Debug/PocoNetd.dll
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

find_library(PocoJSON PocoJSONd.lib HINTS ${CMAKE_SOURCE_DIR}/external/libs/Debug/)
if(NOT PocoJSON)
    message(FATAL_ERROR "Library PocoJSONd was not found!")
endif()
file(COPY ${CMAKE_SOURCE_DIR}/external/libs/Debug/PocoJSONd.dll
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

find_library(PocoFoundation PocoFoundationd.lib HINTS ${CMAKE_SOURCE_DIR}/external/libs/Debug/)
if(NOT PocoFoundation)
    message(FATAL_ERROR "Library PocoFoundationd was not found!")
endif()
file(COPY ${CMAKE_SOURCE_DIR}/external/libs/Debug/PocoFoundationd.dll
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

# Set include and link directories
target_include_directories(scanner PUBLIC
	${CMAKE_SOURCE_DIR}/include/
	${CMAKE_SOURCE_DIR}/external/includes/POCO/Net/include/
	${CMAKE_SOURCE_DIR}/external/includes/POCO/JSON/include/
	${CMAKE_SOURCE_DIR}/external/includes/POCO/Foundation/include/
)

target_link_libraries(scanner
	${PocoNet}
	${PocoJSON}
	${PocoFoundation}
)

# Create executable
add_executable(atlant-scanner src/AtlantScanner.cpp)
target_link_libraries(atlant-scanner PRIVATE
	scanner
)

