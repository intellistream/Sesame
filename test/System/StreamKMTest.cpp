// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by shuhao zhang on 8/8/2021.
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

TEST(System, StreamKM) {
  // [529, 999, 1270, 1624, 2001, 2435, 2648, 3000]
  // [3, 3, 4, 6, 6, 7, 9, 9]
  // Parse parameters.
  param_t cmd_params;
  cmd_params.num_points = 3000;
  cmd_params.seed = 10;
  cmd_params.num_clusters = 7;
  cmd_params.dim = 54;
  cmd_params.coreset_size = 600;
  cmd_params.num_clusters = 7;
  cmd_params.k = 7;
  cmd_params.time_decay = false;

  cmd_params.input_file = "datasets/CoverType.txt";
  cmd_params.algo = SESAME::StreamKMeansType;
  cmd_params.run_offline = true;

  // Run algorithm producing results.
  RunBenchmark(cmd_params);
}