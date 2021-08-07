# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

# adding the Google_Tests_run target
add_executable(Google_Tests_run
        SystemTest.cpp
        )

# linking Google_Tests_run with sesame_lib which will be tested
target_link_libraries(Google_Tests_run sesame gtest_main)

enable_testing()
add_test(
        NAME google_test
        COMMAND $<TARGET_FILE:Google_Tests_run>
)