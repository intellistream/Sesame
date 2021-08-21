// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 19/07/2021.
//

#ifndef ONLINEMLBENCHMARK_BENCHMARK_SRC_UTIL_BENCHMARKUTILS_HPP_
#define ONLINEMLBENCHMARK_BENCHMARK_SRC_UTIL_BENCHMARKUTILS_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <Algorithm/DataStructure/Point.hpp>
#include <memory>
#include <vector>
#include <Sources/DataSource.hpp>
#include <Sinks/DataSink.hpp>
#include <Algorithm/Algorithm.hpp>

struct param_t {
  int pointNumber;
  int clusterNumber;
  int dimension;
  int coresetSize;
  int seed;
  int lastArrivingNum;
  int timeWindow;
  unsigned int timeInterval;
  int onlineClusterNumber;
  double radiusFactor;
  int initBuffer;
  int offlineTimeWindow;
  std::string inputPath;
  std::string outputPath;
  std::string algoName;

};

class BenchmarkUtils {
 public:
  #ifdef USELOG4CXX
    static void print_help(char *string);
  #endif
  static void parseArgs(int argc, char **argv, param_t &cmd_params);
  static void defaultParam(param_t &cmd_params);
  static void loadData(param_t &cmd_params, SESAME::DataSourcePtr dataSourcePtr);
  static void runBenchmark(param_t &cmd_params,
                           SESAME::DataSourcePtr sourcePtr,
                           SESAME::DataSinkPtr sinkPtr,
                           SESAME::AlgorithmPtr algoPtr);
};

#endif // ONLINEMLBENCHMARK_BENCHMARK_SRC_UTIL_BENCHMARKUTILS_HPP_
