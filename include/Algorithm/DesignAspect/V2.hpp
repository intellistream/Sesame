//
// Created by tuidan on 2022/3/10.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V2_HPP_
#define SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V2_HPP_
#include <Algorithm/Algorithm.hpp>
#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <Sinks/DataSink.hpp>
#include <Algorithm/DataStructure/CFTree.hpp>
#include <Utils/BenchmarkUtils.hpp>
#include <Algorithm/OfflineClustering/DBSCAN.hpp>

namespace SESAME {

class V2Parameter : public AlgorithmParameters {
 public:
  int maxInternalNodes; // B
  int maxLeafNodes; // L
  double thresholdDistance; // T
  unsigned int minPoints;//minimum point of core point in DBSCAN
  double epsilon;//maximum distance if point belongs to the density area of core point
};

class V2 : public Algorithm {

 public:
  V2Parameter V2Param;
  int leafMask = 0;
  NodePtr root;
  std::shared_ptr<DBSCAN> dbscan; //used for initialization and offline re-clustering
  vector<NodePtr> clusterNodes;
  CFTreePtr cfTree;
  TimeMeter timerMeter;
  V2(param_t &cmd_params);

  ~V2();

  void Initilize() override;

  void runOnlineClustering(PointPtr input) override;

  void runOfflineClustering(DataSinkPtr sinkPtr) override;
 private:

  void forwardInsert(PointPtr point);
  void backwardEvolution(NodePtr &curNode, PointPtr &point);
  void calculateCorDistance(vector<vector<double>> &distance, vector<NodePtr> &nodes);
  double calculateRadius(PointPtr &point, PointPtr &centroid);
  void selectChild(vector<NodePtr> &children, PointPtr &insertPoint, NodePtr &node);
  double clusterToClusterDist(NodePtr &nodeA, NodePtr &nodeB);
  void pointToClusterDist(PointPtr &insertPoint, NodePtr &node, double &dist);
  void calculateCentroid(CFPtr &cf, PointPtr &centroid);
  void updateNLS(NodePtr &node, PointPtr &point, bool updateAll);
  void initializeCF(CFPtr &cf, int dimension);
  void setCFToBlankNode(SESAME::NodePtr &curNode, SESAME::PointPtr &point);
  void addNodeNLSToNode(SESAME::NodePtr &child, SESAME::NodePtr &parent);
  void clearChildParents(vector<SESAME::NodePtr> &children);
};
}
#endif //SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V2_HPP_
