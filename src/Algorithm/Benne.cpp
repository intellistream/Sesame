#include <cfloat>
#include <cmath>

#include "Algorithm/Benne.hpp"
#include "Algorithm/DataStructure/CoresetTree.hpp"
#include "Algorithm/DataStructure/MeyersonSketch.hpp"
#include "Algorithm/DesignAspect/V10.hpp"
#include "Algorithm/DesignAspect/V9.hpp"
#include "Algorithm/OfflineRefinement/KMeans.hpp"
#include "Algorithm/OfflineRefinement/OfflineRefinement.hpp"
#include "Sinks/DataSink.hpp"

using namespace SESAME;

double calculateDispersion(std::vector<SESAME::PointPtr> queue_, SESAME::PointPtr newCenter);

Benne::Benne(param_t &cmd_params)
{
    param = cmd_params;
    T     = param.benne_threshold;
}

Benne::~Benne() {}

void Benne::Init()
{
    obj = param.obj;
    if (obj == accuracy)
    {
        windowSel  = damped;
        dataSel    = AMS;
        outlierSel = ODBT;
        refineSel  = Incre;
        algo       = std::make_shared<
            StreamClustering<Damped, MeyersonSketch, DistanceDetection<true, true>, NoRefinement>>(
            param);
    }
    else if (obj == efficiency)
    {
        windowSel  = sliding;
        dataSel    = Grids;
        outlierSel = OD;
        refineSel  = NoRefine;
        algo       = std::make_shared<V9>(param);  // problem
    }
    else
    {
        windowSel  = landmark;
        dataSel    = CoreT;
        outlierSel = ODT;
        refineSel  = OneShot;
        algo       = std::make_shared<
            StreamClustering<Landmark, ClusteringFeaturesTree, NoDetection, NoRefinement>>(param);
    }
    algo->Init();
    sum_timer.Tick();
}

void Benne::RunOnline(const PointPtr input)
{
    ds_timer.Tick();
    if (queue_.size() < T.queue_size)
    {
        queue_.push_back(input);
    }
    else
    {
        autoDetection(input);
        autoSelection(input);
        if (dynamicConfigure)
        {
            algo->OutputOnline();
            // TODO: inherit the clustering centers of previous configuration
        }
        // TODO: reconstruct the algorithm according to the selection
        std::vector<PointPtr> emptyQueue;
        queue_.swap(emptyQueue);
    }
    algo->RunOnline(input);
    ds_timer.Tock();
    lat_timer.Add(input->toa);
}

void Benne::RunOffline(DataSinkPtr sinkPtr) { algo->RunOffline(sinkPtr); }

void Benne::autoDetection(const PointPtr input)
{
    int highDimData    = 0;
    int outlierNumber  = 0;
    double var         = 0;
    PointPtr newCenter = std::make_shared<Point>(input->dim);
    for (auto &frontElement : queue_)
    {
        // Obtain the center of the queue
        for (int i = 0; i < frontElement->dim; i++)
        {
            newCenter->feature[i] += frontElement->feature[i] / queue_.size();
        }
        if (frontElement->dim > T.dim)
        {
            highDimData++;
        }
        double minDist = DBL_MAX;
        for (auto &center : centers)
        {
            double dist = frontElement->L2Dist(center);
            if (dist < minDist)
            {
                minDist = dist;
            }
        }
        if (minDist > T.outliers_dist)
        {
            outlierNumber++;
        }
    }
    var = calculateDispersion(queue_, newCenter);
    for (auto &frontElement : queue_)
    {}
    if (highDimData > queue_.size() * 0.5)
    {
        chara.highDimension = true;
    }
    else
    {
        chara.highDimension = false;
    }
    if (var > T.variance)
    {
        chara.frequentDrift = true;
    }
    else
    {
        chara.frequentDrift = false;
    }
    if (outlierNumber > T.outliers_num)
    {
        chara.manyOutliers = true;
    }
    else
    {
        chara.manyOutliers = false;
    }
}

void Benne::autoSelection(const PointPtr input)
{
    if (obj == accuracy)
    {
        if (chara.frequentDrift)
        {
            dataSel != MCs ? dynamicConfigure = true : dynamicConfigure = false;
            dataSel = MCs;
        }
        else
        {
            dataSel != AMS ? dynamicConfigure = true : dynamicConfigure = false;
            dataSel = AMS;
        }
        if (chara.manyOutliers)
        {
            windowSel != landmark ? dynamicConfigure = true : dynamicConfigure = false;
            windowSel = landmark;
        }
        else
        {
            windowSel != damped ? dynamicConfigure = true : dynamicConfigure = false;
            windowSel = damped;
            if (chara.highDimension)
            {
                outlierSel != ODB ? dynamicConfigure = true : dynamicConfigure = false;
                outlierSel = ODB;
            }
            else
            {
                outlierSel != ODBT ? dynamicConfigure = true : dynamicConfigure = false;
                outlierSel = ODBT;
            }
        }
        refineSel != Incre ? dynamicConfigure = true : dynamicConfigure = false;
        refineSel = Incre;
    }
    else if (obj == efficiency)
    {
        if (chara.frequentDrift)
        {
            (dataSel != AMS || windowSel != landmark) ? dynamicConfigure = true
                                                      : dynamicConfigure = false;
            dataSel   = AMS;
            windowSel = landmark;
        }
        else
        {
            (dataSel != Grids || windowSel != sliding) ? dynamicConfigure = true
                                                       : dynamicConfigure = false;
            dataSel   = Grids;
            windowSel = sliding;
        }
        (outlierSel != OD || refineSel != NoRefine) ? dynamicConfigure = true
                                                    : dynamicConfigure = false;
        outlierSel = OD;
        refineSel  = NoRefine;
    }
    else
    {
        if (chara.frequentDrift)
        {
            dataSel != CFT ? dynamicConfigure = true : dynamicConfigure = false;
            dataSel = CFT;
        }
        else
        {
            dataSel != CoreT ? dynamicConfigure = true : dynamicConfigure = false;
            dataSel = CoreT;
        }
        (windowSel != landmark || outlierSel != ODT || refineSel != OneShot)
            ? dynamicConfigure = true
            : dynamicConfigure = false;
        windowSel  = landmark;
        outlierSel = ODT;
        refineSel  = OneShot;
    }
}

double calculateDispersion(std::vector<PointPtr> queue_, PointPtr newCenter)
{
    // calculate dispersion
    PointPtr variance = std::make_shared<Point>(newCenter->dim);
    for (auto &point : queue_)
    {
        for (int i = 0; i < newCenter->dim; i++)
        {
            variance->feature[i] +=
                pow((point->getFeatureItem(i) - newCenter->getFeatureItem(i)), 2) / queue_.size();
        }
    }
    double dispersion = 0;
    for (int i = 0; i < newCenter->dim; i++)
    {
        dispersion += sqrt(variance->feature[i]);
    }
    return dispersion;
}