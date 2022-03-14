// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by wzru on 2022/3/13.
//

#ifndef SESAME_INCLUDE_ALGORITHM_GENERIC_HPP_
#define SESAME_INCLUDE_ALGORITHM_GENERIC_HPP_

#include "Algorithm/Algorithm.hpp"
#include "Algorithm/DataStructure/GenericFactory.hpp"
#include "Algorithm/DataStructure/Point.hpp"
#include "Algorithm/DesignAspect/Param.hpp"
#include "Sinks/DataSink.hpp"
#include "Utils/BenchmarkUtils.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

namespace SESAME {

template <typename W, typename D, typename O>
class StreamClusteringAlgorithm : public Algorithm {
public:
  // StreamClusteringAlgorithm();
  StreamClusteringAlgorithm(const param_t &param);
  ~StreamClusteringAlgorithm();
  void Initilize();
  void runOnlineClustering(PointPtr input);
  void runOfflineClustering(DataSinkPtr ptr);
  void store(std::string outputPath, int dimension,
             std::vector<PointPtr> results);

private:
  void forwardInsert(PointPtr point);
  StreamClusteringAlgorithmParameters Param;
  using WindowPtr = shared_ptr<W>;
  using DataPtr = shared_ptr<D>;
  using OutlierPtr = shared_ptr<O>;
  WindowPtr w;
  DataPtr d;
  OutlierPtr o;
};

template <typename W, typename D, typename O>
StreamClusteringAlgorithm<W, D, O>::~StreamClusteringAlgorithm() {}

template <typename W, typename D, typename O>
StreamClusteringAlgorithm<W, D, O>::StreamClusteringAlgorithm(
    const param_t &cmd_params) {
  Param.pointNumber = cmd_params.pointNumber;
  Param.dimension = cmd_params.dimension;
  Param.clusterNumber = cmd_params.clusterNumber;
}

template <typename W, typename D, typename O>
void StreamClusteringAlgorithm<W, D, O>::Initilize() {
  w = GenericFactory::create<W>(Param);
  d = GenericFactory::create<D>(Param);
  o = GenericFactory::create<O>(Param);
}

template <typename W, typename D, typename O>
void StreamClusteringAlgorithm<W, D, O>::runOnlineClustering(PointPtr input) {
  if (w->addPoint(input)) {
    forwardInsert(input->copy());
  }
  if (w->delPoint()) {
    std::cout << "delete" << std::endl;
  }
}

template <typename W, typename D, typename O>
void StreamClusteringAlgorithm<W, D, O>::runOfflineClustering(DataSinkPtr ptr) {
}

template <typename W, typename D, typename O>
void StreamClusteringAlgorithm<W, D, O>::store(std::string outputPath,
                                               int dimension,
                                               std::vector<PointPtr> results) {}

template <typename W, typename D, typename O>
void StreamClusteringAlgorithm<W, D, O>::forwardInsert(PointPtr point) {}

} // namespace SESAME

#endif // SESAME_INCLUDE_ALGORITHM_ALGORITHM_HPP_
