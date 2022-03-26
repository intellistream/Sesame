// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by wzru on 2022/3/24.
//

#include "Algorithm/DesignAspect/Generic.hpp"
#include "Algorithm/DataStructure/CFTree.hpp"
#include "Algorithm/DataStructure/GenericFactory.hpp"
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
  cmd_params.thresholdDistance = 1000;
  cmd_params.maxInternalNodes = 20;
  cmd_params.maxLeafNodes = 40;
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
  BenchmarkUtils::runBenchmark(cmd_params, sourcePtr, sinkPtr, algoPtr);
}