//
// Created by tuidan on 2021/7/8.
//
#include "Point.hpp"

Point::point(int index, double weight, double* dimension, int featureLength){
    this->index = index;
    this->weight = weight;
    this->feature = new double[this->featureLength];
    for(int i = 0; i < featureLength; i++) this->feature[i] = dimension[i];
    this->clusteringCenter = dimension[featureLength];
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
