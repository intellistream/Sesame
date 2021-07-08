//
// Created by tuidan on 2021/7/8.
//
#include<bits/stdc++.h>
#ifndef ONLINEMLBENCHMARK_POINT_HPP
#define ONLINEMLBENCHMARK_POINT_HPP

#endif //ONLINEMLBENCHMARK_POINT_H

class Point{
private:
    int index; // 1,2,3,4,5....
    double weight; // considering the outdated effect
    double* feature;
    int clusteringCenter;  // using index to identify
public:
    point(int index, double weight, double* feature, int featureLength);
    int getIndex();
    void setIndex(int index);
    double getWeight();
    void setWeight(double weight);
    double getFeatureItem(int index);
    void setFeatureItem(double feature, int index);
    int getClusteringCenter();
    void setClusteringCenter(int index);
};

