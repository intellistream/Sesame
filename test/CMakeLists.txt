cmake_minimum_required(VERSION 3.14)
# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)
#project(SESAME_TEST)
enable_testing()
include(GoogleTest)
find_package(GTest 1.10.0)

# adding the Google_Tests_run target
add_executable(Google_Tests_run
        SystemTest.cpp
        )

# linking Google_Tests_run with sesame_lib which will be tested
target_link_libraries(Google_Tests_run sesame gtest gtest_main gmock gmock_main)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/datasets
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

gtest_add_tests(TARGET Google_Tests_run)