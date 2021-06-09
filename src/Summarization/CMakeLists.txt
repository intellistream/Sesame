cmake_minimum_required (VERSION 3.0)
project (OnlineMLBenchmark)

file(GLOB Summarization_src *.cpp)
add_library(Summarization ${Summarization_src})