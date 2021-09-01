//
// Created by tuidan on 2021/9/1.
//

#ifndef SESAME_INCLUDE_ALGORITHM_EDMSTREAM_HPP_
#define SESAME_INCLUDE_ALGORITHM_EDMSTREAM_HPP_
#include <Algorithm/Algorithm.hpp>
#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <Algorithm/OfflineClustering/KMeans.hpp>
#include <Sinks/DataSink.hpp>
#include <Algorithm/DataStructure/CFTree.hpp>
#include <Utils/BenchmarkUtils.hpp>
namespace SESAME {

class EDMParameter : public AlgorithmParameters {
 public:
  DPTree dpTree;
  OutlierReservoir outres;
  Cache cache;
  std::vector<Cluster> clusters;

  bool isInit = false;

  double a;
  double lamda;
  double deltaT;
  double alpha;
  double beta;

  int cacheNum;

  int actCluMaxNum;

  double radius;

  double minRho;
  double minDelta;
};

class EDMStream : public Algorithm {

 public:
  EDMParameter EDMParam;
  NodePtr root;
  vector<NodePtr> leafNodes;
  CFTreePtr cfTree;
  EDMStream(param_t &cmd_params);

  ~EDMStream();

  void Initilize() override;

  void runOnlineClustering(PointPtr input) override;

  void runOfflineClustering(DataSinkPtr sinkPtr) override;
 private:
};
}
#endif //SESAME_INCLUDE_ALGORITHM_EDMSTREAM_HPP_
