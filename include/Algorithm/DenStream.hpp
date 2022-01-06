//
// Created by 1124a on 2021/8/23.
//
#ifndef SESAME_INCLUDE_ALGORITHM_DENSTREAM_HPP_
#define SESAME_INCLUDE_ALGORITHM_DENSTREAM_HPP_
#include <cmath>
#include <cassert>
#include <limits>
#include <ctime>
#include <cstdio>
#include <Algorithm/Algorithm.hpp>
#include<Algorithm/DataStructure/MicroCluster.hpp>
#include <Algorithm/OfflineClustering/DBSCAN.hpp>
#include <Algorithm/WindowModel/DampedWindow.hpp>
#include <Utils/BenchmarkUtils.hpp>

namespace SESAME {
#define noVisited (-1)
class DenStreamParams : public AlgorithmParameters {
 public:

  int initBufferSize;//number of data point for Initialization
  unsigned int minPoints;//minimum point of core point in DBSCAN
  double epsilon;//maximum distance if point belongs to the density area of core point
  double base;//base of decay function
  double lambda;
  double mu;//used to calculate minimum weight minWeight=mu*beta;
  double beta;//used to calculate minimum weight

};

  class DenStream : public Algorithm
      {
       public:
        DenStreamParams denStreamParams;
        std::shared_ptr<DBSCAN> dbscan; //used for initialization and offline re-clustering
        DampedWindowPtr dampedWindow;
        std::vector <MicroClusterPtr> pMicroClusters;
        std::vector <MicroClusterPtr> oMicroClusters;
        clock_t startTime;
        clock_t pointArrivingTime;
        clock_t lastUpdateTime;//for calculating time interval
        double Tp;
        TimeMeter timerMeter;
        int pMicroClusterIndex;
        int oMicroClusterIndex;

        DenStream(param_t &cmd_params);

        ~DenStream();
        void Initilize() override;
        void runOnlineClustering(PointPtr input) override;
        void runOfflineClustering(DataSinkPtr sinkPtr) override;
        double getMinWeight(){return minWeight;};
       private:
        bool isInitial  = false;
        vector <PointPtr> initialBuffer;
        double minWeight;
        void init(vector <PointPtr> &initData);
        void merge(PointPtr dataPoint);
        void pointsNearCorePoint(vector <PointPtr> &initData,std::vector<int> pointIndex,MicroClusterPtr microCluster);
        int nearestNeighbor(PointPtr dataPoint,std::vector <MicroClusterPtr> microClusters );
        int mergeToMicroCluster(PointPtr dataPoint,std::vector <MicroClusterPtr> microClusters );
        static void  microClusterToPoint(std::vector<MicroClusterPtr> &microClusters,
                                         vector <PointPtr> &points);
        //TODO overlap functions with Clustream, may need to remove to utils folder
      };
}
#endif //SESAME_INCLUDE_ALGORITHM_DENSTREAM_HPP_
