//
// Created by tuidan on 2022/3/14.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V8_HPP_
#define SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V8_HPP_
#include <Algorithm/Algorithm.hpp>
#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <Algorithm/OfflineClustering/KMeans.hpp>
#include <Sinks/DataSink.hpp>
#include <Algorithm/DataStructure/FeatureVector.hpp>
#include <Utils/BenchmarkUtils.hpp>
namespace SESAME {

class V8Parameter : public AlgorithmParameters {
 public:
  double thresholdDistance; // T
  int landmark;
  int distanceOutliers;
};

class V8 : public Algorithm {

 public:
  V8Parameter V8Param;
  int leafMask = 0;
  vector<CFPtr> Clusters;
  vector<CFPtr> Outliers;
  TimeMeter timerMeter;
  V8(param_t &cmd_params);

  ~V8();

  void Initilize() override;

  void runOnlineClustering(PointPtr input) override;

  void runOfflineClustering(DataSinkPtr sinkPtr) override;
 private:

  void forwardInsert(PointPtr point);
  double calculateRadius(PointPtr &point, PointPtr &centroid);
  void selectCluster(SESAME::PointPtr &insertPoint, SESAME::CFPtr &currentCF);
  void pointToClusterDist(PointPtr &insertPoint, CFPtr &node, double &dist);
  void calculateCentroid(CFPtr &cf, PointPtr &centroid);
  void updateNLS(CFPtr &currentCF, PointPtr &point);
  void initializeCF(CFPtr &cf, int dimension);

  void removeOutliers();
  void checkOutliers(CFPtr &cf);
};
}
#endif //SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V8_HPP_
