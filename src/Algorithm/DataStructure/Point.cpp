//
// Created by Shuhao Zhang on 19/07/2021.
//

#include <Algorithm/DataStructure/Point.hpp>

SESAME::Point::Point() {
  this->index = -1;
  this->weight = 1;
  this->dimension = 54;
  this->cost = 0;
  this->clusteringCenter = -1;
}
void SESAME::Point::Initialization(int index, double weight, int dimension, double cost) {
  this->index = index;
  this->weight = weight;
  this->dimension = dimension;
  this->cost = cost;
  this->clusteringCenter = -1;
}

int SESAME::Point::getIndex() {
  return this->index;
}

void SESAME::Point::setIndex(int index) {
  this->index = index;
}

double SESAME::Point::getWeight() {
  return this->weight;
}

void SESAME::Point::setWeight(double weight) {
  this->weight = weight;
}

double SESAME::Point::getFeatureItem(int index) {
  return this->feature[index];
}

void SESAME::Point::setFeatureItem(double feature, int index) {
  this->feature[index] = feature;
}

int SESAME::Point::getClusteringCenter() {
  return this->clusteringCenter;
}

void SESAME::Point::setClusteringCenter(int index) {
  this->clusteringCenter = index;
}
std::vector<double> SESAME::Point::getFeatures() {
  return this->feature;
}

void SESAME::Point::setCost(double c) {
  this->cost = c;
}

double SESAME::Point::getCost() {
  return this->cost;
}

/**
 * TODO: this method is very weird. Why copy by value?
 * @param source
 */
void SESAME::Point::copyFromPoint(Point source) {
  this->index = source.getIndex();
  this->dimension = source.getDimension();
  this->weight = source.getWeight();
  this->cost = source.getCost();
  this->clusteringCenter = source.getClusteringCenter();
  for (int i = 0; i < source.getDimension(); i++) {
    this->feature[i] = source.getFeatureItem(i);
  }
}
