//
// Created by tuidan on 2021/8/24.
//

#ifndef SESAME_INCLUDE_ALGORITHM_BENNE_HPP_
#define SESAME_INCLUDE_ALGORITHM_BENNE_HPP_
#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <Sinks/DataSink.hpp>
#include <Utils/BenchmarkUtils.hpp>
#include <queue>
#include "Algorithm/DesignAspect/Generic.hpp"
#include "Algorithm/Algorithm.hpp"
namespace SESAME
{
enum objective{accuracy, efficiency, balance};
struct characteristics{
    bool frequentDrift  = false;
    bool manyOutliers   = false;
    bool highDimension  = false;
};
struct threshold{
    int queue_size      = 10000;   // queue size for auto detection
    int dim             = 30;     // above is high dimension
    double variance     = 100;    // above is high concept drift
    int outlierNum      = 200;    // above is many outliers
    double outlierDist  = 50;     // above is outlier   
};
enum windowSelection{landmark, sliding, damped};
enum dataSelection{MCs, CFT, CoreT, DPT, Grids, AMS};
enum outlierSelection{OD, NoOD, ODB, ODT, ODBT};
enum refineSelection{Incre, OneShot, NoRefine};
class Benne : public Algorithm
{
public:
    std::queue<PointPtr> queue_;
    std::vector<PointPtr> centers;
    threshold T;
    bool dynamicConfigure = false;
    AlgorithmPtr algo;
    objective obj;
    characteristics chara;
    windowSelection windowSel;
    dataSelection dataSel;
    outlierSelection outlierSel;
    refineSelection refineSel;
                                
    Benne(param_t &cmd_params);

    ~Benne();

    void Init() override;

    void RunOnline(PointPtr input) override;

    void RunOffline(DataSinkPtr sinkPtr) override;

private:
    void autoDetection(PointPtr point);
    void autoSelection(const SESAME::PointPtr input);
};
}  // namespace SESAME
#endif  // SESAME_INCLUDE_ALGORITHM_BENNE_HPP_
