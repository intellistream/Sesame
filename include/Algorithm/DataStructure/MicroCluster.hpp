//
// Created by Zhenyu on 2021/8/8.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_MICROCLUSTER_HPP_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_MICROCLUSTER_HPP_
#include <vector>
#include <algorithm>
#include <cmath>
#include <Algorithm/DataStructure/Point.hpp>
#include <cassert>
#include <iostream>
namespace SESAME {

typedef std::vector<double> dataPoint;

class MicroCluster;

typedef std::shared_ptr<MicroCluster> MicroClusterPtr;
class MicroCluster {
 public:
  const double EPSILON = 0.00005;
  const double MIN_VARIANCE = 1e-50;//TODO Need to move ...

  std::vector<int> id;
  dataPoint centroid;
  dataPoint CF1x;//  the sum of the data values for each dimension
  dataPoint CF2x;// the sum of the squares of  the data values for each dimension
  int CF1t; //  the sum of the time stamps Ti~... Tin
  int CF2t;//the sum of the squares of the time stamps  Til... Tin
  int clusterNum; //number of data point in the clusters
  int dimension;


  MicroCluster();
  MicroCluster(int dimension, int id);
  ~MicroCluster();
  void init(PointPtr datapoint, int timestamp);
  void insert(PointPtr datapoint, int timestamp);
  void merge(MicroCluster &other);
  void substractClusterVector(MicroCluster &other);
  void updateId(MicroCluster &other);
  double getRelevanceStamp(int m) const;
  double getMutime() const;
  double getSigmaTime() const;
  static double getQuantile(double z);
  double getRadius(double radiusFactor);
  double getDeviation();
  dataPoint getCentroid();
  double getInclusionProbability(PointPtr datapoint,double radiusFactor);
  dataPoint getVarianceVector();
  double calCentroidDistance(PointPtr datapoint);
  bool judgeMerge(MicroCluster &other);
 private:

  static double inverseError(double x);
};
}
#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_MICROCLUSTER_HPP_
