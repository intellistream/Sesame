# Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

cmake_minimum_required(VERSION 3.14)
project(SESAME CXX)

if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

# Custom CMake find instructions and macros
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
include(cmake/macros.cmake)
include(cmake/default.cmake)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#gcc 10 g++10
message(STATUS "sudo add-apt-repository 'deb http://mirrors.kernel.org/ubuntu hirsute main universe'")
message(STATUS "sudo apt-get update")
message(STATUS "sudo apt install gcc-11 g++-11")
message(STATUS "sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 11")
message(STATUS "sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 11")

# Set Optimization Flags
set(CMAKE_CXX_FLAGS "-g -std=c++20 -Wall -Werror=return-type -fconcepts-diagnostics-depth=2 -lpthread -fopenmp -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -DNO_RACE_CHECK -DSESAME_DEBUG_MODE=1 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-Wno-ignored-qualifiers -Wno-sign-compare -O3 -DNDEBUG")

# Set LOGGING_LEVEL Flag
if (SESAME_LOGGING_LEVEL)
    get_log_level_value(SESAME_LOGGING_VALUE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSESAME_LOGGING_LEVEL=${SESAME_LOGGING_VALUE}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSESAME_LOGGING_LEVEL=${SESAME_LOGGING_VALUE}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DSESAME_LOGGING_LEVEL=${SESAME_LOGGING_VALUE}")
else (SESAME_LOGGING_LEVEL)
    message("---Everything will be logged")
endif (SESAME_LOGGING_LEVEL)

message(STATUS "CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE}")

option(ENABLE_UNIT_TESTS "Enable unit tests" ON)
message(STATUS "Enable testing: ${ENABLE_UNIT_TESTS}")

if (ENABLE_UNIT_TESTS)
    enable_testing()
    # Google Test
    include(FetchContent)

    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.11.0
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    include(GoogleTest)
#    find_package(GTest 1.11.0)
endif ()

find_package(OpenMP)
find_package(gflags REQUIRED)

# Print all used include directories
message(STATUS "INCLUDE_DIRS:")
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach (dir ${dirs})
    message(STATUS "  - ${dir}")
endforeach ()

# Add Source Code
add_subdirectory(src)

# Add Library
get_source_sesame(sesame_SOURCE_FILES)
get_header_sesame(sesame_HEADER_FILES)
add_library(sesame STATIC ${sesame_SOURCE_FILES} ${sesame_HEADER_FILES} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(sesame PUBLIC "include")
target_include_directories(sesame PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(sesame PUBLIC ${LIBRARIES} pthread gflags)

#Add benchmarks with command
add_subdirectory(benchmark)

#Add tests (based on GTest suits)
add_subdirectory(test)

install(DIRECTORY "include" DESTINATION "/sesame" COMPONENT SESAME)

