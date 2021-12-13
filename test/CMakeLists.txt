# adding the Google_Tests_run target
add_executable(Google_Tests_run
        SystemTest/CluStreamTest.cpp
        SystemTest/StreamKMTest.cpp
        SystemTest/BirchTest.cpp
        SystemTest/EDMStreamTest.cpp
        SystemTest/DenStreamTest.cpp
        SystemTest/DBStreamTest.cpp
        SystemTest/DStreamTest.cpp
        )

# linking Google_Tests_run with sesame_lib which will be tested
target_link_libraries(Google_Tests_run gtest_main sesame)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/datasets
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

gtest_discover_tests(Google_Tests_run)