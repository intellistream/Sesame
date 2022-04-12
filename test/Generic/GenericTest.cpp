// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by wzru on 2022/3/24.
//

#include "Algorithm/DesignAspect/Generic.hpp"
#include "Algorithm/DataStructure/CFTree.hpp"
#include "Algorithm/DataStructure/GenericFactory.hpp"
#include "Algorithm/OfflineRefinement/DBSCAN.hpp"
#include "Algorithm/OfflineRefinement/KMeans.hpp"
#include "Algorithm/OfflineRefinement/OfflineRefinement.hpp"
#include "Algorithm/OutlierDetection/OutlierDetection.hpp"
#include "Algorithm/WindowModel/WindowModel.hpp"
#include "Sinks/DataSinkFactory.hpp"
#include "Sources/DataSourceFactory.hpp"
#include "Utils/BenchmarkUtils.hpp"
#include "Utils/Logger.hpp"

#include "gtest/gtest.h"

#include <filesystem>

using namespace SESAME;

TEST(GenericTest, V1) {
  // Setup Logs.
  setupLogging("benchmark.log", LOG_DEBUG);
  // Parse parameters.
  param_t cmd_params;
  cmd_params.pointNumber = 3000;
  cmd_params.thresholdDistance = 100;
  cmd_params.maxInternalNodes = 10;
  cmd_params.maxLeafNodes = 20;
  cmd_params.clusterNumber = 7;
  cmd_params.dimension = 54;
  cmd_params.GTClusterNumber = 7;
  cmd_params.timeDecay = false;
  cmd_params.landmark = 1000;
  cmd_params.outlierDistanceThreshold = 5000;
  cmd_params.outlierClusterCapacity = 10;

  cmd_params.inputPath = std::filesystem::current_path().generic_string() +
                         "/datasets/CoverType.txt";
  cmd_params.outputPath = "results.txt";
  cmd_params.algoType = Generic;
  cmd_params.executeOffline = true;

  std::vector<PointPtr> input;
  std::vector<PointPtr> results;

  // Create Spout.
  DataSourcePtr sourcePtr = DataSourceFactory::create();
  // Directly load data from file. TODO: configure it to load from external
  // sensors, e.g., HTTP.
  BenchmarkUtils::loadData(cmd_params, sourcePtr);

  // Create Sink.
  DataSinkPtr sinkPtr = DataSinkFactory::create();

  // Create Algorithm.
  AlgorithmPtr algoPtr = GenericFactory::New<StreamClustering<
      Landmark, ClusteringFeaturesTree, DistanceDetection, KMeans>>(cmd_params);

  // Run algorithm producing results.
  auto res =
      BenchmarkUtils::runBenchmark(cmd_params, sourcePtr, sinkPtr, algoPtr);

  ASSERT_NEAR(res->purity, 0.403, 0.001);
}

TEST(GenericTest, V2) {
  // Setup Logs.
  setupLogging("benchmark.log", LOG_DEBUG);
  // Parse parameters.
  param_t cmd_params;
  cmd_params.pointNumber = 3000;
  cmd_params.thresholdDistance = 1000;
  cmd_params.maxInternalNodes = 10;
  cmd_params.maxLeafNodes = 20;
  cmd_params.dimension = 54;
  cmd_params.GTClusterNumber = 7;
  cmd_params.timeDecay = false;
  cmd_params.minPoints = 10;
  cmd_params.landmark = 1000;
  cmd_params.epsilon = 200; // 0.1
  cmd_params.executeOffline = true;
  cmd_params.outlierDistanceThreshold = 5000;
  cmd_params.outlierClusterCapacity = 10;

  cmd_params.inputPath = std::filesystem::current_path().generic_string() +
                         "/datasets/CoverType.txt";
  cmd_params.outputPath = "results.txt";
  cmd_params.algoType = SESAME::Generic;

  std::vector<SESAME::PointPtr> input;
  std::vector<SESAME::PointPtr> results;

  // Create Spout.
  SESAME::DataSourcePtr sourcePtr = SESAME::DataSourceFactory::create();
  // Directly load data from file. TODO: configure it to load from external
  // sensors, e.g., HTTP.
  BenchmarkUtils::loadData(cmd_params, sourcePtr);

  // Create Sink.
  SESAME::DataSinkPtr sinkPtr = SESAME::DataSinkFactory::create();

  // Create Algorithm.
  AlgorithmPtr algoPtr = GenericFactory::New<StreamClustering<
      Landmark, ClusteringFeaturesTree, DistanceDetection, DBSCAN>>(cmd_params);

  // Run algorithm producing results.
  auto res =
      BenchmarkUtils::runBenchmark(cmd_params, sourcePtr, sinkPtr, algoPtr);

  ASSERT_NEAR(res->purity, 0.403, 0.001);
}