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

  //Parse parameters.
  param_t cmd_params;
  cmd_params.pointNumber = 3000;
  cmd_params.dimension = 54;
  cmd_params.a = 0.998;
  cmd_params.cacheNum = 500;
  cmd_params.radius = 250;
  cmd_params.lambda = 1;
  cmd_params.delta = 1500;
  cmd_params.beta = 0.0021;
  cmd_params.opt = 2;


  cmd_params.inputPath = std::filesystem::current_path().generic_string() + "/datasets/CoverType.txt";
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