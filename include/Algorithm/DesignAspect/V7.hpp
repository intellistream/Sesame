//
// Created by tuidan on 2022/3/14.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V7_HPP_
#define SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V7_HPP_
#include <Algorithm/Algorithm.hpp>
#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <Algorithm/OfflineClustering/KMeans.hpp>
#include <Sinks/DataSink.hpp>
#include <Algorithm/DataStructure/CFTree.hpp>
#include <Utils/BenchmarkUtils.hpp>
namespace SESAME {

class V7Parameter : public AlgorithmParameters {
 public:
  int maxInternalNodes; // B
  int maxLeafNodes; // L
  double thresholdDistance; // T
  int landmark;
  double neighborDistance;
  double densityThreshold;
  int outlierClusterCapacity;
};

class V7 : public Algorithm {

 public:
  V7Parameter V7Param;
  std::shared_ptr<KMeans> kmeans; //used for offline initialization
  int leafMask = 0;
  NodePtr root;
  vector<NodePtr> outlierNodes;
  vector<NodePtr> clusterNodes;
  CFTreePtr cfTree;
  TimeMeter timerMeter;
  V7(param_t &cmd_params);

  ~V7();

  void Initilize() override;

  void runOnlineClustering(PointPtr input) override;

  void runOfflineClustering(DataSinkPtr sinkPtr) override;
 private:

  void forwardInsert(PointPtr point);
  void backwardEvolution(NodePtr &curNode, PointPtr &point, SESAME::NodePtr &cluster);
  void calculateCorDistance(vector<vector<double>> &distance, vector<NodePtr> &nodes);
  double calculateRadius(PointPtr &point, PointPtr &centroid);
  void selectChild(vector<NodePtr> &children, PointPtr &insertPoint, NodePtr &node);
  double clusterToClusterDist(NodePtr &nodeA, NodePtr &nodeB);
  void pointToClusterDist(PointPtr &insertPoint, NodePtr &node, double &dist);
  void calculateCentroid(CFPtr &cf, PointPtr &centroid);
  void updateNLS(NodePtr &node, PointPtr &point, bool updateAll);
  void initializeCF(CFPtr &cf, int dimension);
  void setCFToBlankNode(SESAME::NodePtr &curNode, SESAME::PointPtr &point);
  void addNodeNLSToNode(SESAME::NodePtr &child, SESAME::NodePtr &parent, bool updateAll);
  void clearChildParents(vector<SESAME::NodePtr> &children);
  bool checkoutDensity(SESAME::PointPtr &point);
  void insertPointIntoOutliers(SESAME::PointPtr &point);
  void checkOutlierTransferCluster(SESAME::NodePtr &Outlier);
};
}
#endif //SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V7_HPP_
