//
// Created by tuidan on 2021/7/21.
//
#include <Algorithm/Algorithm.hpp>
#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <iostream>

#ifndef SESAME_SRC_ALGORITHM_STREAMKM_HPP_
#define SESAME_SRC_ALGORITHM_STREAMKM_HPP_

namespace SESAME {
class StreamKM : public Algorithm<LandmarkWindow::Bucketmanager> {

 public:
  // initialize
  LandmarkWindow::Bucketmanager *manager;
  Point *streamingCoreset;

  StreamKM(int cluster_number);
  ~StreamKM();

  void initialWindow(LandmarkWindow::Bucketmanager *manager, int pointNumber,
                     int dimension, int coresetSize, int seed) override;
  void buildTimeWindow(int pointNumber, Point *p, Point *streamingCoreset,
                       LandmarkWindow::Bucketmanager *manager) override;
  void runOfflineClustering(int clusterNumber, int coresetSize, int dimension,
                            Point *streamingCoreset, Point *centresStreamingCoreset) override;
};
}

#endif //SESAME_SRC_ALGORITHM_STREAMKM_HPP_
