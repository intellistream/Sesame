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
  double outlierDistanceThreshold;
  int outlierClusterCapacity;

public:
  DistanceDetection(const StreamClusteringParam &param)
      : outlierDistanceThreshold(param.outlierDistanceThreshold),
        outlierClusterCapacity(param.outlierClusterCapacity) {}
  template <NodeConcept T> bool check(PointPtr point, std::vector<T> nodes) {
    if (nodes.empty())
      return false;
    auto dist = closestNode(nodes, point).second;
    return dist > outlierDistanceThreshold;
  }
  template <NodeConcept T> bool check(T node) {
    return node->cf.numPoints < outlierClusterCapacity;
  }
};

class DensityDetection : public OutlierDetection {
  double neighborDistance, densityThreshold;
  int outlierClusterCapacity;

public:
  DensityDetection(const StreamClusteringParam &param)
      : neighborDistance(param.neighborDistance),
        densityThreshold(param.densityThreshold),
        outlierClusterCapacity(param.outlierClusterCapacity) {}
  template <NodeConcept T> bool check(PointPtr point, std::vector<T> nodes) {
    std::vector<T> neighborNodes;
    int neighborDensity = 0, neighborNeighborDensity = 0;
    for (auto node : nodes) {
      auto dist = point->radius(node->centroid());
      if (dist < neighborDistance) {
        neighborNodes.push_back(node);
        neighborDensity += node->cf.numPoints;
      }
    }
    for (auto neighbor : neighborNodes) {
      for (auto node : nodes) {
        if (clusterDistance(node, neighbor) < neighborDistance) {
          neighborNeighborDensity += node->cf.numPoints;
        }
      }
    }
    if (neighborNeighborDensity == 0)
      return false;
    else {
      return (double)neighborDensity / neighborNeighborDensity <=
             densityThreshold;
    }
  }
  template <NodeConcept T> bool check(T node) {
    return node->cf.numPoints < outlierClusterCapacity;
  }
};

class NoDetection : public OutlierDetection {
public:
  NoDetection(const StreamClusteringParam &param) {}
  template <NodeConcept T> bool check(T node) { return false; }
};

} // namespace SESAME
#endif // SESAME_INCLUDE_ALGORITHM_OUTLIERDETECTION_OUTLIERDETECTION_HPP_
