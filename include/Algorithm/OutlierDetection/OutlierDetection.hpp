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
  double outlierDistanceThreshold_;
  int outlierClusterCapacity_;

public:
  DistanceDetection(const StreamClusteringParam &param)
      : outlierDistanceThreshold_(param.outlierDistanceThreshold),
        outlierClusterCapacity_(param.outlierClusterCapacity) {}
  template <NodeConcept T> bool Check(PointPtr point, std::vector<T> nodes) {
    if (nodes.empty())
      return false;
    auto dist = CalcClosestNode(nodes, point).second;
    return dist > outlierDistanceThreshold_;
  }
  template <NodeConcept T> bool Check(T node) {
    return node->cf.num < outlierClusterCapacity_;
  }
};

class DensityDetection : public OutlierDetection {
  double neighborDistance_, densityThreshold_;
  int outlierClusterCapacity_;

public:
  DensityDetection(const StreamClusteringParam &param)
      : neighborDistance_(param.neighborDistance),
        densityThreshold_(param.densityThreshold),
        outlierClusterCapacity_(param.outlierClusterCapacity) {}
  template <NodeConcept T> bool Check(PointPtr point, std::vector<T> nodes) {
    std::vector<T> neighborNodes;
    int neighborDensity = 0, neighborNeighborDensity = 0;
    for (auto node : nodes) {
      auto dist = point->radius(node->centroid());
      if (dist < neighborDistance_) {
        neighborNodes.push_back(node);
        neighborDensity += node->cf.num;
      }
    }
    for (auto neighbor : neighborNodes) {
      for (auto node : nodes) {
        if (CalcClusterDist(node, neighbor) < neighborDistance_) {
          neighborNeighborDensity += node->cf.num;
        }
      }
    }
    if (neighborNeighborDensity == 0)
      return false;
    else {
      return (double)neighborDensity / neighborNeighborDensity <=
             densityThreshold_;
    }
  }
  template <NodeConcept T> bool Check(T node) {
    return node->cf.num < outlierClusterCapacity_;
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
