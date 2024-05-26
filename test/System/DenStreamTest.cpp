// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by Zhenyu on 2021/8/25.
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

TEST(System, DenStream) {
  // Parse parameters.

  // [529, 999, 1270, 1624, 2001, 2435, 2648, 3000]
  // [3, 3, 4, 6, 6, 7, 9, 9]
  param_t cmd_params;
  cmd_params.num_points = 3000;
  cmd_params.dim = 54;
  cmd_params.min_points = 10;
  cmd_params.epsilon = 20; // 0.1

  cmd_params.base = 2;
  cmd_params.lambda = 0.25;
  cmd_params.mu = 5;
  cmd_params.beta = 0.25;
  cmd_params.buf_size = 500;
  cmd_params.input_file = "datasets/CoverType.txt";

  cmd_params.output_file = "results.txt";
  cmd_params.algo = SESAME::DenStreamType;
  cmd_params.num_clusters = 7;
  cmd_params.time_decay = false;
  cmd_params.run_offline = true;

  // Run algorithm producing results.
  RunBenchmark(cmd_params);
}