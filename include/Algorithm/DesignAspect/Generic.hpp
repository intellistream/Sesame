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

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace SESAME {

template <typename W, typename D, typename O, typename R>
concept StreamClusteringConcept = requires {
  requires requires(W w, PointPtr p) {
    { w.Add(p) }
    ->std::same_as<bool>;
  };
  requires requires(D d, PointPtr p) {
    { d.Insert(p) }
    ->std::same_as<typename D::NodePtr>;
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
  void Init();
  void RunOnline(PointPtr input);
  void RunOffline(DataSinkPtr ptr);
  void store(std::string output_file, int dim, std::vector<PointPtr> results);

private:
  using Node = typename D::Node;
  using NodePtr = typename D::NodePtr;
  std::shared_ptr<W> w;
  std::shared_ptr<D> d;
  std::shared_ptr<O> o;
  std::shared_ptr<R> r;

  std::vector<NodePtr> outliers_;
  std::unordered_map<PointPtr, NodePtr> point_map_;
  std::unordered_map<NodePtr, std::unordered_set<PointPtr>> node_map_;
  NodePtr InsertOutliers(PointPtr point);
};

template <typename W, typename D, typename O, typename R>
StreamClustering<W, D, O, R>::~StreamClustering() {}

template <typename W, typename D, typename O, typename R>
StreamClustering<W, D, O, R>::StreamClustering(const param_t &cmd_params) {
  param = cmd_params;
}

template <typename W, typename D, typename O, typename R>
void StreamClustering<W, D, O, R>::Init() {
  w = GenericFactory::New<W>(param);
  d = GenericFactory::New<D>(param);
  o = GenericFactory::New<O>(param);
  r = GenericFactory::New<R>(param);
  d->Init();
  sum_timer.Tick();
}

template <typename W, typename D, typename O, typename R>
void StreamClustering<W, D, O, R>::RunOnline(PointPtr input) {
  constexpr bool has_delete = requires(W & w) { w.Delete(); };
  constexpr bool has_update = requires(W & w, NodePtr node) { w.Update(node); };
  // std::cerr << "#" << input->index << ":" << d->root()->cf.num << std::endl;
  if (w->Add(input)) {
    NodePtr node;
    out_timer.Tick();
    auto out = o->Check(input, d->clusters());
    out_timer.Tock();
    if (out) { // outlier
      node = InsertOutliers(input);
      out_timer.Tick();
      out = o->Check(node);
      out_timer.Tock();
      if (!out) {
        const auto [first, last] = std::ranges::remove_if(
            outliers_, [&](const NodePtr &n) { return n == node; });
        outliers_.erase(first, last);
        ds_timer.Tick();
        auto newNode = d->Insert(node);
        ds_timer.Tock();
        if constexpr (has_delete) {
          for (auto &p : node_map_[node]) {
            node_map_[newNode].insert(p);
            point_map_[p] = newNode;
          }
          node_map_.erase(node);
        }
        node = newNode;
      }
    } else {
      ds_timer.Tick();
      node = d->Insert(input);
      ds_timer.Tock();
    }
    if constexpr (has_delete) {
      node_map_[node].insert(input);
      point_map_[input] = node;
    }
  }
  if constexpr (has_update) {
    d->ForEach([&](NodePtr node) { w->Update(node); });
    std::ranges::for_each(outliers_, [&](NodePtr node) { w->Update(node); });
  }
  if constexpr (has_delete) {
    PointPtr point = w->Delete();
    if (point != nullptr) {
      assert(point_map_.contains(point));
      auto node = point_map_[point];
      ds_timer.Tick();
      node->Update(point->Reverse(), true);
      ds_timer.Tock();
      point_map_.erase(point);
      node_map_[node].erase(point);
      if (node->cf.num == 0) {
        // TODO
        const auto [first, last] = std::ranges::remove_if(
            outliers_, [&](const NodePtr &n) { return n->cf.num == 0; });
        outliers_.erase(first, last);
      }
    }
  }
}

template <typename W, typename D, typename O, typename R>
void StreamClustering<W, D, O, R>::RunOffline(DataSinkPtr ptr) {
  ref_timer.Tick();
  std::vector<PointPtr> onlineCenters;
  auto clusters = d->clusters();
  for (int i = 0; i < clusters.size(); i++) {
    auto centroid = GenericFactory::New<Point>(param.dim, i, 1, 0);
    for (int j = 0; j < param.dim; j++) {
      centroid->setFeatureItem(clusters[i]->cf.ls[j] / clusters[i]->cf.num, j);
    }
    onlineCenters.push_back(centroid);
  }
  r->Run(param, onlineCenters, ptr);
  ref_timer.Tock();
  sum_timer.Tock();
}

template <typename W, typename D, typename O, typename R>
StreamClustering<W, D, O, R>::NodePtr
StreamClustering<W, D, O, R>::InsertOutliers(PointPtr point) {
  if (outliers_.empty()) {
    auto node = GenericFactory::New<Node>(d, point);
    node->index = 0;
    outliers_.push_back(node);
    return node;
  } else {
    auto closest = CalcClosestNode(outliers_, point);
    if (closest.second < param.distance_threshold) {
      closest.first->Update(point);
      return closest.first;
    } else {
      auto node = GenericFactory::New<Node>(d, point);
      node->index = outliers_.size();
      outliers_.push_back(node);
      return node;
    }
  }
}

} // namespace SESAME

#endif // SESAME_INCLUDE_ALGORITHM_ALGORITHM_HPP_
