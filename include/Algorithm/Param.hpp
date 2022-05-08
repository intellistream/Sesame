#ifndef PARAM_HPP_
#define PARAM_HPP_

#include <filesystem>
#include <iostream>
#include <string>

namespace SESAME {

enum AlgoType {
  BirchType,
  StreamKMeansType,
  CluStreamType,
  DenStreamType,
  DBStreamType,
  EDMStreamType,
  DStreamType,
  SLKMeansType,
  V1Stream = 11,
  V2Stream,
  V3Stream,
  V4Stream,
  V5Stream,
  V6Stream,
  V7Stream,
  V8Stream,
  V9Stream,
  Generic,
  G1Stream = 21,
  G2Stream,
  G3Stream,
  G4Stream,
  G5Stream,
  G6Stream,
  G7Stream,
  G8Stream,
  G9Stream,
};

extern char const *algo_names[30];

struct param_t {
  int num_points, dim, num_clusters;
  bool time_decay = false;
  int coreset_size, seed = 10;
  bool fast_source = false;
  bool store = true;

  std::string input_file, output_file;
  AlgoType algo;
  int dataset_option;

  int num_last_arr, time_window;
  unsigned int time_interval = 50;
  int num_online_clusters;

  int buf_size, offline_time_window = 100;

  // CF Tree
  int max_in_nodes, max_leaf_nodes;
  double distance_threshold;

  // used in DBSCAN
  unsigned int min_points;
  double epsilon;

  // used in DenStream(unique)
  double base, lambda, mu;
  double beta; // Also used in DStream, but different meaning

  // EDMStream
  double delta;
  int num_cache, opt;

  // used in DBStream
  double radius, min_weight, alpha;
  int clean_interval;

  // used in DStream
  double cm, cl;
  double grid_width;

  // used in design aspect
  bool run_offline = false; // determine whether to run the offline refinement
  int landmark; // this is the index of landmark point[start from 0](determine
                // to process the algorithm from which algorithm)
  int sliding = 10; // since we test the count-based sliding window, this is the
                    // count number
  bool detect_outlier = false;       // whether detect outliers
  double outlier_distance_threshold; // the max distance of the incoming point
                                     // to its nearest clusters
  double outlier_density_threshold;  // the density value of the point to be
                                     // treated as an outlier
  double neighbor_distance;          // the distance value of the point to judge
                                     // neighborhoods
  int outlier_cap;      // transfer outlier cluster and true cluster
  bool kmeanspp = true; // whether use kmeans++ to initialize the centroids

  double delta_grid =
      0.2; // The delta parameter used int the grid for guessing the optimum.
  int num_samples = 100; // The number of samples used in the grid for guessing
                         // the optimum.
  int num_res;
  void Print() {
    std::cout << "algo: \"" << algo_names[algo] << "\"" << std::endl;
    std::cout << "workload: " << std::filesystem::path(input_file).stem()
              << std::endl;
    std::cout << "num_points: " << num_points << std::endl;
    std::cout << "dim: " << dim << std::endl;
    std::cout << "num_clusters: " << num_clusters << std::endl;
    std::cout << "max_in_nodes: " << max_in_nodes << std::endl;
    std::cout << "max_leaf_nodes: " << max_leaf_nodes << std::endl;
    std::cout << "distance_threshold: " << distance_threshold << std::endl;
    std::cout << "seed: " << seed << std::endl;
    std::cout << "coreset_size: " << coreset_size << std::endl;
    std::cout << "radius: " << radius << std::endl;
    std::cout << "delta: " << delta << std::endl;
    std::cout << "beta: " << beta << std::endl;
    std::cout << "buf_size: " << buf_size << std::endl;
    std::cout << "alpha: " << alpha << std::endl;
    std::cout << "lambda: " << lambda << std::endl;
    std::cout << "clean_interval: " << clean_interval << std::endl;
    std::cout << "min_weight: " << min_weight << std::endl;
    std::cout << "base: " << base << std::endl;
    std::cout << "cm: " << cm << std::endl;
    std::cout << "cl: " << cl << std::endl;
    std::cout << "grid_width: " << grid_width << std::endl;
    std::cout << "min_points: " << min_points << std::endl;
    std::cout << "epsilon: " << epsilon << std::endl;
    std::cout << "mu: " << mu << std::endl;
    std::cout << "num_last_arr: " << num_last_arr << std::endl;
    std::cout << "time_window: " << time_window << std::endl;
    std::cout << "num_online_clusters: " << num_online_clusters << std::endl;
    std::cout << "delta_grid: " << delta_grid << std::endl;
    std::cout << "num_samples: " << num_samples << std::endl;
    std::cout << "landmark: " << landmark << std::endl;
    std::cout << "sliding: " << sliding << std::endl;
    std::cout << "outlier_distance_threshold: " << outlier_distance_threshold
              << std::endl;
    std::cout << "outlier_cap: " << outlier_cap << std::endl;
    std::cout << "outlier_density_threshold: " << outlier_density_threshold
              << std::endl;
    std::cout << "neighbor_distance: " << neighbor_distance << std::endl;
  }
};

using AlgorithmParameters = param_t;

} // namespace SESAME

#endif // PARAM_HPP_