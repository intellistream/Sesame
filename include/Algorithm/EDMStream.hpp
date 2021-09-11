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

namespace SESAME {
class EDMParameter : public AlgorithmParameters {
 public:
  bool isInit = false;

  double a;
  double lamda;
  double beta;
  int cacheNum;
  double radius;

  double minDelta;
  int opt;
};

class EDMStream : public Algorithm {

 public:
  double deltaT;
  int actCluMaxNum = 1000;
  double minRho;
  double alpha;

  EDMParameter EDMParam;
  DPTreePtr dpTree;
  OutPtr outres;
  CachePtr cache;
  std::vector<ClusterPtr> clusters;

  EDMStream(param_t &cmd_params);
  ~EDMStream();
  void Initilize() override;
  void setMinDelta(double minDelta);

  void InitDP(double time);
  SESAME::DPNodePtr streamProcess(SESAME::PointPtr p, int opt, double time);
  double computeAlpha();
  double adjustMinDelta();
  void delCluster();
  SESAME::DPNodePtr retrive(SESAME::PointPtr p, int opt, double time);

  void runOnlineClustering(SESAME::PointPtr input) override;

  void runOfflineClustering(DataSinkPtr sinkPtr) override;
};
}
#endif //SESAME_INCLUDE_ALGORITHM_EDMSTREAM_HPP_
