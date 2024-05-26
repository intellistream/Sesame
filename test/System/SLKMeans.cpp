// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by wzru on 2022/4/10.
//

#include <filesystem>

#include "Algorithm/AlgorithmFactory.hpp"
#include "Algorithm/DataStructure/GenericFactory.hpp"
#include "Algorithm/SlidingWindowClustering.hpp"
#include "Sinks/DataSinkFactory.hpp"
#include "Sources/DataSourceFactory.hpp"
#include "Utils/BenchmarkUtils.hpp"
#include "Utils/Logger.hpp"
#include "gtest/gtest.h"

using namespace SESAME;

TEST(System, SLKMeans) {
  // Parse parameters.
  param_t cmd_params;
  cmd_params.num_points = 3000;
  cmd_params.num_clusters = 7;
  cmd_params.dim = 54;
  cmd_params.num_clusters = 7;
  cmd_params.time_decay = false;
  cmd_params.sliding = 100;
  cmd_params.delta_grid = 0.2;
  cmd_params.num_samples = 1;
  cmd_params.outlier_distance_threshold = 5000;
  cmd_params.outlier_cap = 10;
  cmd_params.seed = 10;

  cmd_params.input_file = "datasets/CoverType.txt";
  cmd_params.algo = SESAME::SLKMeansType;
  cmd_params.run_offline = true;
  cmd_params.run_cmm = true;

  // Run algorithm producing results.
  auto res = SESAME::RunBenchmark(cmd_params);

  ASSERT_NEAR(res.first.purity, 0.47, 0.03);
  ASSERT_NEAR(res.first.cmm, 0.80, 0.03);
}
