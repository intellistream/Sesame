// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by tuidan on 2021/8/25.
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

TEST(System, Birch) {
  // [529, 999, 1270, 1624, 2001, 2435, 2648, 3000]
  // [3, 3, 4, 6, 6, 7, 9, 9]
  // Parse parameters.
  param_t param;
  param.num_points = 10000;
  param.max_in_nodes = 400;
  param.max_leaf_nodes = 100;
  param.distance_threshold = 600;
  param.dim = 54;
  param.num_clusters = 7;
  param.landmark = 1000;
  param.run_cmm = false;

  param.input_file = "datasets/CoverType.txt";
  param.algo = SESAME::BirchType;

  // Run algorithm producing results.
  auto res = SESAME::RunBenchmark(param);

  ASSERT_NEAR(res.first.purity, 0.3749, 0.02);
}
