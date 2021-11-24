//
// Created by 1124a on 2021/8/30.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DBSTREAM_HPP_
#define SESAME_INCLUDE_ALGORITHM_DBSTREAM_HPP_
#include <Algorithm/Algorithm.hpp>
#include <Algorithm/DataStructure/WeightedAdjacencyList.hpp>
#include <Utils/BenchmarkUtils.hpp>
#include <Algorithm/OfflineClustering/ConnectedRegions.hpp>

namespace SESAME {
typedef std::vector<std::vector<MicroClusterPtr>> Clusters;
class DBStreamParams : public AlgorithmParameters {
 public:
  double radius;
  double lambda;
  int cleanUpInterval;//Time gap
  double weightMin;//minimum weight
  double alpha;//α, intersection factor
  double base;//base of decay function
};


class DBStream : public Algorithm
    {
     public:
      DBStreamParams dbStreamParams;
      DampedWindowPtr dampedWindow;
      std::vector<MicroClusterPtr> microClusters;
      SESAME::WeightedAdjacencyList weightedAdjacencyList;
      std::vector<MicroClusterPtr> microClusterNN;//micro clusters found in function findFixedRadiusNN
      int weakEntry;//W_weak, weak entries
      double aWeakEntry;
      clock_t startTime;
      clock_t pointArrivingTime;
      clock_t lastCleanTime;
      int microClusterIndex;
      //Final output of clusters
      Clusters finalClusters;
      ConnectedRegions connectedRegions;
     //Connectivity graph

    // unordered_map<int,std::vector<int>> connecvtivityGraphId;

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
      std::vector<MicroClusterPtr> findFixedRadiusNN(PointPtr dataPoint);
      void cleanUp(clock_t nowTime);
     //The underline has moved to offline refinement
      /*  void reCluster(double threshold);
      void insertIntoGraph(int microClusterId,int OtherId);
      void insertIntoGraph(int microClusterId);
      void findConnectedComponents();*/
    };

}
#endif //SESAME_INCLUDE_ALGORITHM_DBSTREAM_HPP_
