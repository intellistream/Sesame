// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by wzru on 2022/3/24.
//

#include <filesystem>

#include "Algorithm/AlgorithmFactory.hpp"
#include "Algorithm/DataStructure/CFTree.hpp"
#include "Algorithm/DataStructure/GenericFactory.hpp"
#include "Algorithm/DataStructure/MeyersonSketch.hpp"
#include "Algorithm/DesignAspect/Generic.hpp"
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

using namespace SESAME;
using namespace std;

TEST(System, V1) {
  // Parse parameters.
  param_t param;
  param.num_points = 3000;
  param.distance_threshold = 100;
  param.max_in_nodes = 10;
  param.max_leaf_nodes = 20;
  param.num_clusters = 7;
  param.dim = 54;
  param.seed = 10;
  param.num_clusters = 7;
  param.time_decay = false;
  param.landmark = 1000;
  param.outlier_distance_threshold = 5000;
  param.outlier_cap = 10;
  param.k = 100;

  param.input_file = "datasets/CoverType.txt";
  param.algo = G1Stream;
  param.run_offline = true;

  // Run algorithm producing results.
  auto res = SESAME::RunBenchmark(param);

  ASSERT_NEAR(res.first.purity, 0.5453, 0.02);
}

TEST(System, V2) {
  // Parse parameters.
  param_t param;
  param.num_points = 3000;
  param.distance_threshold = 100;
  param.max_in_nodes = 10;
  param.max_leaf_nodes = 20;
  param.dim = 54;
  param.num_clusters = 7;
  param.time_decay = false;
  param.min_points = 10;
  param.landmark = 1000;
  param.epsilon = 200; // 0.1
  param.run_offline = true;
  param.outlier_distance_threshold = 5000;
  param.outlier_cap = 10;

  param.input_file = "datasets/CoverType.txt";
  param.algo = SESAME::G2Stream;

  // Run algorithm producing results.
  auto res = SESAME::RunBenchmark(param);

  ASSERT_NEAR(res.first.purity, 0.5453, 0.02);
}

TEST(System, V3) {
  // Parse parameters.
  param_t param;
  param.num_points = 45690;
  param.distance_threshold = 50;
  param.max_in_nodes = 100;
  param.max_leaf_nodes = 100;
  param.dim = 2;
  param.num_clusters = 75;
  param.time_decay = true;
  param.landmark = 1000;
  param.outlier_distance_threshold = 500;
  param.outlier_cap = 2000;

  param.input_file = "datasets/EDS.txt";
  param.algo = SESAME::G3Stream;

  // Run algorithm producing results.
  auto res = SESAME::RunBenchmark(param);

  ASSERT_NEAR(res.first.purity, 0.8032, 0.02);
}

TEST(System, V4) {
  // Parse parameters.
  param_t param;
  param.num_points = 3000;
  param.distance_threshold = 100;
  param.max_in_nodes = 20;
  param.max_leaf_nodes = 40;
  param.dim = 54;
  param.num_clusters = 7;
  param.time_decay = false;
  param.sliding = 1000;
  param.outlier_distance_threshold = 3000;
  param.outlier_cap = 3;

  param.input_file = "datasets/CoverType.txt";
  param.algo = SESAME::G4Stream;

  // Run algorithm producing results.
  auto res = SESAME::RunBenchmark(param);

  ASSERT_NEAR(res.first.purity, 0.5413, 0.02);
}

TEST(System, V5) {
  // Parse parameters.
  param_t param;
  param.num_points = 3000;
  param.distance_threshold = 1000;
  param.max_in_nodes = 20;
  param.max_leaf_nodes = 40;
  param.dim = 54;
  param.num_clusters = 7;
  param.time_decay = false;
  param.lambda = 1.25;
  param.alpha = 2;
  param.outlier_distance_threshold = 500;
  param.outlier_cap = 100;

  param.input_file = "datasets/CoverType.txt";
  param.algo = SESAME::G5Stream;

  // Run algorithm producing results.
  auto res = SESAME::RunBenchmark(param);

  ASSERT_NEAR(res.first.purity, 0.2987, 0.02);
}

// TEST(System, V6)
// {
//     // Parse parameters.
//     param_t param;

//     param.num_points         = 10000;
//     param.distance_threshold = 10;
//     param.max_in_nodes       = 100;
//     param.max_leaf_nodes     = 50;
//     param.dim                = 2;
//     param.num_clusters       = 363;
//     param.time_decay         = false;
//     param.landmark           = 1000;

//     param.input_file  = "datasets/EDS.txt";

//     param.algo        = SESAME::G6Stream;

//     // Run algorithm producing results.
//     auto res = SESAME::RunBenchmark(param);

//     ASSERT_NEAR(res.first.purity, 0.8251, 0.02);
// }

TEST(System, V7) {
  // Parse parameters.
  param_t param;
  param.num_points = 3000;
  param.distance_threshold = 1000;
  param.max_in_nodes = 20;
  param.max_leaf_nodes = 40;
  param.dim = 54;
  param.num_clusters = 7;
  param.time_decay = false;
  param.landmark = 1000;
  param.neighbor_distance = 5000;
  param.outlier_density_threshold = 0.5;
  param.outlier_cap = 10;

  param.input_file = "datasets/CoverType.txt";
  param.algo = SESAME::G7Stream;

  // Run algorithm producing results.
  auto res = SESAME::RunBenchmark(param);

  ASSERT_NEAR(res.first.purity, 0.432, 0.02);
}

