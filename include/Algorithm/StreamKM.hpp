//
// Created by tuidan on 2021/7/21.
//
#ifndef SESAME_SRC_ALGORITHM_STREAMKM_HPP_
#define SESAME_SRC_ALGORITHM_STREAMKM_HPP_

#include <Algorithm/Algorithm.hpp>
#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <iostream>
#include <Algorithm/OfflineClustering/KMeans.hpp>
namespace SESAME {

class StreamKMParameter: public AlgorithmParameters{
 public:
  int windowSize;
  int seed;
  int clusterNumber;
};

class StreamKM : public Algorithm {

 public:
  StreamKMParameter StreamKMParam;
  // initialize
  LandmarkWindowPtr window;
  std::vector<PointPtr> streamingCoreset;//intermediate results.
  KMeans km;//used for offline processing.
  StreamKM();
  ~StreamKM();

  void runOnlineClustering(const vector<PointPtr> &input) override;
  void runOfflineClustering(const std::vector<PointPtr> &input, vector<PointPtr> &output) override;

};
}

#endif //SESAME_SRC_ALGORITHM_STREAMKM_HPP_
