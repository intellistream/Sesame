//
// Created by tuidan on 2022/3/10.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_CLUESTREAM_HPP_
#define SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_CLUESTREAM__HPP_
#include <Algorithm/Algorithm.hpp>
#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <Algorithm/OfflineRefinement/KMeans.hpp>
#include <Sinks/DataSink.hpp>
#include <Algorithm/DataStructure/CFTree.hpp>
#include <Utils/BenchmarkUtils.hpp>

#include <chrono>
namespace SESAME {

class CluEStream : public Algorithm {
 public:
  using clock_t = std::chrono::_V2::system_clock::time_point;
  std::shared_ptr<KMeans> kmeans; //used for offline initialization
  int leafMask = 0;
  NodePtr root;
  int search_delete_times = 0;
  int out_delete_times = 0;
  int out_split_times = 0;
  int out_merge_times = 0;
  vector<NodePtr> clusterNodes;
  vector<NodePtr> outlierNodes;
  CFTreePtr cfTree;
  TimeMeter timerMeter;
//  clock_t updateTimeStamp = 0;
//  int updateGap = 100;
  long outdatedGap = 50000000;
  clock_t lastTime;
  int deleteDensity = 30;
  double mergeDistance = 40;
  CluEStream(param_t &cmd_params);

  ~CluEStream();

  void Init() override;

  void RunOnline(PointPtr input) override;

  void RunOffline(DataSinkPtr sinkPtr) override;
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
  void initializeCF(CFPtr &cf, int dim);
  void setCFToBlankNode(SESAME::NodePtr &curNode, SESAME::PointPtr &point);
  void addNodeNLSToNode(SESAME::NodePtr &child, SESAME::NodePtr &parent, bool updateAll);
  void clearChildParents(vector<SESAME::NodePtr> &children);

  bool checkoutOutlier(SESAME::PointPtr &point);
  void insertPointIntoOutliers(SESAME::PointPtr &point);
  void checkOutlierTransferCluster(SESAME::NodePtr &outCluster);
};
}
#endif //SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_CLUESTREAM_HPP_