TEST(System, V8) {
  // Parse parameters.
  param_t param;
  param.num_points = 45690;
  param.distance_threshold = 50;
  param.max_in_nodes = 100;
  param.max_leaf_nodes = 100;
  param.dim = 2;
  param.num_clusters = 75;
  param.time_decay = true;
  param.landmark = 1000;
  param.run_offline = false;
  param.outlier_distance_threshold = 600;
  param.outlier_cap = 200;

  param.input_file = "datasets/EDS.txt";
  param.algo = SESAME::G8Stream;

  // Run algorithm producing results.
  auto res = SESAME::RunBenchmark(param);

  // ASSERT_NEAR(res.first.purity, 0.7982, 0.01);
}

TEST(System, V9) {
  // Parse parameters.
  param_t cmd_params;
  cmd_params.num_points = 3000;
  cmd_params.dim = 54;
  cmd_params.lambda = 0.998;
  cmd_params.beta = 0.001;
  cmd_params.cm = 3;
  cmd_params.cl = 0.8;
  cmd_params.grid_width = 50;
  cmd_params.num_clusters = 7;
  cmd_params.time_decay = false;
  cmd_params.landmark = 3000;

  cmd_params.input_file = "datasets/CoverType.txt";
  cmd_params.output_file = "results.txt";
  cmd_params.algo = SESAME::G9Stream;

  // Run algorithm producing results.
  auto res = SESAME::RunBenchmark(cmd_params);
}

TEST(System, V10) {
  // [529, 999, 1270, 1624, 2001, 2435, 2648, 3000]
  // [3, 3, 4, 6, 6, 7, 9, 9]
  // Parse parameters.
  param_t cmd_params;
  cmd_params.num_points = 8000;
  cmd_params.dim = 54;
  cmd_params.alpha = 0.998;
  cmd_params.landmark = 1000;
  cmd_params.num_cache = 1000;
  cmd_params.radius = 250; // 220
  cmd_params.delta = 1500;
  cmd_params.beta = 0.0021;
  cmd_params.opt = 2;
  cmd_params.num_clusters = 7;
  cmd_params.time_decay = true;

  cmd_params.input_file = "datasets/CoverType.txt";
  cmd_params.output_file = "results.txt";
  cmd_params.algo = SESAME::G10Stream;

  // Run algorithm producing results.
  RunBenchmark(cmd_params);
}

TEST(System, V11) {
  // Parse parameters.
  param_t param;

  param.num_points = 10000;
  param.distance_threshold = 10;
  param.max_in_nodes = 100;
  param.max_leaf_nodes = 50;
  param.dim = 2;
  param.num_clusters = 363;
  param.time_decay = false;
  param.landmark = 1000;
  param.clean_interval = 9900;

  param.input_file = "datasets/EDS.txt";
  param.algo = SESAME::G11Stream;

  // Run algorithm producing results.
  auto res = SESAME::RunBenchmark(param);

  // ASSERT_NEAR(res.first.purity, 0.4761, 0.01);
}

TEST(System, V12) {
  // Parse parameters.
  param_t param;

  param.num_points = 10000;
  param.distance_threshold = 10;
  param.max_in_nodes = 100;
  param.max_leaf_nodes = 50;
  param.dim = 2;
  param.num_clusters = 363;
  param.time_decay = false;
  param.landmark = 1000;

  param.input_file = "datasets/EDS.txt";
  param.algo = SESAME::G12Stream;

  auto res = SESAME::RunBenchmark(param);

  // ASSERT_NEAR(res.first.purity, 0.8228, 0.02);
}

TEST(System, V13) {
  // Parse parameters.
  param_t param;

  param.num_points = 10000;
  param.distance_threshold = 10;
  param.max_in_nodes = 100;
  param.max_leaf_nodes = 50;
  param.dim = 2;
  param.num_clusters = 363;
  param.time_decay = false;
  param.landmark = 1000;
  param.clean_interval = 5000;

  param.input_file = "datasets/EDS.txt";
  param.algo = SESAME::G13Stream;

  // Run algorithm producing results.
  auto res = SESAME::RunBenchmark(param);

  // ASSERT_NEAR(res.first.purity, 0.8344, 0.01);
}

TEST(System, V14) {
  // Parse parameters.
  param_t param;
  param.num_points = 3000;
  param.num_clusters = 7;
  param.dim = 54;
  param.num_clusters = 7;
  param.k = 100;
  param.seed = 10;
  param.num_samples = 100;
  param.sliding = 5;

  param.input_file = "datasets/CoverType.txt";
  param.algo = SESAME::G14Stream;

  // Run algorithm producing results.
  auto res = SESAME::RunBenchmark(param);

  ASSERT_NEAR(res.first.purity, 0.701, 0.01);
}

TEST(System, V16) {
  // Parse parameters.
  param_t cmd_params;
  cmd_params.num_points = 3000;
  cmd_params.dim = 54;
  cmd_params.lambda = 0.998;
  cmd_params.beta = 0.001;
  cmd_params.cm = 3;
  cmd_params.cl = 0.8;
  cmd_params.grid_width = 50;
  cmd_params.num_clusters = 7;
  cmd_params.time_decay = false;
  cmd_params.sliding = 1000;

  cmd_params.input_file = "datasets/CoverType.txt";
  cmd_params.algo = SESAME::G16Stream;

  // Run algorithm producing results.
  auto res = SESAME::RunBenchmark(cmd_params);
}
