# adding the Google_Tests_run target
add_executable(google_test
        System/CluStreamTest.cpp
        System/StreamKMTest.cpp
        System/BirchTest.cpp
        System/EDMStreamTest.cpp
        System/DenStreamTest.cpp
        System/DBStreamTest.cpp
        System/DStreamTest.cpp
        System/SLKMeans.cpp
        System/GenericTest.cpp
)

target_compile_features(google_test PUBLIC cxx_std_20)
# linking Google_Tests_run with sesame_lib which will be tested
target_link_libraries(google_test gtest_main sesame)

add_custom_target(gtest_datasets ALL
COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/../datasets ${CMAKE_CURRENT_BINARY_DIR}/datasets)
add_dependencies(google_test gtest_datasets)

gtest_discover_tests(google_test)