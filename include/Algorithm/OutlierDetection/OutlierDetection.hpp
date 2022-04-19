//
// Created by 1124a on 2021/11/2.
//

#ifndef SESAME_INCLUDE_ALGORITHM_OUTLIERDETECTION_OUTLIERDETECTION_HPP_
#define SESAME_INCLUDE_ALGORITHM_OUTLIERDETECTION_OUTLIERDETECTION_HPP_

#include "Algorithm/DataStructure/CFTree.hpp"
#include "Algorithm/DesignAspect/Param.hpp"

#include <memory>

namespace SESAME {
class OutlierDetection;
typedef std::shared_ptr<OutlierDetection> OutlierDetectionPtr;

class OutlierDetection {};

class DistanceDetection : public OutlierDetection {
private:
  const double outlier_distance_threshold_;
  const int outlier_cap_;

public:
  DistanceDetection(const StreamClusteringParam &param)
      : outlier_distance_threshold_(param.outlier_distance_threshold),
        outlier_cap_(param.outlier_cap) {}
  template <NodeConcept T> bool Check(PointPtr point, std::vector<T> nodes) {
    if (nodes.empty())
      return false;
    auto dist = CalcClosestNode(nodes, point).second;
    return dist > outlier_distance_threshold_;
  }
  template <NodeConcept T> bool Check(T node) {
    return node->cf.num < outlier_cap_;
  }
};

class DensityDetection : public OutlierDetection {
  const double neighbor_distance_, outlier_density_threshold_;
  const int outlier_cap_;

public:
  DensityDetection(const StreamClusteringParam &param)
      : neighbor_distance_(param.neighbor_distance),
        outlier_density_threshold_(param.outlier_density_threshold),
        outlier_cap_(param.outlier_cap) {}
  template <NodeConcept T> bool Check(PointPtr point, std::vector<T> nodes) {
    std::vector<T> neighborNodes;
    int neighborDensity = 0, neighborNeighborDensity = 0;
    for (auto node : nodes) {
      auto dist = point->Radius(node->Centroid());
      if (dist < neighbor_distance_) {
        neighborNodes.push_back(node);
        neighborDensity += node->cf.num;
      }
    }
    for (auto neighbor : neighborNodes) {
      for (auto node : nodes) {
        if (CalcClusterDist(node, neighbor) < neighbor_distance_) {
          neighborNeighborDensity += node->cf.num;
        }
      }
    }
    if (neighborNeighborDensity == 0)
      return false;
    else
      return (double)neighborDensity / neighborNeighborDensity <
             outlier_density_threshold_;
  }
  template <NodeConcept T> bool Check(T node) {
    return node->cf.num < outlier_cap_;
  }
};

class NoDetection : public OutlierDetection {

public:
  NoDetection(const StreamClusteringParam &param) {}
  template <NodeConcept T> bool Check(PointPtr point, std::vector<T> nodes) {
    return false;
  }
  template <NodeConcept T> bool Check(T node) { return false; }
};

} // namespace SESAME
#endif // SESAME_INCLUDE_ALGORITHM_OUTLIERDETECTION_OUTLIERDETECTION_HPP_
