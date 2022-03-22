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

template <typename W, typename D, typename O, typename R>
concept StreamClusteringConcept = requires {
  requires requires(W w, PointPtr p) {
    { w.Add(p) }
    ->std::same_as<bool>;
    { w.Del(p) }
    ->std::same_as<bool>;
  };
  requires requires(D d, PointPtr p) {
    { d.Insert(p) }
    ->std::same_as<void>;
  };
  requires requires(O o, PointPtr p, typename D::NodePtr n,
                    std::vector<typename D::NodePtr> vn) {
    { o.Check(p, vn) }
    ->std::same_as<bool>;
    { o.Check(n) }
    ->std::same_as<bool>;
  };
};

template <typename W, typename D, typename O, typename R>
requires StreamClusteringConcept<W, D, O, R> class StreamClustering
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
  using Node = typename D::Node;
  using NodePtr = typename D::NodePtr;
  StreamClusteringParam param;
  std::shared_ptr<W> w;
  std::shared_ptr<D> d;
  std::shared_ptr<O> o;
  std::shared_ptr<R> r;

  std::vector<NodePtr> outliers_;
  static constexpr bool is_landmark = std::is_same<W, Landmark>::value;
  static constexpr bool is_cftree =
      std::is_same<D, ClusteringFeaturesTree>::value;
  NodePtr InsertOutliers(PointPtr point);
};

template <typename W, typename D, typename O, typename R>
StreamClustering<W, D, O, R>::~StreamClustering() {}

template <typename W, typename D, typename O, typename R>
StreamClustering<W, D, O, R>::StreamClustering(const param_t &cmd_params) {
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

template <typename W, typename D, typename O, typename R>
void StreamClustering<W, D, O, R>::Initilize() {
  w = GenericFactory::New<W>(param);
  d = GenericFactory::New<D>(param);
  o = GenericFactory::New<O>(param);
  r = GenericFactory::New<R>(param);
}

template <typename W, typename D, typename O, typename R>
void StreamClustering<W, D, O, R>::runOnlineClustering(PointPtr input) {
  if (w->Add(input)) {
    if (o->Check(input, d->clusters())) {
      auto node = InsertOutliers(input);
      if (!o->Check(node)) {
        std::ranges::remove_if(outliers_,
                               [&](const NodePtr &n) { return n == node; });
        d->Insert(node);
      }
    } else {
      d->Insert(input);
    }
  }
  if (w->Del(input)) {
    // TODO
    std::cout << "delete" << std::endl;
  }
}

template <typename W, typename D, typename O, typename R>
void StreamClustering<W, D, O, R>::runOfflineClustering(DataSinkPtr ptr) {
  std::vector<PointPtr> onlineCenters;
  auto clusters = d->clusters();
  for (int i = 0; i < clusters.size(); i++) {
    auto centroid = GenericFactory::New<Point>(i, 1, param.dimension, 0);
    for (int j = 0; j < param.dimension; j++) {
      centroid->setFeatureItem(
          clusters[i]->cf.ls[j] / clusters[i]->cf.numPoints, j);
    }
    onlineCenters.push_back(centroid);
  }
  r->Run(param, onlineCenters, ptr);
}

template <typename W, typename D, typename O, typename R>
void StreamClustering<W, D, O, R>::store(std::string outputPath, int dimension,
                                         std::vector<PointPtr> results) {}

template <typename W, typename D, typename O, typename R>
StreamClustering<W, D, O, R>::NodePtr
StreamClustering<W, D, O, R>::InsertOutliers(PointPtr point) {
  if (outliers_.empty()) {
    auto node = GenericFactory::New<Node>(point);
    node->index = 0;
    outliers_.push_back(node);
    return node;
  } else {
    auto closest = CalcClosestNode(outliers_, point);
    if (closest.second < param.thresholdDistance) {
      closest.first->Update(point);
    } else {
      auto node = GenericFactory::New<Node>(point);
      node->index = outliers_.size();
      outliers_.push_back(node);
    }
    return closest.first;
  }
}

} // namespace SESAME

#endif // SESAME_INCLUDE_ALGORITHM_ALGORITHM_HPP_
