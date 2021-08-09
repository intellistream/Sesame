# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)
project(SESAME_TEST)

# adding the Google_Tests_run target
add_executable(Google_Tests_run
        SystemTest.cpp
        )

# linking Google_Tests_run with sesame_lib which will be tested
target_link_libraries(Google_Tests_run sesame gtest_main)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/datasets
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_test(
        NAME SESAME_TEST
        COMMAND $<TARGET_FILE:Google_Tests_run>
)