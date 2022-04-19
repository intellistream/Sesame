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
  bool time_decay;
  int coreset_size, seed, true_num_clusters;

  std::string input_file, output_file;
  SESAME::AlgoType algo;
  int dataset_option;

  int num_last_arr, time_window;
  unsigned int time_interval;
  int num_online_clusters;

  int buf_size, offline_time_window;

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
  bool run_offline; // determine whether to run the offline refinement
  int landmark; // this is the index of landmark point[start from 0](determine
                // to process the algorithm from which algorithm)
  int sliding;  // since we test the count-based sliding window, this is the
                // count number
  bool detect_outlier;               // whether detect outliers
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
