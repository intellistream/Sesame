// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

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
  int index; // 1,2,3,4,5....
  double weight; // considering the outdated effect
  double cost;
  int clusteringCenter;  // using index to identify
  int dimension;// feature Length
  std::vector<double> *feature;//TODO: need to think how to remove * here.
 public:
  Point();
  Point(int index, double weight, int dimension, double cost);
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
  SESAME::PointPtr copy();
};
}
#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_POINT_HPP_
