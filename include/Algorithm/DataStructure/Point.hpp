// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 19/07/2021.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_POINT_HPP_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_POINT_HPP_

#include <iostream>
#include <memory>
#include <vector>

namespace SESAME {
class Point;
typedef std::shared_ptr<Point> PointPtr;

class Point {
public:
  int index;         // 1,2,3,4,5....
  double weight = 1; // considering the outdated effect
  double cost;
  double minDist;
  int timestamp;
  bool isOutlier;
  int sgn = 1;

private:
  // the distance to the nearest data point
  int clusteringCenter;         // using index to identify
  int dimension;                // feature Length
  std::vector<double> feature; // TODO: need to think how to remove * here.
public:
  Point(int dimension, int index = -1, double weight = 1.0, double cost = 0.0,
        int timestamp = 0);
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
  double getDisTo(PointPtr p);
  double getMinDist() const;
  void setMinDist(double min_dist);
  PointPtr copy();
  void setTimeStamp(int t);
  int getTimeStamp() const;
  bool getIsOutlier();
  void setIsOutlier(bool flag);
  double Radius(PointPtr centroid);
  double distance(PointPtr centroid);
  PointPtr Reverse();
  std::string Serialize() {
    std::string str =
        "#" + std::to_string(index) + " " + std::to_string(dimension);
    for (int i = 0; i < dimension; i++) {
      str += "," + std::to_string(feature.at(i));
    }
    return str;
  }
  void Debug() { std::cerr << Serialize() << std::endl; }
};
} // namespace SESAME
#endif // SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_POINT_HPP_
