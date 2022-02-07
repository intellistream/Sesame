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
//[542, 872, 999, 1270, 1441,1795,2001,2435,2591,2804,3000]

  //Parse parameters.
  param_t cmd_params;
  cmd_params.pointNumber = 3000;
  cmd_params.dimension = 2;
  cmd_params.a = 0.998;
  cmd_params.cacheNum = 100;
  cmd_params.radius = 0.1;//220
  cmd_params.lambda = 1;
  cmd_params.delta = 10;
  cmd_params.beta = 0.001;
  cmd_params.opt = 2;


  cmd_params.inputPath = std::filesystem::current_path().generic_string() + "/datasets/Diamond/D12.txt";
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