// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by Zhenyu on 2021/9/18.
//


#include <filesystem>
#include <gtest/gtest.h>
#include <Utils/BenchmarkUtils.hpp>
#include <Utils/Logger.hpp>
#include <Sources/DataSourceFactory.hpp>
#include <Sinks/DataSinkFactory.hpp>
#include <Algorithm/AlgorithmFactory.hpp>

TEST(SystemTest, DBStreamTest) {
  //Setup Logs.
  setupLogging("benchmark.log", LOG_DEBUG);
  //Parse parameters.
  // [529, 999, 1270, 1624, 2001, 2435, 2648, 3000]
  // [3, 3, 4, 6, 6, 7, 9, 9]
  param_t cmd_params;
  cmd_params.pointNumber = 999;
  cmd_params.dimension = 2;
  cmd_params.base=2;
  cmd_params.lambda = 0.25;
  cmd_params.radius = 0.01;
  cmd_params.cleanUpInterval = 100;
  cmd_params.weightMin = 0.1;
  cmd_params.alpha = 0.4;
  cmd_params.inputPath = std::filesystem::current_path().generic_string() + "/datasets/D3.txt";
  cmd_params.outputPath = "results.txt";
  cmd_params.algoType = SESAME::DBStreamType;
  cmd_params.GTClusterNumber = 3;
  cmd_params.timeDecay = true;

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