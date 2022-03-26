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
  int pointNumber;
  bool timeDecay;
  int clusterNumber;
  int dimension;
  int coresetSize;
  int seed;
  int lastArrivingNum;
  int timeWindow;
  unsigned int timeInterval;
  int onlineClusterNumber;
  double radiusFactor;
  int initBuffer;
  int offlineTimeWindow;
  int maxInternalNodes;
  int maxLeafNodes;
  int GTClusterNumber;
  double thresholdDistance;

  // used in DBSCAN
  unsigned int minPoints;
  double epsilon;

  // used in DenStream(unique)
  double base;
  double lambda;
  double mu;
  double beta; // Also used in DStream, but different meaning
  // EDMStream
  double a;
  int cacheNum;
  double delta;
  int opt;

  // used in DBStream
  double radius;
  int cleanUpInterval;
  double weightMin;
  double alpha;

  // used in DStream
  double cm;
  double cl;
  double gridWidth;
  std::string inputPath;
  std::string outputPath;
  SESAME::algoType algoType;
  int datasetOption;

  // used in design aspect
  bool executeOffline; // determine whether to run the offline refinement
  int landmark; // this is the index of landmark point[start from 0](determine
                // to process the algorithm from which algorithm)
  int sliding;  // since we test the count-based sliding window, this is the
                // count number
  bool detectOutliers;             // whether detect outliers
  double outlierDistanceThreshold; // the max distance of the incoming point to
                                   // its nearest clusters
  double densityThreshold; // the density value of the point to be treated as an
                           // outlier
  double neighborDistance; // the distance value of the point to judge
                           // neighborhoods
  int outlierClusterCapacity; // transfer outlier cluster and true cluster
  bool kmeanspp = true; // whether use kmeans++ to initialize the centroids
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
