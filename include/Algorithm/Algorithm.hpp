// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by tuidan on 2021/7/22.
//

#ifndef SESAME_INCLUDE_ALGORITHM_ALGORITHM_HPP_
#define SESAME_INCLUDE_ALGORITHM_ALGORITHM_HPP_

#include "Algorithm/DataStructure/Point.hpp"
#include "Sinks/DataSink.hpp"

#include <memory>
#include <string>
#include <vector>

using namespace std;

namespace SESAME {

enum AlgoType {
  BirchType,
  StreamKMeansType,
  CluStreamType,
  DenStreamType,
  DBStreamType,
  EDMStreamType,
  DStreamType,
  V1Stream,
  V2Stream,
  V3Stream,
  V4Stream,
  V5Stream,
  V6Stream,
  V7Stream,
  V8Stream,
  V9Stream,
  Generic
};

class Algorithm;
typedef std::shared_ptr<Algorithm> AlgorithmPtr;

class AlgorithmParameters {
public:
  int num_points;
  int dim;
};

class Algorithm {
public:
  Algorithm() = default;
  virtual ~Algorithm() = default;
  virtual void Init() = 0;
  virtual void RunOnline(SESAME::PointPtr input) = 0;
  virtual void RunOffline(SESAME::DataSinkPtr ptr) = 0;
  void store(std::string output_file, int dim, std::vector<PointPtr> results);
};
} // namespace SESAME

#endif // SESAME_INCLUDE_ALGORITHM_ALGORITHM_HPP_
