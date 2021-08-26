//
// Created by Zhenyu on 2021/8/20.
//
#ifndef SESAME_INCLUDE_ALGORITHM_OFFLINECLUSTERING_DBSCAN_HPP_
#define SESAME_INCLUDE_ALGORITHM_OFFLINECLUSTERING_DBSCAN_HPP_
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>
#include <algorithm>
#include <cmath>
#include <Algorithm/DataStructure/Point.hpp>
#include <Sinks/DataSink.hpp>
#include <memory>
#include <Algorithm/OfflineClustering/OfflineClustering.hpp>

namespace SESAME {
#define UNCLASSIFIED (-1)
#define CORE_POINT 1
#define NOISE (-2)
#define SUCCESS 0
#define FAILURE (-3)
//#define BORDER_POINT 2
class DBSCAN : public SESAME::OfflineClustering {
 public:
  DBSCAN(unsigned int minPts, float eps, int size);
  ~DBSCAN();
  void run(std::vector<PointPtr> &input);
  vector<int> calculateCluster(std::vector<PointPtr> &input,PointPtr &point) const;
  int expandCluster(std::vector<PointPtr> &input,PointPtr &point, int clusterID) const;
  static bool judgeCorePoint(PointPtr &point,PointPtr &other);
  static double calculateEluDistance(PointPtr &point, PointPtr & other);
  void  produceResult(std::vector<PointPtr> &input,SESAME::DataSinkPtr sinkPtr);
  //Obtain private members
  unsigned int getTotalPointSize() const {return pointSize;}
  unsigned int getMinimumClusterSize() const {return minPoints;}
  double getEpsilonSize() const  {return epsilon;}
  int getClusterID() const {return clusterID;}
 private:
  unsigned int pointSize;
  unsigned int minPoints;
  int clusterID;
  double epsilon;
};
}
#endif //SESAME_INCLUDE_ALGORITHM_OFFLINECLUSTERING_DBSCAN_HPP_
