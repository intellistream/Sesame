//
// Created by 1124a on 2021/8/30.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DBSTREAM_HPP_
#define SESAME_INCLUDE_ALGORITHM_DBSTREAM_HPP_
#include <cmath>
#include <cassert>
#include <limits>
#include <ctime>
#include <cstdio>
#include <unordered_set>
#include <Algorithm/Algorithm.hpp>
#include<Algorithm/DataStructure/MicroCluster.hpp>
#include <Algorithm/WindowModel/DampedWindow.hpp>
#include <Utils/BenchmarkUtils.hpp>
#include <Algorithm/DataStructure/WeightedAdjacencyList.hpp>
namespace SESAME {
class DBStreamParams : public AlgorithmParameters {
 public:
  double radius;
  double lambda;
  int cleanUpInterval;//Tgap
  double weightMin;//minimum weight
  double alpha;//α, intersection factor
  double base;//base of decay function
};
class DBStream : public Algorithm
    {
     public:
      DBStreamParams dbStreamParams;
      DampedWindowPtr dampedWindow;
      unordered_set<MicroClusterPtr> microClusters;
      SESAME::WeightedAdjacencyList weightedAdjacencyList;
      double weakEntry;//W_weak, weak entries
      double aWeakEntry;
      clock_t startTime;
      clock_t pointArrivingTime;
      int microClusterIndex;
      //TODO Need to implement weighted a weighted adjacency list S
      DBStream(param_t &cmd_params);
      ~DBStream();
      void Initilize() override;
      void runOnlineClustering(PointPtr input) override;
      void runOfflineClustering(DataSinkPtr sinkPtr) override;
     private:
      bool isInitial  = false;
     // vector <PointPtr> initialBuffer;
      void update(PointPtr dataPoint);
      void cleanUp(clock_t time);
      bool checkMove( std::vector<MicroClusterPtr> microClusters);
      std::vector<MicroClusterPtr> findFixedRadiusNN(PointPtr dataPoint, double decayFactor);
    };

}
#endif //SESAME_INCLUDE_ALGORITHM_DBSTREAM_HPP_
