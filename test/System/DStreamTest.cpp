//
// Created by 1124a on 2021/11/2.
//

#include <gtest/gtest.h>

#include <filesystem>

#include "Algorithm/AlgorithmFactory.hpp"
#include "Algorithm/DataStructure/GenericFactory.hpp"
#include "Sinks/DataSinkFactory.hpp"
#include "Sources/DataSourceFactory.hpp"
#include "Utils/BenchmarkUtils.hpp"
#include "Utils/Logger.hpp"

using namespace SESAME;

TEST(System, DStream) {
  // Parse parameters.
  // [529, 999, 1270, 1624, 2001, 2435, 2648, 3000]
  // [3, 3, 4, 6, 6, 7, 9, 9]
  param_t cmd_params;
  cmd_params.num_points = 3000;
  cmd_params.dim = 54;
  cmd_params.lambda = 0.998;
  cmd_params.beta = 0.001;
  cmd_params.cm = 15;
  cmd_params.cl = 0.001;
  cmd_params.grid_width = 13;
  cmd_params.num_clusters = 7;
  cmd_params.time_decay = false;

  cmd_params.input_file = "datasets/CoverType.txt";
  cmd_params.algo = SESAME::DStreamType;

  // Run algorithm producing results.
  RunBenchmark(cmd_params);
}