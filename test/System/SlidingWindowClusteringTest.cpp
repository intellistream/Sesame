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

  cmd_params.input_file = std::filesystem::current_path().generic_string() +
                          "/datasets/CoverType.txt";
  cmd_params.output_file = "results.txt";
  cmd_params.algo = SESAME::SLKMeansType;
  cmd_params.run_offline = true;
  std::vector<SESAME::PointPtr> input;
  std::vector<SESAME::PointPtr> results;

  // Create Spout.
  SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(cmd_params);
  // Directly load data from file. TODO: configure it to load from external
  // sensors, e.g., HTTP.
  BenchmarkUtils::loadData(cmd_params, sourcePtr);

  // Create Sink.
  SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(cmd_params);

  // Create Algorithm.
  SESAME::AlgorithmPtr algoPtr =
      GenericFactory::New<SlidingWindowClustering>(cmd_params);

  // Run algorithm producing results.
  auto res =
      BenchmarkUtils::runBenchmark(cmd_params, sourcePtr, sinkPtr, algoPtr);

  ASSERT_NEAR(res->purity, 0.4703, 0.03);
  ASSERT_NEAR(res->cmm, 0.8247, 0.03);
}
