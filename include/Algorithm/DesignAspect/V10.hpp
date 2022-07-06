//
// Created by tuidan on 2021/9/1.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V10_HPP_
#define SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V10_HPP_
#include <Algorithm/Algorithm.hpp>
#include <Algorithm/DataStructure/DPNode.hpp>
#include <Sinks/DataSink.hpp>
#include <Algorithm/DataStructure/Cache.hpp>
#include <Algorithm/DataStructure/DataStructureFactory.hpp>
#include <Algorithm/DataStructure/OutlierReservoir.hpp>
#include <Algorithm/DataStructure/DPTree.hpp>
#include <Utils/BenchmarkUtils.hpp>
#include <unordered_set>

namespace SESAME {
class V10 : public Algorithm {

 public:
  StreamClusteringParam V10Param;
  double deltaT;
  int actCluMaxNum = 10000;
  double minRho;
  double alpha;
  bool isInit = false;

  DPTreePtr dpTree;
  OutPtr outres;
  CachePtr cache;
  std::unordered_set<ClusterPtr> clusters;

  void Init() override;
  void setMinDelta(double minDelta);

  void InitDP(double time);
  void CountNode(const SESAME::DPNodePtr &node, int &num);
  SESAME::DPNodePtr streamProcess(SESAME::PointPtr p, int opt, double time);
  double computeAlpha();
  double adjustMinDelta();
  void delCluster();
  SESAME::DPNodePtr retrive(SESAME::PointPtr p, int opt, double time);

  void RunOnline(SESAME::PointPtr input) override;

  void RunOffline(DataSinkPtr sinkPtr) override;
};
}
#endif //SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V10_HPP_
