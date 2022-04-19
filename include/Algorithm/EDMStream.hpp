//
// Created by tuidan on 2021/9/1.
//

#ifndef SESAME_INCLUDE_ALGORITHM_EDMSTREAM_HPP_
#define SESAME_INCLUDE_ALGORITHM_EDMSTREAM_HPP_
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
class EDMParameter : public AlgorithmParameters {
 public:
  bool isInit = false;

  double alpha;
  double lamda;
  double beta;
  int num_cache;
  double radius;

  double minDelta;
  int opt;
};

class EDMStream : public Algorithm {

 public:
  double deltaT;
  int actCluMaxNum = 10000;
  double minRho;
  double alpha;

  EDMParameter EDMParam;
  DPTreePtr dpTree;
  OutPtr outres;
  CachePtr cache;
  TimeMeter timerMeter;
  std::unordered_set<ClusterPtr> clusters;

  EDMStream(param_t &cmd_params);
  ~EDMStream();
  void Init() override;
  void setMinDelta(double minDelta);

  void InitDP(double time);
  SESAME::DPNodePtr streamProcess(SESAME::PointPtr p, int opt, double time);
  double computeAlpha();
  double adjustMinDelta();
  void delCluster();
  SESAME::DPNodePtr retrive(SESAME::PointPtr p, int opt, double time);

  void RunOnline(SESAME::PointPtr input) override;

  void RunOffline(DataSinkPtr sinkPtr) override;
};
}
#endif //SESAME_INCLUDE_ALGORITHM_EDMSTREAM_HPP_
