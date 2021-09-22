// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by tuidan on 2021/7/22.
//

#ifndef SESAME_INCLUDE_ALGORITHM_ALGORITHM_HPP_
#define SESAME_INCLUDE_ALGORITHM_ALGORITHM_HPP_
#include <Algorithm/DataStructure/Point.hpp>
#include <Sinks/DataSink.hpp>
#include <string>
#include <vector>
#include <memory>
using namespace std;
namespace SESAME {

enum algoType { BirchType, StreamKMeansType, CluStreamType, DenStreamType, DBStreamType, EDMStreamType};

class Algorithm;
typedef std::shared_ptr<Algorithm> AlgorithmPtr;

class AlgorithmParameters {
 public:
  int pointNumber;
  int dimension;
};

class Algorithm {
 public:
  Algorithm() = default;
  virtual ~Algorithm() = default;
  virtual void Initilize() = 0;
  virtual void runOnlineClustering(SESAME::PointPtr input) = 0;
  virtual void runOfflineClustering(SESAME::DataSinkPtr ptr) = 0;
  void store(std::string outputPath, int dimension, std::vector<PointPtr> results);
};
}

#endif //SESAME_INCLUDE_ALGORITHM_ALGORITHM_HPP_
