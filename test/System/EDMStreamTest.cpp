//
// Created by tuidan on 2021/9/15.
//

#include "Algorithm/AlgorithmFactory.hpp"
#include "Algorithm/DataStructure/GenericFactory.hpp"
#include "Sinks/DataSinkFactory.hpp"
#include "Sources/DataSourceFactory.hpp"
#include "Utils/BenchmarkUtils.hpp"
#include "Utils/Logger.hpp"

#include <filesystem>

#include <gtest/gtest.h>

TEST(SystemTest, EDMStream) {
  // Setup Logs.
  setupLogging("benchmark.log", LOG_DEBUG);
  // [529, 999, 1270, 1624, 2001, 2435, 2648, 3000]
  // [3, 3, 4, 6, 6, 7, 9, 9]
  // Parse parameters.
  param_t cmd_params;
  cmd_params.num_points = 45690;
  cmd_params.dim = 2;
  cmd_params.alpha = 0.998;
  cmd_params.num_cache = 10000;
  cmd_params.radius = 30; // 220
  cmd_params.lambda = 1;
  cmd_params.delta = 300;

  cmd_params.beta = 1;
//  cmd_params.opt = 2;
  // [529, 999, 1270, 1624, 2001, 2435, 2648, 3000]
  // [3, 3, 4, 6, 6, 7, 9, 9]
  cmd_params.num_clusters = 75;
  cmd_params.time_decay = false;

  cmd_params.input_file =
      std::filesystem::current_path().generic_string() + "/datasets/EDS.txt";
  cmd_params.output_file = "results.txt";
  cmd_params.algo = SESAME::EDMStreamType;

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
  SESAME::AlgorithmPtr algoPtr = SESAME::AlgorithmFactory::create(cmd_params);

  // Run algorithm producing results.
  auto res = BenchmarkUtils::runBenchmark(cmd_params, sourcePtr, sinkPtr, algoPtr);
}