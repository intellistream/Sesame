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

namespace SESAME {

template <typename W, typename D, typename O>
concept StreamClusteringConcept = requires {
  requires requires(W w, PointPtr p) {
    { w.addPoint(p) }
    ->std::same_as<bool>;
  };
  requires requires(D d, PointPtr p) {
    { d.insert(p) }
    ->std::same_as<void>;
  };
  requires requires(O o, PointPtr p, typename D::NodePtr n,
                    std::vector<typename D::NodePtr> vn) {
    { o.check(p, vn) }
    ->std::same_as<bool>;
    { o.check(n) }
    ->std::same_as<bool>;
  };
};

template <typename W, typename D, typename O>
requires StreamClusteringConcept<W, D, O> class StreamClustering
    : public Algorithm {
public:
  StreamClustering(const param_t &param);
  ~StreamClustering();
  void Initilize();
  void runOnlineClustering(PointPtr input);
  void runOfflineClustering(DataSinkPtr ptr);
  void store(std::string outputPath, int dimension,
             std::vector<PointPtr> results);

private:
  StreamClusteringParam param;
  using WindowPtr = std::shared_ptr<W>;
  using DataStructurePtr = std::shared_ptr<D>;
  using OutlierPtr = std::shared_ptr<O>;
  using NodePtr = typename D::NodePtr;
  WindowPtr w;
  DataStructurePtr d;
  OutlierPtr o;

  std::vector<NodePtr> outliers;
  static constexpr bool has_delpoint = requires(const W &w) { w.delPoint(); };
  static constexpr bool is_landmark = std::is_same<W, Landmark>::value;
  static constexpr bool is_cftree =
      std::is_same<D, ClusteringFeaturesTree>::value;
  NodePtr insertOutliers(PointPtr point);
};

template <typename W, typename D, typename O>
StreamClustering<W, D, O>::~StreamClustering() {}

template <typename W, typename D, typename O>
StreamClustering<W, D, O>::StreamClustering(const param_t &cmd_params) {
  param.pointNumber = cmd_params.pointNumber;
  param.dimension = cmd_params.dimension;
  param.clusterNumber = cmd_params.clusterNumber;

  if constexpr (is_landmark)
    param.landmark = cmd_params.landmark;
  if constexpr (is_cftree) {
    param.maxInternalNodes = cmd_params.maxInternalNodes;
    param.maxLeafNodes = cmd_params.maxLeafNodes;
    param.thresholdDistance = cmd_params.thresholdDistance;
  }
}

template <typename W, typename D, typename O>
void StreamClustering<W, D, O>::Initilize() {
  w = GenericFactory::create<W>(param);
  d = GenericFactory::create<D>(param);
  o = GenericFactory::create<O>(param);
}

template <typename W, typename D, typename O>
void StreamClustering<W, D, O>::runOnlineClustering(PointPtr input) {
  if (w->addPoint(input)) {
    if (o->check(input, d->getClusterNodes())) {
      auto node = insertOutliers(input);
      if (!o->check(node)) {
        std::ranges::remove_if(outliers,
                               [&](const NodePtr &n) { return n == node; });
        d->insert(node);
      }
    } else {
      d->insert(input);
    }
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
StreamClustering<W, D, O>::NodePtr
StreamClustering<W, D, O>::insertOutliers(PointPtr point) {
  if (outliers.empty()) {
    auto node = d->newNode();
    node->update(point);
    node->index = 0;
    outliers.push_back(node);
    return node;
  } else {
    auto closest = closestNode(outliers, point);
    if (closest.second < param.thresholdDistance) {
      closest.first->update(point);
    } else {
      auto node = d->newNode();
      node->update(point);
      node->index = outliers.size();
      outliers.push_back(node);
    }
    return closest.first;
  }
}

} // namespace SESAME

#endif // SESAME_INCLUDE_ALGORITHM_ALGORITHM_HPP_
