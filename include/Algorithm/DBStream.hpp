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
typedef std::vector<std::vector<MicroClusterPtr>> Clusters;
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
      std::vector<MicroClusterPtr> microClusterNN;//micro clusters found in function findFixedRadiusNN
      double weakEntry;//W_weak, weak entries
      double aWeakEntry;
      clock_t startTime;
      clock_t pointArrivingTime;
      int microClusterIndex;
      //Final output of clusters
      Clusters finalClusters;
      //TODO Need to implement weighted a weighted adjacency list S
      DBStream(param_t &cmd_params);
      ~DBStream();
      void Initilize() override;
      void runOnlineClustering(PointPtr input) override;
      void runOfflineClustering(DataSinkPtr sinkPtr) override;
     private:
      bool isInitial  = false;
      void update(PointPtr dataPoint);
      bool checkMove( std::vector<MicroClusterPtr> microClusters) const;
      std::vector<MicroClusterPtr> findFixedRadiusNN(PointPtr dataPoint, double decayFactor);
      void cleanUp(clock_t nowTime);
      void reCluster(double threshold);
      static void insertIntoGraph(unordered_map<MicroClusterPtr ,unordered_set<MicroClusterPtr>> connectivityGraph,
                                  MicroClusterPtr microCluster,MicroClusterPtr Other);
      static void insertIntoGraph(unordered_map<MicroClusterPtr ,unordered_set<MicroClusterPtr>> connectivityGraph,
                           MicroClusterPtr microCluster);
      void findConnectedComponents(unordered_map<MicroClusterPtr,unordered_set<MicroClusterPtr>> connectivityGraph);
    };

}
#endif //SESAME_INCLUDE_ALGORITHM_DBSTREAM_HPP_
