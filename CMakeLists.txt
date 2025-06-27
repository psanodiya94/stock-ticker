cmake_minimum_required(VERSION 3.10.0)
project(stock-ticker VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Export compile commands for tooling support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find Qt Packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network Test)

# Enable automatic moc, uic and rcc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Source Files
file(GLOB_RECURSE SRC_SOURCES "src/*.cpp")
file(GLOB_RECURSE INC_HEADERS "include/*.h")
set(SOURCES ${SRC_SOURCES} ${INC_HEADERS})

# Build main library
add_library(stock-ticker-lib ${SOURCES})
target_link_libraries(stock-ticker-lib PRIVATE Qt6::Core Qt6::Widgets Qt6::Network spdlog::spdlog fmt::fmt)
target_include_directories(stock-ticker-lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Main executable
add_executable(stock-ticker src/StockTickerMain.cpp)
target_include_directories(stock-ticker PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(stock-ticker PRIVATE stock-ticker-lib Qt6::Core Qt6::Widgets Qt6::Network spdlog::spdlog fmt::fmt)

# Set output directories
set_target_properties(stock-ticker PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Ensure output directories are used for all configurations (e.g., Debug/Release on Windows)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_BINARY_DIR}/lib)
endforeach()

# Enable warnings
if (MSVC)
    target_compile_options(stock-ticker PRIVATE /W4 /permissive-)
else()
    target_compile_options(stock-ticker PRIVATE -Wall -Wextra -pedantic)
endif()

# Group sources in IDEs
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${SRC_SOURCES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${INC_HEADERS})

# Install target
install(TARGETS stock-ticker RUNTIME DESTINATION bin)

# Add Doxygen support
find_package(Doxygen REQUIRED)

if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(doc
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()

# GoogleTest/GoogleMock integration (system install)
find_package(GTest REQUIRED)

# GMock may not have a separate find_package in older CMake versions
# Use GTest to locate GMock if necessary
find_path(GMOCK_INCLUDE_DIRS gmock/gmock.h)
find_library(GMOCK_LIBRARY NAMES gmock libgmock)
find_library(GMOCK_MAIN_LIBRARY NAMES gmock_main libgmock_main)

# Find SPDLOGspdlog and fmt for logging
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

# Add test sources (adjust path as needed)
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
file(GLOB_RECURSE TEST_INCLUDES "tests/*.h")
if(TEST_SOURCES)
    add_executable(stock-ticker-tests ${TEST_SOURCES} ${TEST_INCLUDES})
    target_include_directories(stock-ticker-tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    target_link_libraries(stock-ticker-tests PRIVATE stock-ticker-lib GTest::gtest GTest::gmock GTest::gtest_main Qt6::Core Qt6::Widgets Qt6::Network Qt6::Test spdlog::spdlog fmt::fmt)
    add_test(NAME stock-ticker-tests COMMAND stock-ticker-tests)
endif()

enable_testing()

