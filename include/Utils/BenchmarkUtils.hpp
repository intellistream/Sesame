// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 19/07/2021.
//

#ifndef ONLINEMLBENCHMARK_BENCHMARK_SRC_UTIL_BENCHMARKUTILS_HPP_
#define ONLINEMLBENCHMARK_BENCHMARK_SRC_UTIL_BENCHMARKUTILS_HPP_

#include "Algorithm/Algorithm.hpp"
#include "Algorithm/DataStructure/Point.hpp"
#include "Evaluation/Evaluation.hpp"
#include "Sinks/DataSink.hpp"
#include "Sources/DataSource.hpp"

#include <getopt.h>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace SESAME;

class BenchmarkUtils
{
public:
    static void loadData(param_t &cmd_params, DataSourcePtr dataSourcePtr);
    static BenchmarkResultPtr RunBenchmark(param_t &cmd_params);
};

#endif  // ONLINEMLBENCHMARK_BENCHMARK_SRC_UTIL_BENCHMARKUTILS_HPP_
