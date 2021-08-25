//
// Created by tuidan on 2021/8/24.
//

#ifndef SESAME_INCLUDE_ALGORITHM_BIRCH_HPP_
#define SESAME_INCLUDE_ALGORITHM_BIRCH_HPP_
#include <Algorithm/Algorithm.hpp>
#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <Algorithm/OfflineClustering/KMeans.hpp>
#include <Sinks/DataSink.hpp>
#include <Algorithm/DataStructure/CFTree.hpp>
namespace SESAME {

class BirchParameter : public AlgorithmParameters {
 public:
  int maxInternalNodes; // B
  int maxLeafNodes; // L
  double thresholdDistance; // T
};

class Birch : public Algorithm {

 public:
  BirchParameter BirchParam;
  std::shared_ptr<KMeans> kmeans; //used for offline initialization
  int leafMask = 0;
  NodePtr root;
  vector<NodePtr> leafNodes;
  CFTreePtr cfTree;
  Birch();

  ~Birch();

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
#endif //SESAME_INCLUDE_ALGORITHM_BIRCH_HPP_
