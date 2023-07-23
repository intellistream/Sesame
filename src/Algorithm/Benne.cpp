#include <cfloat>
#include <cmath>

#include "Algorithm/Benne.hpp"
#include "Algorithm/DataStructure/CoresetTree.hpp"
#include "Algorithm/DataStructure/MeyersonSketch.hpp"
#include "Algorithm/DesignAspect/V10.hpp"
#include "Algorithm/DesignAspect/V16.hpp"
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
        dataSel    = CFT;
        outlierSel = ODBT;
        refineSel  = Incre;
        algo       = make_shared<
            StreamClustering<Damped, ClusteringFeaturesTree, OutlierDetection<true, true>, NoRefinement>>(param);
    }
    else if (obj == efficiency)
    {
        windowSel  = sliding;
        dataSel    = Grids;
        outlierSel = NoOD;
        refineSel  = NoRefine;
        eff_obj    = true;
        algo       = make_shared<V9>(param);  // problem
    }
    else
    {
        windowSel  = landmark;
        dataSel    = CoreT;
        outlierSel = ODT;
        refineSel  = OneShot;
        algo       = make_shared<
            StreamClustering<Landmark, CoresetTree, OutlierDetection<false, true>, KMeans>>(param);
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
            change_count++;
            cerr << "benne algo changes from " << hex << old_algo << " to " << new_algo << " when #"
                 << dec << input->index << endl;
        }
        UpdateAlgo(old_algo, new_algo);
        vector<PointPtr> emptyQueue;
        queue_.swap(emptyQueue);
    }
    ds_timer.Tock();
    if (refineSel == Incre && (input->index > 0 && input->index % INCRE_REF_CNT == 0))
    {
        ref_timer.Tick();
        vector<PointPtr> temp_centers, new_centers;
        algo->OutputOnline(temp_centers);
        if (temp_centers.size()) cerr << "temp_centers size: " << temp_centers.size() << endl;
        kmeans.Run(param, temp_centers, new_centers);
        algo->Init();
        for (auto &center : new_centers)
        {
            algo->Insert(center);
        }
        ref_timer.Tock();
    }
    lat_timer.Add(input->toa);
}

void Benne::RunOffline(DataSinkPtr sinkPtr)
{
    cout << "change_count: " << change_count << endl;
    cout << "final_algo: " << hex << (windowSel << 12 | dataSel << 8 | outlierSel << 4 | refineSel) << dec << endl;
    assert(centers.size() <= 40000);
    for (auto &center : centers)
    {
        sinkPtr->put(center);
    }
    algo->RunOffline(sinkPtr);
    sum_timer.Tock();
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
        if (frontElement->dim > 30)
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
    if (outlierNumber > queue_.size() * 0.5)
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
            dataSel != MCs ? ds_changed = true : ds_changed = false;
            dataSel = MCs;
        }
        else
        {
            dataSel != CFT ? ds_changed = true : ds_changed = false;
            dataSel = CFT;
        }
        if (chara.manyOutliers)
        {
            (windowSel != landmark || outlierSel != ODBT) ? ds_changed = true : ds_changed = false;
            windowSel  = landmark;
            outlierSel = ODBT;
        }
        else
        {
            windowSel != damped ? ds_changed = true : ds_changed = false;
            windowSel = damped;
            if (chara.highDimension)
            {
                outlierSel != ODB ? ds_changed = true : ds_changed = false;
                outlierSel = ODB;
            }
            else
            {
                outlierSel != ODBT ? ds_changed = true : ds_changed = false;
                outlierSel = ODBT;
            }
        }
        refineSel != Incre ? ds_changed = true : ds_changed = false;
        refineSel = Incre;
    }
    else if (obj == efficiency)
    {
        if (chara.frequentDrift)
        {
            (dataSel != DPT || windowSel != landmark) ? ds_changed = true : ds_changed = false;
            dataSel   = DPT;
            windowSel = landmark;
        }
        else
        {
            (dataSel != Grids || windowSel != sliding) ? ds_changed = true : ds_changed = false;
            dataSel   = Grids;
            windowSel = sliding;
        }
        (outlierSel != NoOD || refineSel != NoRefine) ? ds_changed = true : ds_changed = false;
        outlierSel = NoOD;
        refineSel  = NoRefine;
    }
    else
    {
        if (chara.frequentDrift)
        {
            dataSel != AMS ? ds_changed = true : ds_changed = false;
            dataSel = AMS;
        }
        else
        {
            dataSel != CoreT ? ds_changed = true : ds_changed = false;
            dataSel = CoreT;
        }
        if (chara.highDimension)
        {
            outlierSel != OD ? ds_changed = true : ds_changed = false;
            outlierSel = OD;
        }
        else
        {
            outlierSel != ODT ? ds_changed = true : ds_changed = false;
            outlierSel = ODT;
        }
        (windowSel != landmark || refineSel != OneShot) ? ds_changed = true : ds_changed = false;
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
    case 0x0531:
        algo = make_shared<
            StreamClustering<Landmark, MeyersonSketch, OutlierDetection<false, true>, KMeans>>(
            param);
        break;
    case 0x0231:
        algo = make_shared<
            StreamClustering<Landmark, CoresetTree, OutlierDetection<false, true>, KMeans>>(param);
        break;
    case 0x1412:
    case 0x1402: algo = std::make_shared<V16>(param); break;
    case 0x0312: algo = std::make_shared<V10>(param); break;
    default: cerr << "Error: no such algorithm: " << hex << new_algo << dec << endl;
    }
    algo->Init();
    if (!eff_obj)
    {
        for (auto &center : temp_centers)
        {
            algo->Insert(center);
        }
    }
    else
    {
        for (auto &center : temp_centers)
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
