//
// Created by tuidan on 2021/7/8.
//
#include <cstdio>
#ifndef ONLINEMLBENCHMARK_POINT_HPP
#define ONLINEMLBENCHMARK_POINT_HPP

class Point{
private:
    int index; // 1,2,3,4,5....
    double weight; // considering the outdated effect
    double* feature;
    double cost;
    int clusteringCenter;  // using index to identify
public:
    void Initialization(int index, double weight,int featureLength, double cost);
    void setCost(double c);
    double getCost();
    int getIndex();
    void setIndex(int index);
    double getWeight();
    void setWeight(double weight);
    double getFeatureItem(int index);
    void setFeatureItem(double feature, int index);
    void setFeatureLength(int length);
    int getClusteringCenter();
    void setClusteringCenter(int index);
    double *getFeatures();
};

#endif //ONLINEMLBENCHMARK_POINT_H
