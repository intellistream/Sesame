cmake_minimum_required (VERSION 3.0)
project (OnlineMLBenchmark)

file(GLOB WindowModel_src *.cpp)
add_library(WindowModel ${WindowModel_src})