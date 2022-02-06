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
      double weakEntry;//W_weak, weak entries
      double aWeakEntry;
      timespec startTime;
      timespec lastArrivingTime0;
      timespec pointArrivingTime0;
      timespec lastCleanTime0;

      int lastArrivingTime;
      int  pointArrivingTime;
      int  lastCleanTime;
      int microClusterIndex;
      //Final output of clusters
      Clusters finalClusters;
      ConnectedRegions connectedRegions;
     //Connectivity graph
     TimeMeter timerMeter;
    // unordered_map<int,std::vector<int>> connecvtivityGraphId;


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
      void cleanUp(int nowTime);
     //The underline has moved to offline refinement
      /*  void reCluster(double threshold);
      void insertIntoGraph(int microClusterId,int OtherId);
      void insertIntoGraph(int microClusterId);
      void findConnectedComponents();*/
    };

}
#endif //SESAME_INCLUDE_ALGORITHM_DBSTREAM_HPP_
