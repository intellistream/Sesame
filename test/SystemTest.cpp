// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by shuhao zhang on 8/8/2021.
//

#include <gtest/gtest.h>
#include <Utils/BenchmarkUtils.hpp>
#include <Utils/Logger.hpp>
#include <Sources/DataSourceFactory.hpp>
#include <Sinks/DataSinkFactory.hpp>
#include <Algorithm/AlgorithmFactory.hpp>

TEST(SystemTest, SampleRun) {
  //Setup Logs.
  setupLogging("benchmark.log", LOG_DEBUG);

  //Parse parameters.
  param_t cmd_params;
  BenchmarkUtils::defaultParam(cmd_params);

  std::vector<SESAME::PointPtr> input;
  std::vector<SESAME::PointPtr> results;

  //Create Spout.
  SESAME::DataSourcePtr sourcePtr = SESAME::DataSourceFactory::create();
  //Directly load data from file. TODO: configure it to load from external sensors, e.g., HTTP.
  BenchmarkUtils::loadData(cmd_params, sourcePtr);

  //Create Sink.
  SESAME::DataSinkPtr sinkPtr = SESAME::DataSinkFactory::create();

  //Create Algorithm.
  SESAME::AlgorithmPtr algoPtr = SESAME::AlgorithmFactory::create(cmd_params.algoName,
                                                                  cmd_params.pointNumber,
                                                                  cmd_params.clusterNumber,
                                                                  cmd_params.dimension,
                                                                  cmd_params.coresetSize,
                                                                  cmd_params.seed,
                                                                  cmd_params.lastArrivingNum,
                                                                  cmd_params.timeWindow,
                                                                  cmd_params.timeInterval,
                                                                  cmd_params.onlineClusterNumber,
                                                                  cmd_params.radiusFactor,
                                                                  cmd_params.initBuffer,
                                                                  cmd_params.offlineTimeWindow);

  //Run algorithm producing results.
  BenchmarkUtils::runBenchmark(cmd_params, sourcePtr, sinkPtr, algoPtr);
}