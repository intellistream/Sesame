// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by wzru on 2022/3/13.
//

#ifndef SESAME_INCLUDE_ALGORITHM_PARAM_HPP_
#define SESAME_INCLUDE_ALGORITHM_PARAM_HPP_

namespace SESAME {

class StreamClusteringParam {
public:
  // General Params
  int pointNumber;
  int dimension;

  // CF-Tree Params
  int maxInternalNodes;     // B
  int maxLeafNodes;         // L
  double thresholdDistance; // T

  // KMeans / KMeans++ Params
  int clusterNumber;
  bool KMeansPP;

  // DBSCAN Params
  unsigned int minPoints; // minimum point of core point in DBSCAN
  double epsilon; // maximum distance if point belongs to the density area of
                  // core point

  // Landmark Window
  int landmark;
  // Sliding Window
  int sliding;
  // Damped Window
  double alpha, lambda;

  double outlierDistanceThreshold;
  int outlierClusterCapacity;
  double neighborDistance, densityThreshold;
};

} // namespace SESAME

#endif