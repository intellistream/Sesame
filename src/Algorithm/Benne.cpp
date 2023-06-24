#include <Algorithm/Benne.hpp>
#include <cmath>
#include <cfloat>
#include "Algorithm/OfflineRefinement/KMeans.hpp"
#include "Algorithm/DataStructure/CoresetTree.hpp"
#include "Algorithm/DataStructure/MeyersonSketch.hpp"
#include "Algorithm/DesignAspect/V10.hpp"
#include "Algorithm/DesignAspect/V9.hpp"
#include "Algorithm/OfflineRefinement/OfflineRefinement.hpp"

SESAME::Benne::Benne(param_t &cmd_params)
{
    param = cmd_params;
}
SESAME::Benne::~Benne(){}
void SESAME::Benne::Init()
{
    obj = param.obj;
    if(obj == accuracy)
    {
        windowSel   = damped;
        dataSel     = AMS;
        outlierSel  = ODBT;
        refineSel   = Incre;
        algo        = std::make_shared<StreamClustering<Damped, MeyersonSketch, 
        DistanceDetection<true, true>, NoRefinement>>(param);
    }
    else if(obj == efficiency)
    {
        windowSel   = sliding;
        dataSel     = Grids;
        outlierSel  = OD;
        refineSel   = NoRefine;
        algo        = std::make_shared<V9>(param);  // problem
    }
    else
    {
        windowSel   = landmark;
        dataSel     = CoreT;
        outlierSel  = ODT;
        refineSel   = OneShot;
        algo        = std::make_shared<StreamClustering<Landmark, ClusteringFeaturesTree, 
        NoDetection, NoRefinement>>(param);
    }
    sum_timer.Tick();
}

void SESAME::Benne::RunOnline(const SESAME::PointPtr input)
{
    ds_timer.Tick();
    if(queue_.size() < T.queue_size)
    {
        queue_.push(input);
    }
    else
    {
        autoDetection(input);
        autoSelection(input);
        if(dynamicConfigure){
            algo->OutputOnline();
            // TODO: inherit the clustering centers of previous configuration
        }
        // TODO: reconstruct the algorithm according to the selection
        std::queue<SESAME::PointPtr> emptyQueue;
        queue_.swap(emptyQueue);
        queue_.empty();
    }
    algo->RunOnline();
    ds_timer.Tock();
    lat_timer.Add(input->toa);
}

void SESAME::Benne::autoDetection(const SESAME::PointPtr input)
{

    int highDimData = 0;
    int outlierNumber= 0;
    double var = 0;
    while (!queue_.empty()) {
        SESAME::PointPtr& frontElement = queue_.front();
        if(frontElement->dim > T.dim)
        {
            highDimData++;
        }
        // update variance
        double minDist = DBL_MAX;
        for(auto &center : centers)
        {
            double dist = getDistance(frontElement, center, frontElement->dim);
            if(dist < minDist)
            {
                minDist = dist;
            }
        }
        if(minDist > T.outlierDist)
        {
            outlierNumber++;
        }
        if(highDimData > queue_.size() * 0.5)
        {
            chara.highDimension = true;
        } 
        else
        {
            chara.highDimension = false;
        }
        if(var > T.variance)
        {
            chara.frequentDrift = true;
        }
        else
        {
            chara.frequentDrift = false;
        }
        if(outlierNumber > T.outlierNum)
        {
            chara.manyOutliers = true;
        }
        else
        {
            chara.manyOutliers = false;
        }
    queue_.pop();
}
}

void SESAME::Benne::autoSelection(const SESAME::PointPtr input)
{
    if(obj == accuracy)
    {
        if(chara.frequentDrift)
        {
            dataSel != MCs? dynamicConfigure = true: dynamicConfigure = false;
            dataSel = MCs;
        }
        else
        {
            dataSel != AMS? dynamicConfigure = true: dynamicConfigure = false;
            dataSel = AMS;
        }
        if(chara.manyOutliers)
        {
            windowSel != landmark? dynamicConfigure = true: dynamicConfigure = false;
            windowSel = landmark;
        }
        else
        {
            windowSel != damped? dynamicConfigure = true: dynamicConfigure = false;
            windowSel = damped;
            if(chara.highDimension)
            {
                outlierSel != ODB? dynamicConfigure = true: dynamicConfigure = false;
                outlierSel = ODB;
            }
            else
            {
                outlierSel != ODBT? dynamicConfigure = true: dynamicConfigure = false;
                outlierSel = ODBT;
            }
        }
        refineSel != Incre? dynamicConfigure = true: dynamicConfigure = false;
        refineSel = Incre;
    }
    else if(obj == efficiency)
    {
        if(chara.frequentDrift)
        {
            (dataSel != AMS || windowSel != landmark) ? dynamicConfigure = true: dynamicConfigure = false;
            dataSel = AMS;
            windowSel = landmark;
        }
        else
        {
            (dataSel != Grids || windowSel != sliding) ? dynamicConfigure = true: dynamicConfigure = false;
            dataSel = Grids;
            windowSel = sliding;
        }
        (outlierSel != OD || refineSel != NoRefine)? dynamicConfigure = true: dynamicConfigure = false;
        outlierSel = OD;
        refineSel = NoRefine;
    }
    else
    {
        if(chara.frequentDrift)
        {
            dataSel != CFT? dynamicConfigure = true: dynamicConfigure = false;
            dataSel = CFT;
        }
        else
        {
            dataSel != CoreT? dynamicConfigure = true: dynamicConfigure = false;
            dataSel = CoreT;
        }
        (windowSel != landmark || outlierSel != ODT || refineSel != OneShot)? dynamicConfigure = true: dynamicConfigure = false;
        windowSel = landmark;
        outlierSel = ODT;
        refineSel = OneShot;
    }
}

double getDistance(SESAME::PointPtr inputs, SESAME::PointPtr centres, int dim)
{
    int i;
    double f = 0.0;
    for (i = 0; i < dim; i++)
    {
        f += pow((inputs->getFeatureItem(i) - centres->getFeatureItem(i)), 2);
    }
    return f;
}



