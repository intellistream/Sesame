// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by Zhenyu on 2021/8/25.
//


#include <filesystem>
#include <gtest/gtest.h>
#include <Utils/BenchmarkUtils.hpp>
#include <Utils/Logger.hpp>
#include <Sources/DataSourceFactory.hpp>
#include <Sinks/DataSinkFactory.hpp>
#include <Algorithm/AlgorithmFactory.hpp>

TEST(SystemTest, DenStreamTest) {
  //Setup Logs.
  setupLogging("benchmark.log", LOG_DEBUG);
  //Parse parameters.
  param_t cmd_params;
  cmd_params.pointNumber = 15120;
  cmd_params.dimension = 54;
  cmd_params.minPoints=40;
  cmd_params.epsilon=30;
  cmd_params.base=2;
  cmd_params.lambda= 0.3;
  cmd_params.mu=5;
  cmd_params.beta=0.25;
  cmd_params.initBuffer = 1000;
  cmd_params.inputPath = std::filesystem::current_path().generic_string() + "/datasets/encoded_data.txt";
  cmd_params.outputPath = "results.txt";
  cmd_params.algoType = SESAME::DenStreamType;

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