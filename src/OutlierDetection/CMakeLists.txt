cmake_minimum_required (VERSION 3.0)
project (OnlineMLBenchmark)

file(GLOB OutlierDetection_src *.cpp)
add_library(OutlierDetection ${OutlierDetection_src})