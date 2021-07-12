//
// Created by tuidan on 2021/7/10.
//

#ifndef ONLINEMLBENCHMARK_KMEANS_HPP
#define ONLINEMLBENCHMARK_KMEANS_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Point.hpp"

class KMeans++{
public:
    // Randomly chooses k centres with kMeans++ distribution
    struct point * chooseRandomCentres(int k,int n,int d,struct point points[])

    // kMeans++ algorithm for n points of dimension d with k centres
    struct point * lloydPlusPlus(int k,int n,int d,struct point points[], double * resultCost);

    // computes the target function for the given pointarray points[] (of size n) with the given array of centres centres[] (of size k)
    double targetFunctionValue(int k,int n,struct point centres[],struct point points[]);

    // Computes the index of the centre nearest to p with the given array of centres centres[] (of size k)
    int determineClusterCentreKMeans(int k,struct point p,struct point centres[]);

    // Computes the cost of point p with centre centre
    double costOfPointToCenter(struct point * p,struct point * centre);
};


#endif //ONLINEMLBENCHMARK_KMEANS_H

