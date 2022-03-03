//
// Created by tuidan on 2021/9/15.
//

#include <filesystem>
#include <gtest/gtest.h>
#include <Utils/BenchmarkUtils.hpp>
#include <Utils/Logger.hpp>
#include <Sources/DataSourceFactory.hpp>
#include <Sinks/DataSinkFactory.hpp>
#include <Algorithm/AlgorithmFactory.hpp>

TEST(SystemTest, EDMStreamTest) {
  //Setup Logs.
  setupLogging("benchmark.log", LOG_DEBUG);
  // [529, 999, 1270, 1624, 2001, 2435, 2648, 3000]
  // [3, 3, 4, 6, 6, 7, 9, 9]
  //Parse parameters.
  param_t cmd_params;
  cmd_params.pointNumber = 5000;
  cmd_params.dimension = 5;
  cmd_params.a = 0.998;
  cmd_params.cacheNum = 100;
  cmd_params.radius = 0.1;//220
  cmd_params.lambda = 1;
  cmd_params.delta = 10;

  cmd_params.beta = 0.001;
  cmd_params.opt = 2;
  // [529, 999, 1270, 1624, 2001, 2435, 2648, 3000]
  // [3, 3, 4, 6, 6, 7, 9, 9]
  cmd_params.GTClusterNumber = 7;
  cmd_params.timeDecay = true;


  cmd_params.inputPath = std::filesystem::current_path().generic_string() + "/datasets/sensor.txt";
  cmd_params.outputPath = "results.txt";
  cmd_params.algoType = SESAME::EDMStreamType;

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