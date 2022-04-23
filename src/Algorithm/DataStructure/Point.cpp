// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 19/07/2021.
//

#include "Algorithm/DataStructure/Point.hpp"

#include <cassert>
#include <cmath>

namespace SESAME {

Point::Point(int dim, int index, double weight, double cost,
             int timestamp)
    : feature(dim, 0.0) {
  this->index = index;
  this->weight = weight;
  this->dim = dim;
  this->cost = cost;
  this->clusteringCenter = -1;
  this->timestamp = timestamp;
}

int Point::getIndex() const { return this->index; }

void Point::setIndex(int index) { this->index = index; }

double Point::getWeight() const { return this->weight; }

void Point::setWeight(double weight) { this->weight = weight; }

double Point::getFeatureItem(int index) const {
  return this->feature[index];
}

void Point::setFeatureItem(double feature, int index) {
  this->feature.at(index) = feature;
}

int Point::getClusteringCenter() const { return this->clusteringCenter; }

void Point::setClusteringCenter(int index) { this->clusteringCenter = index; }

void Point::setCost(double c) { this->cost = c; }

double Point::getCost() const { return this->cost; }

void Point::setTimeStamp(int t) { this->timestamp = t; }

int Point::getTimeStamp() const { return this->timestamp; }

/**
 * @param source
 */
PointPtr Point::copy() { return std::make_shared<Point>(*this); }
int Point::getDimension() const { return this->dim; }
int Point::getFeatureLength() { return (int)this->feature.size(); }

double Point::getDisTo(PointPtr p) {
  double distance = 0;
  for (int i = 0; i < this->dim; i++) {
    distance =
        distance + pow(p->getFeatureItem(i) - this->getFeatureItem(i), 2);
  }
  return sqrt(distance);
}
double Point::getMinDist() const { return minDist; }
void Point::setMinDist(double min_dist) { minDist = min_dist; }

double Point::distance(PointPtr centroid) {
  double sum = 0;
  for (int i = 0; i < getDimension(); i++) {
    auto val = centroid->getFeatureItem(i) - getFeatureItem(i);
    sum += fabs(val);
  }
  return sum;
}

double Point::Radius(PointPtr centroid) {
  double sum = 0.0;
  for (int i = 0; i < getDimension(); i++) {
    assert(std::isnan(centroid->getFeatureItem(i)) == false);
    assert(std::isnan(getFeatureItem(i)) == false);
    auto val = centroid->getFeatureItem(i) - getFeatureItem(i);
    sum += val * val;
  }
  assert(sum >= 0.0);
  return sqrt(sum);
}
void SESAME::Point::setIsOutlier(bool flag) { this->isOutlier = flag; }
bool SESAME::Point::getIsOutlier() { return this->isOutlier; }

PointPtr Point::Reverse() {
  auto res = copy();
  res->sgn = -res->sgn;
  return res;
}

} // namespace SESAME
