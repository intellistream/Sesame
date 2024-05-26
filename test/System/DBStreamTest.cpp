// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by Zhenyu on 2021/9/18.
//

#include "Algorithm/AlgorithmFactory.hpp"
#include "Algorithm/DataStructure/GenericFactory.hpp"
#include "Sinks/DataSinkFactory.hpp"
#include "Sources/DataSourceFactory.hpp"
#include "Utils/BenchmarkUtils.hpp"
#include "Utils/Logger.hpp"

#include <filesystem>

#include <gtest/gtest.h>

using namespace SESAME;

TEST(System, DBStream) {
  // Parse parameters.
  // [529, 999, 1270, 1624, 2001, 2435, 2648, 3000]
  // [3, 3, 4, 6, 6, 7, 9, 9]
  param_t cmd_params;
  cmd_params.num_points = 581012;
  cmd_params.dim = 2;
  cmd_params.base = 2;
  cmd_params.lambda = 0.998;
  cmd_params.radius = 20;
  cmd_params.clean_interval = 400;
  cmd_params.min_weight = 0.5;
  cmd_params.alpha = 0.2;
  cmd_params.input_file = "datasets/CoverType.txt";
  cmd_params.algo = SESAME::DBStreamType;
  cmd_params.num_clusters = 363;
  cmd_params.time_decay = false;
  cmd_params.store = false;

  auto res = SESAME::RunBenchmark(cmd_params);
}