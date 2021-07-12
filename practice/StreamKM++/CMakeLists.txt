cmake_minimum_required(VERSION 3.19)
project(StreamKm++)

# 同时指定工程版本和语言
project(StreamKm++ VERSION 0.0.1 LANGUAGES CXX)

# 设置 c/c++版本
set(CMAKE_CXX_STANDARD 11)


# 设置编译参数
# set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} /Users/tuidan/code/C++/StreamClustering/StreamKm++_Cmake/new.txt")
include_directories(DataStructure;Entrance;OfflineClustering;Utility;WindowModel/LandmarkWindow)


add_executable(StreamKM++
        Utility/DatasetFunctions.hpp
        Utility/featurepp.hpp
        OfflineClustering/kMeansUtilityFunctions.hpp
        OfflineClustering/LloydPlusPlus.hpp
        Utility/mt19937ar.hpp
        WindowModel/LandmarkWindow/StreamingCoreset.hpp
        DataStructure/TreeCoreset.hpp
        Utility/DatasetFunctions.cpp
        Utility/featuretools.cpp
        OfflineClustering/kMeansUtilityFunctions.cpp
        OfflineClustering/LloydPlusPlus.cpp
        Entrance/StreamKM++.cpp
        Utility/mt19937ar.cpp
        DataStructure/Point.cpp
        WindowModel/LandmarkWindow/StreamingCoreset.cpp
        DataStructure/TreeCoreset.cpp)
