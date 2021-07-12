//
// Created by tuidan on 2021/7/8.
//
#include "Point.hpp"


void Point::Initialization(int index, double weight, int featureLength, double cost){
    this->index = index;
    this->weight = weight;
    this->feature = new double[featureLength];
    this->cost = cost;
    this->clusteringCenter = NULL;
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

void Point::setFeatureLength(int length) {
    this->feature = new double[length];
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
