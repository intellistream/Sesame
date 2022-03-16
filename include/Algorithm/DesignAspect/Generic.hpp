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
concept StreamClusteringConcept = requires {
  requires requires(W w, PointPtr p) {
    { w.addPoint(p) }
    ->std::same_as<bool>;
  };
  requires requires(D d, PointPtr p, std::vector<typename D::NodePtr> vn) {
    { d.insert(p, vn, vn) }
    ->std::same_as<void>;
  };
};

template <typename W, typename D, typename O>
requires StreamClusteringConcept<W, D, O> class StreamClustering
    : public Algorithm {
public:
  // StreamClustering();
  StreamClustering(const param_t &param);
  ~StreamClustering();
  void Initilize();
  void runOnlineClustering(PointPtr input);
  void runOfflineClustering(DataSinkPtr ptr);
  void store(std::string outputPath, int dimension,
             std::vector<PointPtr> results);

private:
  void forwardInsert(PointPtr point);
  StreamClusteringParam Param;
  using WindowPtr = shared_ptr<W>;
  using DataPtr = shared_ptr<D>;
  using OutlierPtr = shared_ptr<O>;
  WindowPtr w;
  DataPtr d;
  OutlierPtr o;

  std::vector<typename D::NodePtr> clusterNodes, outliers;
  static constexpr bool has_delpoint = requires(const W &w) { w.delPoint(); };
  static constexpr bool is_landmark = std::is_same<W, Landmark>::value;
  static constexpr bool is_cftree =
      std::is_same<D, ClusteringFeaturesTree>::value;
};

template <typename W, typename D, typename O>
StreamClustering<W, D, O>::~StreamClustering() {}

template <typename W, typename D, typename O>
StreamClustering<W, D, O>::StreamClustering(const param_t &cmd_params) {
  Param.pointNumber = cmd_params.pointNumber;
  Param.dimension = cmd_params.dimension;
  Param.clusterNumber = cmd_params.clusterNumber;

  if constexpr (is_landmark)
    Param.landmark = cmd_params.landmark;
  if constexpr (is_cftree)
    Param.maxInternalNodes = cmd_params.maxInternalNodes;
  Param.maxLeafNodes = cmd_params.maxLeafNodes;
  Param.thresholdDistance = cmd_params.thresholdDistance;
}

template <typename W, typename D, typename O>
void StreamClustering<W, D, O>::Initilize() {
  w = GenericFactory::create<W>(Param);
  d = GenericFactory::create<D>(Param);
  o = GenericFactory::create<O>(Param);
}

template <typename W, typename D, typename O>
void StreamClustering<W, D, O>::runOnlineClustering(PointPtr input) {
  if (w->addPoint(input)) {
    forwardInsert(input->copy());
  }
  if constexpr (has_delpoint) {
    if (w->delPoint()) {
      std::cout << "delete" << std::endl;
    }
  }
}

template <typename W, typename D, typename O>
void StreamClustering<W, D, O>::runOfflineClustering(DataSinkPtr ptr) {}

template <typename W, typename D, typename O>
void StreamClustering<W, D, O>::store(std::string outputPath, int dimension,
                                      std::vector<PointPtr> results) {}

template <typename W, typename D, typename O>
void StreamClustering<W, D, O>::forwardInsert(PointPtr point) {
  d->insert(point, clusterNodes, outliers);
}

} // namespace SESAME

#endif // SESAME_INCLUDE_ALGORITHM_ALGORITHM_HPP_
