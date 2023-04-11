// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 19/07/2021.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_POINT_HPP_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_POINT_HPP_

#include <chrono>
#include <iostream>
#include <memory>
#include <vector>

namespace SESAME
{
class Point;
typedef std::shared_ptr<Point> PointPtr;

class Point
{
public:
    using clock_t = std::chrono::_V2::system_clock::time_point;
    int index;          // 1,2,3,4,5....
    double weight = 1;  // considering the outdated effect
    double cost;
    double min_dist;
    double knn = 0.0, conn = 1.0;
    int timestamp;
    bool outlier = false;
    int sgn      = 1;             // the distance to the nearest data point
    int clu_id   = -1;            // using index to identify
    int dim;                      // feature Length
    clock_t toa;                  // time of arrival
    std::vector<double> feature;  // TODO: need to think how to remove * here.
    Point(int dim, int index = -1, double weight = 1.0, double cost = 0.0, int timestamp = 0);
    PointPtr copy();
    void setCost(double c);
    double getCost() const;
    int getIndex() const;
    void setIndex(int index);
    double getWeight() const;
    void setWeight(double weight);
    double getFeatureItem(int index) const;
    void setFeatureItem(double feature, int index);
    int getClusteringCenter() const;
    void setClusteringCenter(int index);
    int getDimension() const;
    void setDimension(int d);
    int getFeatureLength();
    double getMinDist() const;
    void setMinDist(double min_dist);
    void setTimeStamp(int t);
    int getTimeStamp() const;
    bool getOutlier();
    void setOutlier(bool flag);
    double L2Dist(PointPtr centroid);
    double L1Dist(PointPtr centroid);
    PointPtr Reverse();
    std::string Serialize();
    void Debug();
};
}  // namespace SESAME
#endif  // SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_POINT_HPP_
