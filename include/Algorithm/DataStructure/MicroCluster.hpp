// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by 1124a on 2021/8/16.
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
  constexpr static  const double EPSILON = 0.00005;
  constexpr static  const double MIN_VARIANCE = 1e-50;//TODO Need to move ...

  std::vector<int> id;
  dataPoint centroid;
  dataPoint LS;//  the sum of the data values for each dimension
  dataPoint SS;// the sum of the squares of  the data values for each dimension
  int LST; //  the sum of the time stamps Ti~... Tin
  int SST;//the sum of the squares of the time stamps  Til... Tin
  int weight; //number of data point in the clusters
  int dimension;

  MicroCluster(int dimension, int id);
  ~MicroCluster();
  void init(PointPtr datapoint, int timestamp);

  void insert(PointPtr datapoint, int timestamp);
  void merge(MicroClusterPtr other);
  void substractClusterVector(MicroClusterPtr other);
  void updateId(MicroClusterPtr other);
  double getRelevanceStamp(int lastArrivingNum) const;
  double getMutime() const;
  double getSigmaTime() const;
  static double getQuantile(double z);
  double getRadius(double radiusFactor);
  double getDeviation();
  dataPoint getCentroid();
  double getInclusionProbability(PointPtr datapoint,double radiusFactor);
  dataPoint getVarianceVector();
  double calCentroidDistance(PointPtr datapoint);
  bool judgeMerge(MicroClusterPtr other);
  SESAME::MicroClusterPtr copy();

 private:
  static double inverseError(double x);
};
}
#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_MICROCLUSTER_HPP_
