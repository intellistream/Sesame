// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 19/07/2021.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_POINT_HPP_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_POINT_HPP_

#include <memory>
#include <vector>

namespace SESAME {
class Point;
typedef std::shared_ptr<Point> PointPtr;

class Point {
private:
  int index;     // 1,2,3,4,5....
  double weight; // considering the outdated effect
  double cost;
  double minDist;
  int timestamp;

private:
  // the distance to the nearest data point
  int clusteringCenter;         // using index to identify
  int dimension;                // feature Length
  std::vector<double> *feature; // TODO: need to think how to remove * here.
public:
  Point();
  Point(int index, double weight, int dimension, double cost);
  Point(int index, double weight, int dimension, double cost, int timestamp);
  void setCost(double c);
  double getCost() const;
  int getIndex() const;
  void setIndex(int index);
  double getWeight() const;
  void setWeight(double weight);
  double getFeatureItem(int index) const;
  void setFeatureItem(double feature, int index);
  int getClusteringCenter() const;
  void setClusteringCenter(int index);
  int getDimension() const;
  void setDimension(int d);
  int getFeatureLength();
  double getDisTo(SESAME::PointPtr p);
  double getMinDist() const;
  void setMinDist(double min_dist);
  SESAME::PointPtr copy();
  void setTimeStamp(int t);
  int getTimeStamp() const;
  double radius(PointPtr centroid);
  double distance(PointPtr centroid);
};
} // namespace SESAME
#endif // SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_POINT_HPP_
