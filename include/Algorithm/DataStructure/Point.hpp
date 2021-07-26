//
// Created by Shuhao Zhang on 19/07/2021.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_POINT_HPP_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_POINT_HPP_
#include <memory>
#include <Algorithm/DataStructure/DataStructure.hpp>
#include <vector>

namespace SESAME {
class Point;
typedef std::shared_ptr<Point> PointPtr;

class Point : public DataStructure {
 private:
  int index; // 1,2,3,4,5....
  double weight; // considering the outdated effect
  std::vector<double> feature;
  double cost;
  // feature Length
  int clusteringCenter;  // using index to identify
 public:
  Point();
  void Initialization(int index, double weight, int dimension, double cost);
  void setCost(double c);
  double getCost();
  int getIndex();
  void setIndex(int index);
  double getWeight();
  void setWeight(double weight);
  double getFeatureItem(int index);
  void setFeatureItem(double feature, int index);
  int getClusteringCenter();
  void setClusteringCenter(int index);
  void copyFromPoint(Point source);
  std::vector<double> getFeatures();
};
}
#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_POINT_HPP_
