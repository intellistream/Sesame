// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by tuidan on 2021/8/25.
//


#include <filesystem>
#include <gtest/gtest.h>
#include <Utils/BenchmarkUtils.hpp>
#include <Utils/Logger.hpp>
#include <Sources/DataSourceFactory.hpp>
#include <Sinks/DataSinkFactory.hpp>
#include <Algorithm/AlgorithmFactory.hpp>

TEST(SystemTest, BirchTest) {
  //Setup Logs.
  setupLogging("benchmark.log", LOG_DEBUG);

  //Parse parameters.
  param_t cmd_params;
  cmd_params.pointNumber = 15120;
  cmd_params.thresholdDistance = 7;
  cmd_params.maxInternalNodes = 3;
  cmd_params.maxLeafNodes = 3;
  cmd_params.dimension = 54;

  cmd_params.inputPath = std::filesystem::current_path().generic_string() + "/datasets/CoverType.txt";
  cmd_params.outputPath = "results.txt";
  cmd_params.algoType = SESAME::BirchType;

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