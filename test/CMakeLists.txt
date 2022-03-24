# adding the Google_Tests_run target
add_executable(google_test
        SystemTest/CluStreamTest.cpp
        SystemTest/StreamKMTest.cpp
        SystemTest/BirchTest.cpp
        SystemTest/EDMStreamTest.cpp
        SystemTest/DenStreamTest.cpp
        SystemTest/DBStreamTest.cpp
        SystemTest/DStreamTest.cpp
        DesignAspect/V1Test.cpp
        DesignAspect/V2Test.cpp
        DesignAspect/V3Test.cpp
        DesignAspect/V4Test.cpp
        DesignAspect/V5Test.cpp
        DesignAspect/V6Test.cpp
        DesignAspect/V7Test.cpp
        DesignAspect/V8Test.cpp
        Generic/GenericTest.cpp
)

target_compile_features(google_test PUBLIC cxx_std_20)
# linking Google_Tests_run with sesame_lib which will be tested
target_link_libraries(google_test gtest_main sesame)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/datasets
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

gtest_discover_tests(google_test)