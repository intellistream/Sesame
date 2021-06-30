cmake_minimum_required (VERSION 3.0)
project (OnlineMLBenchmark)

file(GLOB DataStructure_src *.cpp)
add_library(DataStructure ${DataStructure_src})