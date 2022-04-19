// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

/**
 * @brief This is the main entry point of the entire program.
 * Users will typically access this file to use the stream clustering algorithm.
 * We use this as the entry point for benchmarking.
 */
#include "Algorithm/AlgorithmFactory.hpp"
#include "Sinks/DataSinkFactory.hpp"
#include "Sources/DataSourceFactory.hpp"
#include "Utils/BenchmarkUtils.hpp"
#include "Utils/Logger.hpp"

#include <gflags/gflags.h>

using namespace std;

DEFINE_string(input_file, "dataset/CoverType.txt", "Input file path");
DEFINE_int32(num_points, 3000, "Number of points");
DEFINE_int32(dim, 5, "Dimension of points");
DEFINE_int32(num_clusters, 7, "Number of clusters");
// BIRCH
DEFINE_int32(max_in_nodes, 100, "Maximum number of internal nodes");
DEFINE_int32(max_leaf_nodes, 100, "Maximum number of leaf nodes");
DEFINE_double(distance_threshold, 100.0, "Distance threshold");
// StreamKM++
DEFINE_int32(seed, 1, "Seed for random number generator");
DEFINE_int32(coreset_size, 100, "Coreset size");
// EDMStream
DEFINE_double(radius, 50.0, "Radius");
DEFINE_double(delta, 100.0, "Delta");
DEFINE_double(beta, 0.1, "Beta");
DEFINE_int32(buf_size, 100, "Buffer size");
DEFINE_double(alpha, 0.998, "Alpha");
DEFINE_double(lambda, 1.0, "Lambda");
// DBStream
DEFINE_int32(clean_interval, 400, "Clean interval");
DEFINE_double(min_weight, 0.5, "Minimum weight");
DEFINE_double(base, 2, "Base");
// DStream
DEFINE_double(cm, 5.0, "Cm");
DEFINE_double(cl, 0.8, "Cl");
DEFINE_double(grid_width, 12.0, "Grid width");
// DenStream
DEFINE_int32(min_points, 10, "Minimum points");
DEFINE_double(epsilon, 20.0, "Epsilon");
DEFINE_double(mu, 2.0, "Mu");
// Clustream
DEFINE_int32(num_last_arr, 2, "Number of last arrive");
DEFINE_int32(time_window, 200, "Time window");
DEFINE_int32(time_interval, 100, "Time interval"); // random
DEFINE_int32(num_offline_clusters, 7, "Number of offline clusters");
DEFINE_int32(offline_time_window, 200, "Offline time window"); // random
// SL-KMeans
DEFINE_double(
    delta_grid, 0.2,
    "The delta parameter used int the grid for guessing the optimum.");
DEFINE_int32(num_samples, 100, "Number of samples");

int main(int argc, char **argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  // Setup Logs.
  setupLogging("benchmark.log", LOG_DEBUG);

  // Parse parameters.
  param_t cmd_params;
  BenchmarkUtils::defaultParam(cmd_params);
  cmd_params.num_points = 3000;
  cmd_params.seed = 10;
  cmd_params.num_clusters = 7;
  cmd_params.dim = 54;
  cmd_params.coreset_size = 600;
  cmd_params.num_last_arr = 5;
  cmd_params.time_window = 300;
  cmd_params.time_interval = 8;
  cmd_params.num_online_clusters = 150;
  cmd_params.radius = 20;
  cmd_params.buf_size = 20;
  cmd_params.offline_time_window = 0;
  cmd_params.max_in_nodes = 40;
  cmd_params.max_leaf_nodes = 20;
  cmd_params.distance_threshold = 10;
  cmd_params.true_num_clusters = 7;
  cmd_params.time_decay = false;

  cmd_params.output_file = "results.txt";
  cmd_params.algo = SESAME::Generic;
  BenchmarkUtils::parseArgs(argc, argv, cmd_params);
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
  SESAME::AlgorithmPtr algoPtr = SESAME::AlgorithmFactory::create(cmd_params);

  // Run algorithm producing results.
  BenchmarkUtils::runBenchmark(cmd_params, sourcePtr, sinkPtr, algoPtr);
}
