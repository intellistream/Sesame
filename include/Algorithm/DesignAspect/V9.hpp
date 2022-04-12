//
// Created by 1124a on 2022/4/10.
//

#ifndef COVERTBIRCH_FILE_INCLUDE_ALGORITHM_DESIGNASPECT_V9_HPP_
#define COVERTBIRCH_FILE_INCLUDE_ALGORITHM_DESIGNASPECT_V9_HPP_
#include <Algorithm/Algorithm.hpp>
#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <Sinks/DataSink.hpp>
#include <Algorithm/DataStructure/DensityGrid.hpp>
#include <Algorithm/DataStructure/CharacteristicVector.hpp>
#include <Algorithm/DataStructure/GridCluster.hpp>
#include <Utils/BenchmarkUtils.hpp>
namespace SESAME {

class V9Parameter : public AlgorithmParameters {
 public:
  double beta;
  double cm;
  double cl;
  int landmark;
  double outlierDistanceThreshold;
  double outlierClusterCapacity;
};
typedef std::unordered_map<DensityGrid, CharacteristicVector,GridKeyHash,EqualGrid> HashMap;
class V9 : public Algorithm {

 public:
  V9Parameter V9Param;
  int gap;
  double dm;
  int NGrids;
  int pointArrivingTime;

  TimeMeter timerMeter;
  HashMap gridList;
  std::unordered_map<DensityGrid, int,GridKeyHash,EqualGrid> deletedGrids;
  std::vector<GridCluster> clusterList;
  std::vector<GridCluster> newClusterList;
  std::vector<double> minVals;
  std::vector<double> maxVals;


  V9(param_t &cmd_params);
  ~V9();
  void Initilize() override;
  void runOnlineClustering(PointPtr input) override;
  void runOfflineClustering(DataSinkPtr sinkPtr) override;

 private:
  bool ifReCalculateN(PointPtr point);
  void reCalculateN();
  void GridListUpdate(PointPtr input);
  void initialClustering();
  void adjustClustering();
  bool adjustLabels();
  bool inspectChangedGrids();
  HashMap adjustForSparseGrid(DensityGrid grid, CharacteristicVector characteristicVec, int gridClass);
  HashMap adjustForTransitionalGrid(DensityGrid grid, CharacteristicVector characteristicVec, int gridClass);
  HashMap reCluster (GridCluster gridCluster);
  HashMap adjustNewLabels(HashMap newGridList);
  void mergeClusters(int smallCluster, int bigCluster);
  void cleanClusters();
  HashMap cleanNewClusters(HashMap newGridList);
  HashMap mergeNewClusters(HashMap newGridList, int smallCluster, int bigCluster);
  double densityThresholdFunction(int tg, double cl, int NGrids);
  void updateGridListDensity();
  static void mergeGridList(HashMap gridList, const HashMap &otherList);

  bool checkoutOutlier(CharacteristicVector characteristicVec);//checkIfSporadic
  void insertIntoOutliers();
  HashMap checkOutlierTransferCluster(DensityGrid grid, CharacteristicVector characteristicVec, int gridClass);
};
}
#endif //COVERTBIRCH_FILE_INCLUDE_ALGORITHM_DESIGNASPECT_V9_HPP_
