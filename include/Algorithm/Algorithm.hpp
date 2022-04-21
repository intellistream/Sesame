// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by tuidan on 2021/7/22.
//

#ifndef SESAME_INCLUDE_ALGORITHM_ALGORITHM_HPP_
#define SESAME_INCLUDE_ALGORITHM_ALGORITHM_HPP_

#include "Algorithm/DataStructure/Point.hpp"
#include "Sinks/DataSink.hpp"
#include "Timer/Timer.hpp"

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

using namespace std;

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
  V1Stream,
  V2Stream,
  V3Stream,
  V4Stream,
  V5Stream,
  V6Stream,
  V7Stream,
  V8Stream,
  V9Stream,
  Generic
};

static char *algo_names[] = {
    "Birch",     "StreamKMeans", "CluStream", "DenStream", "DBStream",
    "EDMStream", "DStream",      "SL-KMeans", "V1Stream",  "V2Stream",
    "V3Stream",  "V4Stream",     "V5Stream",  "V6Stream",  "V7Stream",
    "V8Stream",  "V9Stream",     "Generic"};

class Algorithm;
typedef std::shared_ptr<Algorithm> AlgorithmPtr;

struct param_t {
  int num_points, dim, num_clusters;
  bool time_decay = false;
  int coreset_size, seed;

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
  int sliding;  // since we test the count-based sliding window, this is the
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
  void Print() {
    std::cout << "algo: " << algo_names[algo] << std::endl;
    std::cout << "input_file: " << std::filesystem::path(input_file).filename()
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
  }
};

using AlgorithmParameters = param_t;

class Algorithm {
public:
  Algorithm() = default;
  virtual ~Algorithm() = default;
  virtual void Init() = 0;
  virtual void RunOnline(SESAME::PointPtr input) = 0;
  virtual void RunOffline(SESAME::DataSinkPtr ptr) = 0;
  void store(std::string output_file, int dim, std::vector<PointPtr> results);
  Timer ds_timer, out_timer, ref_timer, sum_timer;
  param_t param;
  void PrintPerf() {
    cout << "ds_us: " << ds_timer.sum / 1000 << endl;
    cout << "out_us: " << out_timer.sum / 1000 << endl;
    cout << "ref_us: " << ref_timer.sum / 1000 << endl;
    auto sum = ds_timer.sum + out_timer.sum + ref_timer.sum;
    cout << "sum_us: " << sum / 1000 << endl;
    cout << "lat_us: " << sum / 1e3 / param.num_points << endl;
    cout << "et_s: " << sum_timer.sum / 1e9 << endl;
    cout << "qps: " << param.num_points * 1e9 / sum_timer.sum << endl;
  }
};

} // namespace SESAME

#endif // SESAME_INCLUDE_ALGORITHM_ALGORITHM_HPP_
