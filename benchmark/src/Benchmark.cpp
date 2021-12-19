// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

/**
 * @brief This is the main entry point of the entire program.
 * Users will typically access this file to use the stream clustering algorithm.
 * We use this as the entry point for benchmarking.
 */
#include <Utils/BenchmarkUtils.hpp>
#include <Utils/Logger.hpp>
#include <Sources/DataSourceFactory.hpp>
#include <Sinks/DataSinkFactory.hpp>
#include <Algorithm/AlgorithmFactory.hpp>

using namespace std;

int main(int argc, char **argv) {
  //Setup Logs.
  setupLogging("benchmark.log", LOG_DEBUG);

  //Parse parameters.
  param_t cmd_params;
  BenchmarkUtils::defaultParam(cmd_params);
  cmd_params.pointNumber = 15120;
  cmd_params.seed = 10;
  cmd_params.clusterNumber = 10;
  cmd_params.dimension = 54;
  cmd_params.coresetSize = 100;
  cmd_params.lastArrivingNum = 60;
  cmd_params.timeWindow = 6;
  cmd_params.timeInterval = 4;
  cmd_params.onlineClusterNumber = 15;
  cmd_params.radiusFactor = 2;
  cmd_params.initBuffer = 500;
  cmd_params.offlineTimeWindow = 2;
  cmd_params.outputPath = "results.txt";
  cmd_params.algoType = SESAME::CluStreamType;
  BenchmarkUtils::parseArgs(argc, argv, cmd_params);
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

