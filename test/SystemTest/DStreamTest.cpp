//
// Created by 1124a on 2021/11/2.
//


#include <filesystem>
#include <gtest/gtest.h>
#include <Utils/BenchmarkUtils.hpp>
#include <Utils/Logger.hpp>
#include <Sources/DataSourceFactory.hpp>
#include <Sinks/DataSinkFactory.hpp>
#include <Algorithm/AlgorithmFactory.hpp>

TEST(SystemTest, DStreamTest) {
  //Setup Logs.
  setupLogging("benchmark.log", LOG_DEBUG);
  //Parse parameters.
  // [529, 999, 1270, 1624, 2001, 2435, 2648, 3000]
  // [3, 3, 4, 6, 6, 7, 9, 9]
  param_t cmd_params;
  cmd_params.num_points = 3000;
  cmd_params.dim = 54;
  cmd_params.lambda= 0.998;
  cmd_params.beta=0.001;
  cmd_params.cm = 15;
  cmd_params.cl = 0.001;
  cmd_params.grid_width= 13;
  cmd_params.num_clusters = 7;
  cmd_params.time_decay = false;

  cmd_params.input_file = std::filesystem::current_path().generic_string() + "/datasets/CoverType.txt";
  cmd_params.output_file = "results.txt";
  cmd_params.algo = SESAME::DStreamType;

  std::vector<SESAME::PointPtr> input;
  std::vector<SESAME::PointPtr> results;

  //Create Spout.
  SESAME::DataSourcePtr sourcePtr = SESAME::DataSourceFactory::create();
  //Directly load data from file. TODO: configure it to load from external sensors, e.g., HTTP.
  BenchmarkUtils::loadData(cmd_params, sourcePtr);

  //Create Sink.
  SESAME::DataSinkPtr sinkPtr = SESAME::DataSinkFactory::create();

  //Create Algorithm.
  SESAME::AlgorithmPtr algoPtr = SESAME::AlgorithmFactory::create(cmd_params);

  //Run algorithm producing results.
  BenchmarkUtils::runBenchmark(cmd_params, sourcePtr, sinkPtr, algoPtr);
}