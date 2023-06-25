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
using namespace std;

double calculateDispersion(vector<SESAME::PointPtr> queue_, SESAME::PointPtr newCenter);

Benne::Benne(param_t &cmd_params) : kmeans(cmd_params)
{
    param = cmd_params, T = param.benne_threshold, obj = param.obj;
}

Benne::~Benne() {}

void Benne::Init()
{
    if (obj == accuracy)
    {
        windowSel  = damped;
        dataSel    = AMS;
        outlierSel = ODBT;
        refineSel  = Incre;
        algo       = make_shared<
            StreamClustering<Damped, MeyersonSketch, OutlierDetection<true, true>, KMeans>>(param);
    }
    else if (obj == efficiency)
    {
        windowSel  = sliding;
        dataSel    = Grids;
        outlierSel = OD;
        refineSel  = NoRefine;
        algo       = make_shared<V9>(param);  // problem
    }
    else
    {
        windowSel  = landmark;
        dataSel    = CoreT;
        outlierSel = ODT;
        refineSel  = OneShot;
        algo       = make_shared<
            StreamClustering<Landmark, ClusteringFeaturesTree, NoDetection, NoRefinement>>(param);
    }
    algo->Init();
    sum_timer.Tick();
}

void Benne::RunOnline(const PointPtr input)
{
    algo->RunOnline(input);
    ds_timer.Tick();
    if (queue_.size() < T.queue_size)
    {
        queue_.push_back(input);
    }
    else
    {
        auto old_algo = windowSel << 12 | dataSel << 8 | outlierSel << 4 | refineSel;
        Train(input);
        auto new_algo = Infer(input);
        if (old_algo != new_algo)
        {
            cerr << "benne algo changes from " << hex << old_algo << " to " << new_algo << " when #"
                 << input->index << oct << endl;
        }
        UpdateAlgo(old_algo, new_algo);
        vector<PointPtr> emptyQueue;
        queue_.swap(emptyQueue);
    }

    if (refineSel == Incre && (input->index > 0 && input->index % INCRE_REF_CNT == 0))
    {
        vector<PointPtr> temp_centers, new_centers;
        algo->OutputOnline(temp_centers);
        cerr << "temp_centers size: " << temp_centers.size() << endl;
        auto newSink = make_shared<DataSink>(param);
        kmeans.Run(param, temp_centers, new_centers);
        algo->Init();
        for (auto &center : new_centers)
        {
            algo->Insert(center);
        }
    }

    ds_timer.Tock();
    lat_timer.Add(input->toa);
}

void Benne::RunOffline(DataSinkPtr sinkPtr)
{
    for (auto &center : centers)
    {
        sinkPtr->put(center);
    }
    algo->RunOffline(sinkPtr);
}

void Benne::Train(const PointPtr input)
{
    int highDimData    = 0;
    int outlierNumber  = 0;
    double var         = 0;
    PointPtr newCenter = make_shared<Point>(input->dim);
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

int Benne::Infer(const PointPtr input)
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
            dataSel != AMS ? dynamicConfigure = true : dynamicConfigure = false;
            dataSel = AMS;
        }
        else
        {
            dataSel != CoreT ? dynamicConfigure = true : dynamicConfigure = false;
            dataSel = CoreT;
        }
        if (chara.highDimension)
        {
            outlierSel != OD ? dynamicConfigure = true : dynamicConfigure = false;
            outlierSel = OD;
        }
        else
        {
            outlierSel != ODB ? dynamicConfigure = true : dynamicConfigure = false;
            outlierSel = ODB;
        }
        (windowSel != landmark || refineSel != OneShot) ? dynamicConfigure = true
                                                        : dynamicConfigure = false;
        windowSel = landmark;
        refineSel = OneShot;
    }

    int combine = windowSel << 12 | dataSel << 8 | outlierSel << 4 | refineSel;
    return combine;
}

void Benne::UpdateAlgo(int old_algo, int new_algo)
{
    if (old_algo == new_algo) return;
    vector<PointPtr> temp_centers;
    algo->OutputOnline(temp_centers);
    switch (new_algo)
    {
    case 0x0040:
        algo = make_shared<
            StreamClustering<Landmark, MicroClusters, OutlierDetection<true, true>, NoRefinement>>(
            param);
        break;
    case 0x2020:
        algo = make_shared<
            StreamClustering<Damped, MicroClusters, OutlierDetection<true, false>, NoRefinement>>(
            param);
        break;
    case 0x2040:
        algo = make_shared<
            StreamClustering<Damped, MicroClusters, OutlierDetection<true, true>, NoRefinement>>(
            param);
        break;
    case 0x0140:
        algo = make_shared<StreamClustering<Landmark, ClusteringFeaturesTree,
                                            OutlierDetection<true, true>, NoRefinement>>(param);
        break;
    case 0x2120:
        algo = make_shared<StreamClustering<Damped, ClusteringFeaturesTree,
                                            OutlierDetection<true, false>, NoRefinement>>(param);
        break;
    case 0x2140:
        algo = make_shared<StreamClustering<Damped, ClusteringFeaturesTree,
                                            OutlierDetection<true, true>, NoRefinement>>(param);
        break;
    case 0x0501:
        algo = make_shared<
            StreamClustering<Landmark, MeyersonSketch, OutlierDetection<false, false>, KMeans>>(
            param);
        break;
    case 0x0201:
        algo = make_shared<
            StreamClustering<Landmark, CoresetTree, OutlierDetection<false, false>, KMeans>>(param);
        break;
    case 0x0521:
        algo = make_shared<
            StreamClustering<Landmark, MeyersonSketch, OutlierDetection<true, false>, KMeans>>(
            param);
        break;
    case 0x0221:
        algo = make_shared<
            StreamClustering<Landmark, CoresetTree, OutlierDetection<true, false>, KMeans>>(param);
        break;
    default: cerr << "Error: no such algorithm: " << hex << new_algo << oct << endl;
    }
    algo->Init();
    if (dynamicConfigure)
    {
        for (auto &center : temp_centers)
        {
            algo->Insert(center);
        }
    }
    else
    {
        for (auto &center : centers)
        {
            centers.push_back(center);
        }
    }
}

double calculateDispersion(vector<PointPtr> queue_, PointPtr newCenter)
{
    // calculate dispersion
    PointPtr variance = make_shared<Point>(newCenter->dim);
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