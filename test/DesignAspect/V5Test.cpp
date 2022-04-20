// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by tuidan on 2021/8/25.
//

#include "Algorithm/AlgorithmFactory.hpp"
#include "Sinks/DataSinkFactory.hpp"
#include "Sources/DataSourceFactory.hpp"
#include "Utils/BenchmarkUtils.hpp"
#include "Utils/Logger.hpp"

#include "gtest/gtest.h"

#include <filesystem>

TEST(DesignTest, V5) {
  // Setup Logs.
  setupLogging("benchmark.log", LOG_DEBUG);
  // Parse parameters.
  param_t cmd_params;
  cmd_params.num_points = 3000;
  cmd_params.distance_threshold = 1000;
  cmd_params.max_in_nodes = 20;
  cmd_params.max_leaf_nodes = 40;
  cmd_params.dim = 54;
  cmd_params.num_clusters = 7;
  cmd_params.time_decay = false;
  cmd_params.lambda = 1.25;
  cmd_params.alpha = 2;
  cmd_params.outlier_distance_threshold = 5000;
  cmd_params.outlier_cap = 10;

  cmd_params.input_file = std::filesystem::current_path().generic_string() +
                         "/datasets/CoverType.txt";
  cmd_params.output_file = "results.txt";
  cmd_params.algo = SESAME::V5Stream;

  std::vector<SESAME::PointPtr> input;
  std::vector<SESAME::PointPtr> results;

  // Create Spout.
  SESAME::DataSourcePtr sourcePtr = SESAME::DataSourceFactory::create();
  // Directly load data from file. TODO: configure it to load from external
  // sensors, e.g., HTTP.
  BenchmarkUtils::loadData(cmd_params, sourcePtr);

  // Create Sink.
  SESAME::DataSinkPtr sinkPtr = SESAME::DataSinkFactory::create();

  // Create Algorithm.
  SESAME::AlgorithmPtr algoPtr = SESAME::AlgorithmFactory::create(cmd_params);

  // Run algorithm producing results.
  auto res =
      BenchmarkUtils::runBenchmark(cmd_params, sourcePtr, sinkPtr, algoPtr);

  ASSERT_NEAR(res->purity, 0.2987, 0.01);
}