// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 20/07/2021.
//

#ifndef SESAME_INCLUDE_ALGORITHM_OFFLINECLUSTERING_KMEANS_HPP_
#define SESAME_INCLUDE_ALGORITHM_OFFLINECLUSTERING_KMEANS_HPP_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>
#include <algorithm>
#include <cmath>
#include <Algorithm/DataStructure/Point.hpp>
#include <Algorithm/OfflineClustering/OfflineClustering.hpp>
#include <Utils/Logger.hpp>
#include <memory>
#include <Sinks/DataSink.hpp>
namespace SESAME {
class KMeans : public SESAME::OfflineClustering {
 public:
  // Randomly chooses k centres with kMeans++ distribution
  double calculateEluDistance(PointPtr &point, PointPtr &center);
  void calculateClusterCenter(PointPtr &center, std::vector<PointPtr> &group);
  void randomSelectCenters(int numberOfCenters, int numberOfInput,
                           std::vector<PointPtr> &input, std::vector<PointPtr> &centers);
  void selectCentersFromWeight(int numberOfCenters, int numberOfInput,
                               std::vector<PointPtr> &input, std::vector<PointPtr> &centers);
  void groupPointsByCenters(int numberOfCenters, int numberOfInput,
                            std::vector<PointPtr> &input, std::vector<PointPtr> &centers,
                            std::vector<std::vector<PointPtr>> &groups);
  void adjustClusteringCenters(std::vector<PointPtr> &centers, std::vector<vector < PointPtr>>
  &groups);
  void refreshGroup(std::vector<std::vector<PointPtr>> &oldGroups, vector <vector<PointPtr>> &newGroups);
  void checkStopStatus(bool &flag,
                       std::vector<std::vector<PointPtr>> &oldGroups,
                       std::vector<std::vector<PointPtr>> &newGroups);
  void storeResult(std::vector<std::vector<PointPtr>> &groups, std::vector<PointPtr> &output);
  void produceResult(std::vector<std::vector<PointPtr>> &groups, DataSinkPtr sinkPtr);
  void runKMeans(int numberOfCenters,
                 int numberOfInput,
                 std::vector<PointPtr> &centers,
                 std::vector<PointPtr> &input,
                 std::vector<std::vector<PointPtr>> &oldGroups,
                 std::vector<std::vector<PointPtr>> &newGroups,
                 bool KMeansPP);
};
}
#endif //SESAME_INCLUDE_ALGORITHM_OFFLINECLUSTERING_KMEANS_HPP_
