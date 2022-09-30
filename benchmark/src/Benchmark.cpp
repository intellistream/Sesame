// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

/**
 * @brief This is the main entry point of the entire program.
 * Users will typically access this file to use the stream clustering algorithm.
 * We use this as the entry point for benchmarking.
 */
#include "Algorithm/AlgorithmFactory.hpp"
#include "Algorithm/DataStructure/GenericFactory.hpp"
#include "Sinks/DataSinkFactory.hpp"
#include "Sources/DataSourceFactory.hpp"
#include "Utils/BenchmarkUtils.hpp"
#include "Utils/Logger.hpp"

#include <gflags/gflags.h>

using namespace std;
using namespace SESAME;

DEFINE_int32(algo, 0, "Algorithm to use");
DEFINE_string(input_file, "datasets/CoverType.txt", "Input file path");
DEFINE_int32(num_points, 3000, "Number of points");
DEFINE_int32(dim, 54, "Dimension of points");
DEFINE_int32(num_clusters, 7, "Number of clusters");
// BIRCH
DEFINE_int32(max_in_nodes, 20, "Maximum number of internal nodes");
DEFINE_int32(max_leaf_nodes, 40, "Maximum number of leaf nodes");
DEFINE_double(distance_threshold, 10.0, "Distance threshold");
// StreamKM++
DEFINE_int32(seed, 1, "Seed for random number generator");
DEFINE_int32(coreset_size, 100, "Coreset size");
// EDMStream
DEFINE_double(radius, 50.0, "Radius");
DEFINE_double(delta, 100.0, "Delta");
DEFINE_double(beta, 0.1, "Beta");
DEFINE_int32(buf_size, 10000, "Buffer size");
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
DEFINE_int32(num_online_clusters, 80, "Number of online clusters");
// SL-KMeans
DEFINE_double(
    delta_grid, 0.2,
    "The delta parameter used int the grid for guessing the optimum.");
DEFINE_int32(num_samples, 100, "Number of samples");
// Generic
DEFINE_int32(landmark, 1000, "Landmark");
DEFINE_int32(sliding, 1000, "Sliding");
DEFINE_double(outlier_distance_threshold, 1000, "Outlier distance threshold");
DEFINE_int32(outlier_cap, 100, "Outlier cap");
DEFINE_double(outlier_density_threshold, 100, "Outlier density threshold");
DEFINE_double(neighbor_distance, 200, "Neighbor distance");
DEFINE_int32(k, 0, "KMeans K");
DEFINE_int32(arr_rate, 0, "Arrival rate");
DEFINE_bool(run_offline, true, "Whether run offline clustering");
DEFINE_bool(run_eval, true, "Whether run evaluation");
DEFINE_bool(run_cmm, true, "Whether run CMM evaluation");
DEFINE_bool(run_pur, true, "Whether run Purity evaluation");

int main(int argc, char **argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  // Parse parameters.
  param_t cmd_params;
  cmd_params.algo = (AlgoType)FLAGS_algo;
  BenchmarkUtils::defaultParam(cmd_params);
  // gen from gen_flags.js
  cmd_params.algo = (AlgoType)FLAGS_algo;
  cmd_params.input_file = FLAGS_input_file;
  cmd_params.num_points = FLAGS_num_points;
  cmd_params.dim = FLAGS_dim;
  cmd_params.num_clusters = FLAGS_num_clusters;
  cmd_params.max_in_nodes = FLAGS_max_in_nodes;
  cmd_params.max_leaf_nodes = FLAGS_max_leaf_nodes;
  cmd_params.distance_threshold = FLAGS_distance_threshold;
  cmd_params.seed = FLAGS_seed;
  cmd_params.coreset_size = FLAGS_coreset_size;
  cmd_params.radius = FLAGS_radius;
  cmd_params.delta = FLAGS_delta;
  cmd_params.beta = FLAGS_beta;
  cmd_params.buf_size = FLAGS_buf_size;
  cmd_params.alpha = FLAGS_alpha;
  cmd_params.lambda = FLAGS_lambda;
  cmd_params.clean_interval = FLAGS_clean_interval;
  cmd_params.min_weight = FLAGS_min_weight;
  cmd_params.base = FLAGS_base;
  cmd_params.cm = FLAGS_cm;
  cmd_params.cl = FLAGS_cl;
  cmd_params.grid_width = FLAGS_grid_width;
  cmd_params.min_points = FLAGS_min_points;
  cmd_params.epsilon = FLAGS_epsilon;
  cmd_params.mu = FLAGS_mu;
  cmd_params.num_last_arr = FLAGS_num_last_arr;
  cmd_params.time_window = FLAGS_time_window;
  cmd_params.num_online_clusters = FLAGS_num_online_clusters;
  cmd_params.delta_grid = FLAGS_delta_grid;
  cmd_params.num_samples = FLAGS_num_samples;
  cmd_params.landmark = FLAGS_landmark;
  cmd_params.sliding = FLAGS_sliding;
  cmd_params.outlier_distance_threshold = FLAGS_outlier_distance_threshold;
  cmd_params.outlier_cap = FLAGS_outlier_cap;
  cmd_params.outlier_density_threshold = FLAGS_outlier_density_threshold;
  cmd_params.neighbor_distance = FLAGS_neighbor_distance;
  cmd_params.k = FLAGS_k;
  cmd_params.arr_rate = FLAGS_arr_rate;
  cmd_params.run_offline = FLAGS_run_offline;
  cmd_params.run_eval = FLAGS_run_eval;
  cmd_params.run_cmm = FLAGS_run_cmm;
  cmd_params.run_pur = FLAGS_run_pur;

  cmd_params.output_file = "results.txt";
  cmd_params.fast_source = true;
  cmd_params.store = false;

  std::vector<SESAME::PointPtr> input;
  std::vector<SESAME::PointPtr> results;

  // Create Spout.
  SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(cmd_params);

  // Directly load data from file. TODO: configure it to load from external
  // sensors, e.g., HTTP.
  BenchmarkUtils::loadData(cmd_params, sourcePtr);

  // Create Sink.
  SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(cmd_params);

  // Create Algorithm.
  SESAME::AlgorithmPtr algoPtr = SESAME::AlgorithmFactory::create(cmd_params);

  cmd_params.Print();

  // Run algorithm producing results.
  auto res =
      BenchmarkUtils::runBenchmark(cmd_params, sourcePtr, sinkPtr, algoPtr);

  res->Print();
}
