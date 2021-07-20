//
// Created by Shuhao Zhang on 19/07/2021.
//

#include <Algorithm/DataStructure/Point.hpp>

Point::Point() {
  this->index = -1;
  this->weight = 1;
  this->dimension = 54;
  this->feature = new double[this->dimension];
  this->cost = 0;
  this->clusteringCenter = -1;
}
void Point::Initialization(int index, double weight, int dimension, double cost) {
  this->index = index;
  this->weight = weight;
  this->dimension = dimension;
  this->feature = new double[dimension];
  this->cost = cost;
  this->clusteringCenter = -1;
}

int Point::getIndex() {
  return this->index;
}

void Point::setIndex(int index) {
  this->index = index;
}

double Point::getWeight() {
  return this->weight;
}

void Point::setWeight(double weight) {
  this->weight = weight;
}

double Point::getFeatureItem(int index) {
  return this->feature[index];
}

void Point::setFeatureItem(double feature, int index) {
  this->feature[index] = feature;
}

int Point::getClusteringCenter() {
  return this->clusteringCenter;
}

void Point::setClusteringCenter(int index) {
  this->clusteringCenter = index;
}

double *Point::getFeatures() {
  return this->feature;
}

void Point::setCost(double c) {
  this->cost = c;
}

double Point::getCost() {
  return this->cost;
}

void Point::copyFromPoint(Point source) {
  this->index = source.getIndex();
  this->dimension = source.getDimension();
  this->weight = source.getWeight();
  this->cost = source.getCost();
  this->clusteringCenter = source.getClusteringCenter();
  for (int i = 0; i < source.getDimension(); i++) {
    this->feature[i] = source.getFeatureItem(i);
  }
}
