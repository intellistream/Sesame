//
// Created by Shuhao Zhang on 19/07/2021.
//

#ifndef ONLINEMLBENCHMARK_BENCHMARK_SRC_UTIL_BENCHMARKUTILS_HPP_
#define ONLINEMLBENCHMARK_BENCHMARK_SRC_UTIL_BENCHMARKUTILS_HPP_

#include <cstdio>
#include <iostream>
#include <cstdlib>

struct param_t {
  int pointNumber;
  int clusterNumber;
  int dimension;
  int coreSize;
  int seed;
};

class BenchmarkUtils {
 public:
  static void parseArgs(int argc, char **argv, param_t *cmd_params);
  static void defaultParam( param_t *cmd_params);
};

#endif // ONLINEMLBENCHMARK_BENCHMARK_SRC_UTIL_BENCHMARKUTILS_HPP_
