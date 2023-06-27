//
// Created by tuidan on 2021/8/24.
//

#ifndef SESAME_INCLUDE_ALGORITHM_BENNE_HPP_
#define SESAME_INCLUDE_ALGORITHM_BENNE_HPP_

#include "Algorithm/Algorithm.hpp"
#include "Algorithm/DesignAspect/Generic.hpp"
#include "Algorithm/OfflineRefinement/KMeans.hpp"
#include "Algorithm/WindowModel/LandmarkWindow.hpp"
#include "Sinks/DataSink.hpp"
#include "Utils/BenchmarkUtils.hpp"

namespace SESAME
{
struct characteristics
{
    bool frequentDrift = false;
    bool manyOutliers  = false;
    bool highDimension = false;
};

enum windowSelection
{
    landmark = 0,
    sliding  = 1,
    damped   = 2
};
enum dataSelection
{
    MCs   = 0,
    CFT   = 1,
    CoreT = 2,
    DPT   = 3,
    Grids = 4,
    AMS   = 5
};
enum outlierSelection
{
    OD   = 0,
    NoOD = 1,
    ODB  = 2,
    ODT  = 3,
    ODBT = 4
};
enum refineSelection
{
    Incre    = 0,
    OneShot  = 1,
    NoRefine = 2
};
class Benne : public Algorithm
{
public:
    std::vector<PointPtr> queue_;
    std::vector<PointPtr> centers;
    BenneThreshold T;
    bool ds_changed = false;
    bool eff_obj = false;
    AlgorithmPtr algo;
    ObjType obj;
    characteristics chara;
    windowSelection windowSel;
    dataSelection dataSel;
    outlierSelection outlierSel;
    refineSelection refineSel;
    KMeans kmeans;

    using MicroClusters                = ClusteringFeaturesList;
    static constexpr int INCRE_REF_CNT = 50000;

    Benne(param_t &cmd_params);

    ~Benne();

    void Init() override;

    void RunOnline(PointPtr input) override;

    void RunOffline(DataSinkPtr sinkPtr) override;

private:
    void Train(PointPtr point);
    int Infer(const SESAME::PointPtr input);
    void UpdateAlgo(int, int);
};

}  // namespace SESAME
#endif  // SESAME_INCLUDE_ALGORITHM_BENNE_HPP_
