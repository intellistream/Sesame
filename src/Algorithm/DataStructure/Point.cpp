// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 19/07/2021.
//

#include <cmath>
#include <Algorithm/DataStructure/Point.hpp>

SESAME::Point::Point() {
  this->index = -1;
  this->weight = 1;
  this->dimension = 54;
  this->cost = 0;
  this->clusteringCenter = -1;
  this->feature = new std::vector<double>(54);
  this->timestamp = 0;
}

SESAME::Point::Point(int index, double weight, int dimension, double cost) {
  this->index = index;
  this->weight = weight;
  this->dimension = dimension;
  this->cost = cost;
  this->clusteringCenter = -1;
  this->feature = new std::vector<double>(dimension);
}

SESAME::Point::Point(int index, double weight, int dimension, double cost, int timestamp) {
  this->index = index;
  this->weight = weight;
  this->dimension = dimension;
  this->cost = cost;
  this->clusteringCenter = -1;
  this->feature = new std::vector<double>(dimension);
  this->timestamp = timestamp;
}

int SESAME::Point::getIndex() const {
  return this->index;
}

void SESAME::Point::setIndex(int index) {
  this->index = index;
}

double SESAME::Point::getWeight() const {
  return this->weight;
}

void SESAME::Point::setWeight(double weight) {
  this->weight = weight;
}

double SESAME::Point::getFeatureItem(int index) const {
  return this->feature->at(index);
}

void SESAME::Point::setFeatureItem(double feature, int index) {
  this->feature->at(index) = feature;
}

int SESAME::Point::getClusteringCenter() const {
  return this->clusteringCenter;
}

void SESAME::Point::setClusteringCenter(int index) {
  this->clusteringCenter = index;
}

void SESAME::Point::setCost(double c) {
  this->cost = c;
}

double SESAME::Point::getCost() const {
  return this->cost;
}

void SESAME::Point::setTimeStamp(int t) {
  this->timestamp = t;
}

int SESAME::Point::getTimeStamp() const {
  return this->timestamp;
}



/**
 * @param source
 */
SESAME::PointPtr SESAME::Point::copy() {
  return std::make_shared<Point>(*this);
}
int SESAME::Point::getDimension() const {
  return this->dimension;
}
int SESAME::Point::getFeatureLength() {
  return (int)this->feature->size();
}

double SESAME::Point::getDisTo(SESAME::PointPtr p) {
  double distance = 0;
  for(int i = 0; i < this->dimension; i++) {
    distance = distance + pow(p->getFeatureItem(i) - this->getFeatureItem(i), 2);
  }
  return sqrt(distance);
}
double SESAME::Point::getMinDist() const {
  return minDist;
}
void SESAME::Point::setMinDist(double min_dist) {
  minDist = min_dist;
}
