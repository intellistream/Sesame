//
// Created by tuidan on 2022/3/10.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V3_HPP_
#define SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V3_HPP_
#include <Algorithm/Algorithm.hpp>
#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <Algorithm/OfflineClustering/KMeans.hpp>
#include <Sinks/DataSink.hpp>
#include <Algorithm/DataStructure/CFTree.hpp>
#include <Utils/BenchmarkUtils.hpp>
namespace SESAME {

class V3Parameter : public AlgorithmParameters {
 public:
  int maxInternalNodes; // B
  int maxLeafNodes; // L
  double thresholdDistance; // T
  int landmark;
};

class V3 : public Algorithm {

 public:
  V3Parameter V3Param;
  std::shared_ptr<KMeans> kmeans; //used for offline initialization
  int leafMask = 0;
  NodePtr root;
  vector<NodePtr> clusterNodes;
  CFTreePtr cfTree;
  TimeMeter timerMeter;
  V3(param_t &cmd_params);

  ~V3();

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
#endif //SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V3_HPP_
