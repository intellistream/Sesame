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

struct param_t {
  int pointNumber;
  int clusterNumber;
  int dimension;
  int coresetSize;
  int seed;
  unsigned int lastArrivingNum;
  unsigned int timeWindow;
  unsigned int timeInterval;
  int onlineClusterNumber;
  double radiusFactor;
  int initBuffer;
  unsigned int offlineTimeWindow;
  std::string inputPath;
  std::string outputPath;
  std::string algoName;
};

class BenchmarkUtils {
 public:
  static void print_help(char *string);
  static void parseArgs(int argc, char **argv, param_t &cmd_params);
  static void defaultParam(param_t &cmd_params);
  static void loadData(param_t &cmd_params, std::vector<SESAME::PointPtr> &input);
  static void runBenchmark(param_t &cmd_params,
                           const std::vector<SESAME::PointPtr> &input,
                           std::vector<SESAME::PointPtr> &output);
  static void evaluate(param_t &cmd_params,
                       const std::vector<SESAME::PointPtr> &input,
                       const std::vector<SESAME::PointPtr> &output);
  static void store(param_t &cmd_params, const std::vector<SESAME::PointPtr> &output);
};

#endif // ONLINEMLBENCHMARK_BENCHMARK_SRC_UTIL_BENCHMARKUTILS_HPP_
