//
// Created by 1124a on 2021/10/27.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DSTREAM_HPP_
#define SESAME_INCLUDE_ALGORITHM_DSTREAM_HPP_

#include "Algorithm/Algorithm.hpp"
#include "Algorithm/DataStructure/DensityGrid.hpp"
#include "Algorithm/DataStructure/CharacteristicVector.hpp"
#include "Algorithm/DataStructure/GridCluster.hpp"
#include "Utils/BenchmarkUtils.hpp"

#include <omp.h>

namespace SESAME{
class DStream;
class DStreamParams : public AlgorithmParameters {
 public:
  // User defined parameter lambda in damped window
  double lambda;
  /* User defined parameter: Adjusts the window of protection for
  * renaming previously deleted grids as being sporadic
  * */
  double beta;
  double cm; // User defined parameter: Controls the threshold for dense grids
  double cl; // User defined parameter: Controls the threshold for sparse grids
  double grid_width; //width of grid
};

typedef std::unordered_map<DensityGrid, CharacteristicVector,GridKeyHash,EqualGrid> HashMap;
class DStream : public Algorithm
    {
     public:
      DStreamParams dStreamParams;
      DampedWindowPtr dampedWindow;
      double startTime = 0.0;
      int pointArrivingTime;
      int gap;// Time gap between calls to the offline component
      double dm;// Density threshold for dense grids; controlled by cm
      double dl; //  Density threshold for sparse grids; controlled by cl
      int NGrids;//The number of density grids ,with an initial value 0
      HashMap gridList;
      std::unordered_map<DensityGrid, int,GridKeyHash,EqualGrid> deletedGrids;
      std::vector<GridCluster> clusterList; // A list of all Grid Clusters
      std::vector<GridCluster> newClusterList; //A list of grid clusters used when re-clustering an existing cluster.
      std::vector<double> minVals; //The minimum value seen for a numerical dim; used to calculate N
      std::vector<double> maxVals; //The maximum value seen for a numerical dim; used to calculate N

      DStream(param_t &cmd_params);
      ~DStream();
      void Init() override;
      void RunOnline(PointPtr input) override;
      void RunOffline(DataSinkPtr sinkPtr) override;

     private:
      bool clusterInitial = false;
      bool isInitial  = false;
      bool recalculateN =false; // flag indicating whether N needs to be recalculated after this instance
      std::vector<double> tempCoord;
      std::vector<double> Coord;
      void ifReCalculateN(PointPtr point);
      void reCalculateN();
      void GridListUpdate(std::vector<double> coordinate);
      void initialClustering();
      void adjustClustering();
      bool adjustLabels();
      bool inspectChangedGrids();
      HashMap adjustForSparseGrid(DensityGrid grid, CharacteristicVector characteristicVec, int gridClass);
      HashMap adjustForDenseGrid(DensityGrid grid, CharacteristicVector characteristicVec, int gridClass);
      HashMap adjustForTransitionalGrid(DensityGrid grid, CharacteristicVector characteristicVec, int gridClass);
      void removeSporadic();
      HashMap reCluster (GridCluster gridCluster);
      HashMap adjustNewLabels(HashMap newGridList);
      void mergeClusters(int smallCluster, int bigCluster);
      void cleanClusters();
      HashMap cleanNewClusters(HashMap newGridList);
      HashMap mergeNewClusters(HashMap newGridList, int smallCluster, int bigCluster);
      double outlier_density_thresholdFunction(int tg, double cl, double decayFactor, int NGrids);
      bool checkIfSporadic(CharacteristicVector characteristicVec);
      void updateGridListDensity();
      static void mergeGridList(HashMap gridList, const HashMap &otherList);
      double seconds() { return omp_get_wtime() - startTime; }
    };

}

#endif //SESAME_INCLUDE_ALGORITHM_DSTREAM_HPP_
