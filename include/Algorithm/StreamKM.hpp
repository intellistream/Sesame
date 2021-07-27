//
// Created by tuidan on 2021/7/21.
//
#include <Algorithm/Algorithm.hpp>
#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <iostream>

#ifndef SESAME_SRC_ALGORITHM_STREAMKM_HPP_
#define SESAME_SRC_ALGORITHM_STREAMKM_HPP_

#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <Algorithm/OfflineClustering/KMeans.hpp>
namespace SESAME {
class StreamKM : public Algorithm {

 public:
  // initialize
  LandmarkWindowPtr window;
  std::vector<PointPtr> streamingCoreset;//intermediate results.
  KMeans km;//used for offline processing.
  StreamKM();
  ~StreamKM();

  void initialWindow(int pointNumber,
                     int dimension, int coresetSize, int seed) override;
  void buildTimeWindow(int pointNumber, const vector<PointPtr> &input) override;
  void runOfflineClustering(int clusterNumber, int coresetSize,
                            int dimension, vector<PointPtr> &output) override;

};
}

#endif //SESAME_SRC_ALGORITHM_STREAMKM_HPP_
