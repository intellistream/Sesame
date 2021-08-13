//
// Created by Zhenyu on 2021/8/8.
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
namespace SESAME{

class CluStreamParameter: public AlgorithmParameters{
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
class CluStream: public Algorithm{
 public:
  CluStreamParameter CluStreamParam;
  KMeans km;//used for offline initialization
  LandmarkWindowPtr window;
  std::vector <MicroCluster>  microClusters;

  int pointsFitted;
  int pointsForgot;
  int pointsMerged;
  clock_t startTime;
  CluStream();
  ~CluStream();

  void runOnlineClustering(const vector<PointPtr> &input) override;
  void runOfflineClustering(const std::vector<PointPtr> &input, vector<PointPtr> &output) override;

  void initOffline(vector<PointPtr> &initData, vector<PointPtr> &initialData);
  void incrementalCluster(PointPtr data);
  double calRadius(MicroCluster * closestCluster);
  void insertIntoCluster(PointPtr data,  MicroCluster * closestCluster);
  void deleteCreateCluster(PointPtr data);
  void MergeCreateCluster(PointPtr data);
   void microClusterToPoint(std::vector <MicroCluster> &microClusters, vector<PointPtr> &points);
  static double distance(dataPoint a,dataPoint b,int dim);
};

/**
      Args:
      timeWindow (int): Time of Landmark Window parameter ,
      clusterNumber (int): Maximum number of micro clusters to use,
      lastArrivingNum (int): The last m arriving in the micro clusters,
      radiusFactor (int): Multiplier for the Cluster radius,(the maximal boundary factor,calculated by the
      RMS deviation of the data points in Mp from the centroid in the paper),

      Attributes:
      StartTime (int): Program starting timestamp
      pointsFitted (int): number of data points that entered the 'fit' case
      pointsForgot (int): number of data points that entered the 'forgot micro cluster' case
      pointsMerged (int): number of data points that entered the 'merged micro cluster' case"
**/
}
#endif //SESAME_INCLUDE_ALGORITHM_CLUSTREAM_HPP_
