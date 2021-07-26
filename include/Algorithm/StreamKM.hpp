//
// Created by tuidan on 2021/7/21.
//
#include <Algorithm/Algorithm.hpp>
#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <iostream>

#ifndef SESAME_SRC_ALGORITHM_STREAMKM_HPP_
#define SESAME_SRC_ALGORITHM_STREAMKM_HPP_

#include <Algorithm/WindowModel/LandmarkWindow.hpp>
namespace SESAME {
class StreamKM : public Algorithm {

 public:
  // initialize
  LandmarkWindowPtr window;
  Point streamingCoreset;

  StreamKM();
  ~StreamKM();

  void initialWindow(int pointNumber,
                     int dimension, int coresetSize, int seed) override;
  void buildTimeWindow(int pointNumber, vector<Point> &input) override;
  void runOfflineClustering(int clusterNumber, int coresetSize,
                            int dimension, vector<Point> &output) override;

};

typedef std::shared_ptr<StreamKM> StreamKMAlgorithmPtr;

}

#endif //SESAME_SRC_ALGORITHM_STREAMKM_HPP_
