// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by 1124a on 2021/8/16.
//

#ifndef SESAME_INCLUDE_ALGORITHM_CLUSTREAM_HPP_
#define SESAME_INCLUDE_ALGORITHM_CLUSTREAM_HPP_

#include <cmath>
#include <cassert>
#include <limits>
#include <ctime>
#include <cstdio>
#include <Algorithm/Algorithm.hpp>
#include<Algorithm/DataStructure/MicroCluster.hpp>
#include <Algorithm/OfflineClustering/KMeans.hpp>
#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <Algorithm/DataStructure/Snapshot.hpp>
namespace SESAME {

class CluStreamParameter : public AlgorithmParameters {
 public:
  int lastArrivingNum;
  int timeWindow;
  unsigned int timeInterval;
  int clusterNumber; //total number of micro clusters online
  int offlineClusterNumber; //total number of micro clusters online
  double radiusFactor;//radius factor
  int initBuffer;
  int offlineTimeWindow;
};

const double doubleMax = std::numeric_limits<double>::max();
class CluStream : public Algorithm {
 public:
  CluStreamParameter CluStreamParam;
  std::shared_ptr<KMeans> kmeans; //used for offline initialization
  LandmarkWindowPtr window;
  MicroClusters microClusters; //Defined in Snapshot, std::vector <MicroclusterPtr>
  int pointsFitted;
  int pointsForgot;
  int pointsMerged;
  clock_t startTime;
  CluStream();
  ~CluStream();

  void Initilize() override;

  void runOnlineClustering(PointPtr input) override;

  void runOfflineClustering(DataSinkPtr sinkPtr) override;

 private:
  void initOffline(vector <PointPtr> &initData, vector <PointPtr> &initialData);
  void incrementalCluster(PointPtr data);
  double calRadius(MicroClusterPtr closestCluster);
  void insertIntoCluster(PointPtr data, MicroClusterPtr closestCluster);
  void deleteCreateCluster(PointPtr data);
  void MergeCreateCluster(PointPtr data);
  void microClusterToPoint(MicroClusters &microClusters, vector <PointPtr> &points) const;
  static double distance(dataPoint a, dataPoint b, int dim);

  bool initilized = false;
  vector <PointPtr> initialInputs;
};

}
#endif //SESAME_INCLUDE_ALGORITHM_CLUSTREAM_HPP_
