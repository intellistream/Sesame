cmake_minimum_required(VERSION 3.19)
project(Clustream)

set(CMAKE_CXX_STANDARD 20)

add_executable(Clustream main.cpp microcluster.cpp KMeans.cpp Clustream.cpp)