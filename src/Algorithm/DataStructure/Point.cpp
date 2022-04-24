// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 19/07/2021.
//

#include "Algorithm/DataStructure/Point.hpp"

#include <cassert>
#include <cmath>
#include <immintrin.h>

namespace SESAME {

Point::Point(int dim, int index, double weight, double cost,
             int timestamp)
    : feature((dim + 3) / 4 * 4, 0.0) {
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

double Point::L1Dist(PointPtr centroid) {
  static const auto mask = _mm256_set1_pd(0x7fffffff);
  double sum = 0;
  const int dim = getDimension();
  auto a = feature.data(), b = centroid->feature.data();
  auto sum_v = _mm256_setzero_pd();
  for(size_t i = 0; i < dim; i += 4) {
    __m256d diff = _mm256_sub_pd(_mm256_loadu_pd(a + i), _mm256_loadu_pd(b + i));
    auto abs_v = _mm256_and_pd(diff, mask);
    sum_v = _mm256_add_pd(sum_v, abs_v);
  }
  double v[4];
  _mm256_storeu_pd(v, sum_v);
  sum += v[0] + v[1] + v[2] + v[3];
  return sum;
}

double Point::L2Dist(PointPtr centroid) {
  double sum = 0.0;
  const int dim = getDimension();
  auto a = feature.data(), b = centroid->feature.data();
  auto sum_v = _mm256_setzero_pd();
  for(size_t i = 0; i < dim; i += 4) {
    __m256d diff = _mm256_sub_pd(_mm256_loadu_pd(a + i), _mm256_loadu_pd(b + i));
    __m256d square = _mm256_mul_pd(diff, diff);
    sum_v = _mm256_add_pd(sum_v, square);
  }
  double v[4];
  _mm256_storeu_pd(v, sum_v);
  sum += v[0] + v[1] + v[2] + v[3];
//   for (int i = 0; i < dim; i++) {
// #ifndef NDEBUG
//     assert(std::isnan(centroid->getFeatureItem(i)) == false);
//     assert(std::isnan(getFeatureItem(i)) == false);
// #endif
//     auto val = a[i] - b[i];
//     sum += val * val;
//   }
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
