cmake_minimum_required (VERSION 3.0)
project (OnlineMLBenchmark)

file(GLOB OfflineClustering_src *.cpp)
add_library(OfflineClustering ${OfflineClustering_src})