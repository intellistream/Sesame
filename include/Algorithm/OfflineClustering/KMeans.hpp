//
// Created by Shuhao Zhang on 20/07/2021.
//

#ifndef SESAME_INCLUDE_ALGORITHM_OFFLINECLUSTERING_KMEANS_HPP_
#define SESAME_INCLUDE_ALGORITHM_OFFLINECLUSTERING_KMEANS_HPP_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <Algorithm/DataStructure/Point.hpp>
#include <Algorithm/OfflineClustering/OfflineClustering.hpp>

namespace SESAME {
class KMeans : public SESAME::OfflineClustering {
 public:
  // Randomly chooses k centres with kMeans++ distribution
  std::vector<SESAME::PointPtr> chooseRandomCentres(int k, int n, std::vector<PointPtr> &points);

  // kMeans++ algorithm for n points of dimension d with k centres
  std::vector<SESAME::PointPtr> lloydPlusPlus(int k, int n, int d, std::vector<PointPtr> &points, double *resultCost);

  // computes the target function for the given pointarray points[] (of size n) with the given array of centres centres[] (of size k)
  double targetFunctionValue(int k, int n, std::vector<PointPtr> &centres, std::vector<PointPtr> &points);

  // Computes the index of the centre nearest to p with the given array of centres centres[] (of size k)
  int determineClusterCentreKMeans(int k, PointPtr p, std::vector<PointPtr> &centres);

  // Computes the cost of point p with centre centre
  double costOfPointToCenter(PointPtr p, PointPtr centre);
};
}
#endif //SESAME_INCLUDE_ALGORITHM_OFFLINECLUSTERING_KMEANS_HPP_
