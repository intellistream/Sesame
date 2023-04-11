//
// Created by tuidan on 2022/3/14.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V8_HPP_
#define SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V8_HPP_
#include <Algorithm/Algorithm.hpp>
#include <Algorithm/DataStructure/FeatureVector.hpp>
#include <Algorithm/OfflineRefinement/KMeans.hpp>
#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <Sinks/DataSink.hpp>
#include <Utils/BenchmarkUtils.hpp>
namespace SESAME
{

class V8Parameter : public AlgorithmParameters
{
public:
    double distance_threshold;  // T
    int landmark;
    double outlier_distance_threshold;
    double outlier_cap;
};

class V8 : public Algorithm
{
public:
    V8Parameter V8Param;
    int leafMask = 0;
    vector<CFPtr> Clusters;
    vector<CFPtr> outlierNodes;
    TimeMeter timerMeter;
    V8(param_t &cmd_params);

    ~V8();

    void Init() override;

    void RunOnline(PointPtr input) override;

    void RunOffline(DataSinkPtr sinkPtr) override;

private:
    void forwardInsert(PointPtr point);
    double calculateRadius(PointPtr &point, PointPtr &centroid);
    void selectCluster(SESAME::PointPtr &insertPoint, SESAME::CFPtr &currentCF);
    void pointToClusterDist(PointPtr &insertPoint, CFPtr &node, double &dist);
    void calculateCentroid(CFPtr &cf, PointPtr &centroid);
    void updateNLS(CFPtr &currentCF, PointPtr &point);
    void initializeCF(CFPtr &cf, int dim);

    bool checkoutOutlier(SESAME::PointPtr &point);
    void insertPointIntoOutliers(SESAME::PointPtr &point);
    void checkOutlierTransferCluster(SESAME::CFPtr &outCluster);
};
}  // namespace SESAME
#endif  // SESAME_INCLUDE_ALGORITHM_DESIGNASPECT_V8_HPP_
