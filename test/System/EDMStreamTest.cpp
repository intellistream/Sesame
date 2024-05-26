//
// Created by tuidan on 2021/9/15.
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

TEST(System, EDMStream) {
  // [529, 999, 1270, 1624, 2001, 2435, 2648, 3000]
  // [3, 3, 4, 6, 6, 7, 9, 9]
  // Parse parameters.
  param_t cmd_params;
  cmd_params.num_points = 8000;
  cmd_params.dim = 54;
  cmd_params.alpha = 0.998;
  cmd_params.num_cache = 1000;
  cmd_params.radius = 250; // 220
  cmd_params.lambda = 1;
  cmd_params.delta = 1500;

  cmd_params.beta = 0.0021;
  cmd_params.opt = 2;
  // [529, 999, 1270, 1624, 2001, 2435, 2648, 3000]
  // [3, 3, 4, 6, 6, 7, 9, 9]
  cmd_params.num_clusters = 7;
  cmd_params.time_decay = true;

  cmd_params.input_file = "datasets/CoverType.txt";
  cmd_params.algo = SESAME::EDMStreamType;

  // Run algorithm producing results.
  RunBenchmark(cmd_params);
}