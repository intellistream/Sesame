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

#include <filesystem>

using namespace std;
using namespace std::filesystem;
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
DEFINE_int32(clean_interval, 2500, "Clean interval");
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
DEFINE_int32(time_window, 50, "Time window");
DEFINE_int32(num_online_clusters, 80, "Number of online clusters");
// SL-KMeans
DEFINE_double(
    delta_grid, 0.2,
    "The delta parameter used int the grid for guessing the optimum.");
DEFINE_int32(num_samples, 10, "Number of samples");
// Generic
DEFINE_int32(landmark, 10000, "Landmark");
DEFINE_int32(sliding, 500, "Sliding");
DEFINE_double(outlier_distance_threshold, 1000, "Outlier distance threshold");
DEFINE_int32(outlier_cap, 100, "Outlier cap");
DEFINE_double(outlier_density_threshold, 100, "Outlier density threshold");
DEFINE_double(neighbor_distance, 200, "Neighbor distance");
DEFINE_int32(k, 7, "KMeans K");
DEFINE_int32(arr_rate, 0, "Arrival rate");
DEFINE_bool(run_offline, true, "Whether run offline clustering");
DEFINE_bool(run_eval, true, "Whether run evaluation");
DEFINE_bool(run_cmm, false, "Whether run CMM evaluation");
DEFINE_bool(run_pur, true, "Whether run Purity evaluation");
// Benne
DEFINE_int32(obj, 0, "Objective: 0(balance), 1(accuracy), 2(efficiency)");
DEFINE_int32(queue_size_threshold, 10000, "Benne queue size threshold");
DEFINE_int32(dim_threshold, 30, "Benne dimension threshold");
DEFINE_double(variance_threshold, 100.0, "Benne variance threshold");
DEFINE_int32(outliers_num_threshold, 200, "Benne outliers threshold");
DEFINE_double(outliers_dist_threshold, 50.0,
              "Benne outliers distance threshold");

int main(int argc, char **argv) {
#ifndef NDEBUG
  std::cerr << "\033[1;31m#####################################################"
               "#######\n"
            << "#                                                          #\n"
            << "#           DON'T run benchmark in debug mode.             #\n"
            << "#                                                          #\n"
            << "############################################################"
               "\033[0m\n";
  sleep(1);
#endif
  // Parse parameters.
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  param_t param;
  param.algo = (AlgoType)FLAGS_algo;
  param.algo = (AlgoType)FLAGS_algo;
  param.input_file = FLAGS_input_file;
  param.num_points = FLAGS_num_points;
  param.dim = FLAGS_dim;
  param.num_clusters = FLAGS_num_clusters;
  param.max_in_nodes = FLAGS_max_in_nodes;
  param.max_leaf_nodes = FLAGS_max_leaf_nodes;
  param.distance_threshold = FLAGS_distance_threshold;
  param.seed = FLAGS_seed;
  param.coreset_size = FLAGS_coreset_size;
  param.radius = FLAGS_radius;
  param.delta = FLAGS_delta;
  param.beta = FLAGS_beta;
  param.buf_size = FLAGS_buf_size;
  param.alpha = FLAGS_alpha;
  param.lambda = FLAGS_lambda;
  param.clean_interval = FLAGS_clean_interval;
  param.min_weight = FLAGS_min_weight;
  param.base = FLAGS_base;
  param.cm = FLAGS_cm;
  param.cl = FLAGS_cl;
  param.grid_width = FLAGS_grid_width;
  param.min_points = FLAGS_min_points;
  param.epsilon = FLAGS_epsilon;
  param.mu = FLAGS_mu;
  param.num_last_arr = FLAGS_num_last_arr;
  param.time_window = FLAGS_time_window;
  param.num_online_clusters = FLAGS_num_online_clusters;
  param.delta_grid = FLAGS_delta_grid;
  param.num_samples = FLAGS_num_samples;
  param.landmark = FLAGS_landmark;
  param.sliding = FLAGS_sliding;
  param.outlier_distance_threshold = FLAGS_outlier_distance_threshold;
  param.outlier_cap = FLAGS_outlier_cap;
  param.outlier_density_threshold = FLAGS_outlier_density_threshold;
  param.neighbor_distance = FLAGS_neighbor_distance;
  param.k = FLAGS_k;
  param.arr_rate = FLAGS_arr_rate;
  param.run_offline = FLAGS_run_offline;
  param.run_eval = FLAGS_run_eval;
  param.run_cmm = FLAGS_run_cmm;
  param.run_pur = FLAGS_run_pur;
  param.obj = (BenneObj)FLAGS_obj;
  param.benne_threshold.dim = FLAGS_dim_threshold;
  param.benne_threshold.queue_size = FLAGS_queue_size_threshold;
  param.benne_threshold.variance = FLAGS_variance_threshold;
  param.benne_threshold.outliers_num = FLAGS_outliers_num_threshold;
  param.benne_threshold.outliers_dist = FLAGS_outliers_dist_threshold;

  param.fast_source = true;
  param.store = false;

  RunBenchmark(param);
}
