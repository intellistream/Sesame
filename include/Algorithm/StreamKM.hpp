// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by tuidan on 2021/7/21.
//
#ifndef SESAME_SRC_ALGORITHM_STREAMKM_HPP_
#define SESAME_SRC_ALGORITHM_STREAMKM_HPP_

#include <Algorithm/Algorithm.hpp>
#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <iostream>
#include <Algorithm/OfflineRefinement/KMeans.hpp>
#include <Sinks/DataSink.hpp>
#include <Utils/BenchmarkUtils.hpp>

namespace SESAME {

class StreamKMParameter : public AlgorithmParameters {
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
  vector <PointPtr> inputs;//buffered inputs.
  vector <PointPtr> streamingCoreset;//intermediate results.
  KMeans km;//used for offline processing.
  TimeMeter timerMeter;
  StreamKM(param_t &cmd_params);

  ~StreamKM();

  void Initilize() override;

  void runOnlineClustering(PointPtr input) override;

  void runOfflineClustering(DataSinkPtr sinkPtr) override;

 private:
  void dumpResults(vector <PointPtr> &centers, vector <vector<SESAME::PointPtr>> groups, DataSinkPtr ptr) const;
};
}

#endif //SESAME_SRC_ALGORITHM_STREAMKM_HPP_
