### Node Engine Tests ###
add_executable(load-data-test "UnitTests/LoadDataTest.cpp")
target_link_libraries(load-data-test sesame)
add_test(NAME load-data-test COMMAND load-data-test)