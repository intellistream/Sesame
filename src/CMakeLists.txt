cmake_minimum_required (VERSION 3.0)
project (OnlineMLBenchmark)

add_subdirectory(DataStructure)
add_subdirectory(OfflineClustering)
add_subdirectory(OutlierDetection)
add_subdirectory(Summarization)
add_subdirectory(WindowModel)
add_executable(sesame Sesame.cpp)
