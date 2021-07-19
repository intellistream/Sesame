//
// Created by Shuhao Zhang on 19/07/2021.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_POINT_HPP_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_POINT_HPP_
class Point {
 private:
  int index; // 1,2,3,4,5....
  double weight; // considering the outdated effect
  double *feature;
  double cost;
  int dimension; // feature Length
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
  int getDimension();
  void setDimension(int d);
  double getFeatureItem(int index);
  void setFeatureItem(double feature, int index);
  int getClusteringCenter();
  void setClusteringCenter(int index);
  void copyFromPoint(Point source);
  double *getFeatures();
};
#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_POINT_HPP_
