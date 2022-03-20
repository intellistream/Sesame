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
  int distanceOutliers;

public:
  DistanceDetection(const StreamClusteringParam &param)
      : distanceOutliers(param.distanceOutliers) {}
  template <NodeConcept T> bool check(T node, PointPtr point) {
    if (node->cf.numPoints < distanceOutliers) {
      return true;
    } else {
      return false;
    }
  }
};

class DensityDetection : public OutlierDetection {
public:
  DensityDetection(const StreamClusteringParam &param) {}
  template <NodeConcept T> bool check(T node, PointPtr point) {
    // vector<T> neighborNodes;
    // auto distance = 0.0, neighborDensity = 0.0, neighborNeighborDensity =
    // 0.0;
    // TODO
    return false;
    // for (auto cluster : this->clusterNodes) {
    //   pointToClusterDist(point, cluster, distance);
    //   if (distance < this->V7Param.neighborDistance) {
    //     neighborNodes.push_back(cluster);
    //     neighborDensity += cluster->getCF()->getN();
    //   }
    // }
    // for (auto neighbor : neighborNodes) {
    //   for (auto cluster : this->clusterNodes) {
    //     if (clusterToClusterDist(cluster, neighbor) <
    //         this->V7Param.neighborDistance) {
    //       neighborNeighborDensity += cluster->getCF()->getN();
    //     }
    //   }
    // }
    // if (neighborNeighborDensity == 0)
    //   return true;
    // else {
    //   double densityScore = neighborDensity / neighborNeighborDensity;
    //   if (densityScore <= this->V7Param.densityThreshold)
    //     return false;
    //   else
    //     return true;
    // }
  }
};

class NoDetection : public OutlierDetection {
public:
  NoDetection(const StreamClusteringParam &param) {}
  template <NodeConcept T> bool check(T node) { return false; }
};

} // namespace SESAME
#endif // SESAME_INCLUDE_ALGORITHM_OUTLIERDETECTION_OUTLIERDETECTION_HPP_
