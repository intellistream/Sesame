// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 19/07/2021.
//

#ifndef ONLINEMLBENCHMARK_BENCHMARK_SRC_UTIL_BENCHMARKUTILS_HPP_
#define ONLINEMLBENCHMARK_BENCHMARK_SRC_UTIL_BENCHMARKUTILS_HPP_

#include "Algorithm/Algorithm.hpp"
#include "Algorithm/DataStructure/Point.hpp"
#include "Evaluation/Evaluation.hpp"
#include "Sinks/DataSink.hpp"
#include "Sources/DataSource.hpp"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct param_t {
  int num_points, dim, num_clusters;
  bool time_decay = false;
  int coreset_size, seed;

  std::string input_file, output_file;
  SESAME::AlgoType algo;
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
  bool detect_outlier = false;               // whether detect outliers
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
  void print() {
    std::cerr << "algo: " << algo << std::endl;
    std::cerr << "input_file: " << input_file << std::endl;
    std::cerr << "num_points: " << num_points << std::endl;
    std::cerr << "dim: " << dim << std::endl;
    std::cerr << "num_clusters: " << num_clusters << std::endl;
    std::cerr << "max_in_nodes: " << max_in_nodes << std::endl;
    std::cerr << "max_leaf_nodes: " << max_leaf_nodes << std::endl;
    std::cerr << "distance_threshold: " << distance_threshold << std::endl;
    std::cerr << "seed: " << seed << std::endl;
    std::cerr << "coreset_size: " << coreset_size << std::endl;
    std::cerr << "radius: " << radius << std::endl;
    std::cerr << "delta: " << delta << std::endl;
    std::cerr << "beta: " << beta << std::endl;
    std::cerr << "buf_size: " << buf_size << std::endl;
    std::cerr << "alpha: " << alpha << std::endl;
    std::cerr << "lambda: " << lambda << std::endl;
    std::cerr << "clean_interval: " << clean_interval << std::endl;
    std::cerr << "min_weight: " << min_weight << std::endl;
    std::cerr << "base: " << base << std::endl;
    std::cerr << "cm: " << cm << std::endl;
    std::cerr << "cl: " << cl << std::endl;
    std::cerr << "grid_width: " << grid_width << std::endl;
    std::cerr << "min_points: " << min_points << std::endl;
    std::cerr << "epsilon: " << epsilon << std::endl;
    std::cerr << "mu: " << mu << std::endl;
    std::cerr << "num_last_arr: " << num_last_arr << std::endl;
    std::cerr << "time_window: " << time_window << std::endl;
    std::cerr << "num_online_clusters: " << num_online_clusters << std::endl;
    std::cerr << "delta_grid: " << delta_grid << std::endl;
    std::cerr << "num_samples: " << num_samples << std::endl;
  }
};

class BenchmarkUtils {
public:
  static void print_help(char *string);
  static void parseArgs(int argc, char **argv, param_t &cmd_params);
  static void defaultParam(param_t &cmd_params);
  static void loadData(param_t &cmd_params,
                       SESAME::DataSourcePtr dataSourcePtr);
  static BenchmarkResultPtr runBenchmark(param_t &cmd_params,
                                         SESAME::DataSourcePtr sourcePtr,
                                         SESAME::DataSinkPtr sinkPtr,
                                         SESAME::AlgorithmPtr algoPtr);
};

#endif // ONLINEMLBENCHMARK_BENCHMARK_SRC_UTIL_BENCHMARKUTILS_HPP_
