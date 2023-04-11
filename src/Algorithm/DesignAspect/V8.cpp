//
// Created by tuidan on 2021/8/24.
//
#include <Algorithm/DataStructure/DataStructureFactory.hpp>
#include <Algorithm/DesignAspect/V8.hpp>
#include <cfloat>

void SESAME::V8::Init() {}

// true means point is not an outlier, false means outlier
bool SESAME::V8::checkoutOutlier(SESAME::PointPtr &point)
{
    auto distance    = 0.0;
    auto minDIstance = DBL_MAX;
    for (auto cluster : this->Clusters)
    {
        pointToClusterDist(point, cluster, distance);
        if (distance < minDIstance)
        {
            minDIstance = distance;
        }
    }
    if (minDIstance > this->V8Param.outlier_distance_threshold)
        return false;
    else
        return true;
}

void SESAME::V8::insertPointIntoOutliers(SESAME::PointPtr &point)
{
    int index     = 0;
    auto distance = DBL_MAX;
    CFPtr insertCluster;
    if (this->outlierNodes.empty())
    {
        insertCluster = make_shared<CF>();
        updateNLS(insertCluster, point);
        insertCluster->setIndex(0);
        this->outlierNodes.push_back(insertCluster);
    }
    else
    {
        int i = 0;
        for (auto outlierCluster : this->outlierNodes)
        {
            double temp = 0.0;
            pointToClusterDist(point, outlierCluster, temp);
            if (temp < distance)
            {
                distance = temp;
                index    = i;
            }
            i++;
        }
        double pointToOutlierDist = 0;
        insertCluster             = this->outlierNodes[index];
        pointToClusterDist(point, insertCluster, pointToOutlierDist);
        if (pointToOutlierDist < this->V8Param.distance_threshold)
        {
            updateNLS(insertCluster, point);
        }
        else
        {
            insertCluster = make_shared<CF>();
            updateNLS(insertCluster, point);
            insertCluster->setIndex(this->outlierNodes.size());
            this->outlierNodes.push_back(insertCluster);
        }
    }
    checkOutlierTransferCluster(insertCluster);
}

void SESAME::V8::checkOutlierTransferCluster(SESAME::CFPtr &outCluster)
{
    if (outCluster->getN() >= this->V8Param.outlier_cap)
    {
        // need to transfer outlier cluster into real cluster
        this->outlierNodes.erase(this->outlierNodes.begin() + outCluster->getIndex());
        this->Clusters.push_back(outCluster);
    }
}

void SESAME::V8::RunOnline(const SESAME::PointPtr input)
{
    // insert the root
    if (input->getIndex() >= this->V8Param.landmark)
    {
        forwardInsert(input->copy());
    }
}

void SESAME::V8::RunOffline(DataSinkPtr sinkPtr)
{
    for (int i = 0; i < this->Clusters.size(); i++)
    {
        PointPtr centroid = DataStructureFactory::createPoint(i, 1, V8Param.dim, 0);
        for (int j = 0; j < V8Param.dim; j++)
        {
            centroid->setFeatureItem(this->Clusters[i]->getLS().at(j) / this->Clusters[i]->getN(),
                                     j);
        }
        sinkPtr->put(centroid->copy());
    }
    timerMeter.printTime(false, false, false, false);
}

SESAME::V8::V8(param_t &cmd_params)
{
    this->V8Param.num_points                 = cmd_params.num_points;
    this->V8Param.dim                        = cmd_params.dim;
    this->V8Param.distance_threshold         = cmd_params.distance_threshold;  // b
    this->V8Param.landmark                   = cmd_params.landmark;
    this->V8Param.outlier_distance_threshold = cmd_params.outlier_distance_threshold;  // a
    this->V8Param.outlier_cap                = cmd_params.outlier_cap;
}
SESAME::V8::~V8() {}

// when a new point insert into the CF, update the CF N, LS and SS
void SESAME::V8::updateNLS(SESAME::CFPtr &currentCF, SESAME::PointPtr &point)
{
    vector<double> tmpLS = currentCF->getLS();
    vector<double> tmpSS = currentCF->getSS();
    if (tmpLS.empty())
    {
        for (int i = 0; i < point->getDimension(); i++)
        {
            tmpLS.push_back(0);
            tmpSS.push_back(0);
        }
    }
    currentCF->setN(currentCF->getN() + 1);
    for (int i = 0; i < point->getDimension(); i++)
    {
        tmpLS[i] += point->getFeatureItem(i);
        tmpSS[i] += pow(point->getFeatureItem(i), 2);
    }
    currentCF->setLS(tmpLS);
    currentCF->setSS(tmpSS);
}

// centroid index: -1(virtual)
// centroid feature: mean of the feature of cluster points
// centroid cluster: -1
void SESAME::V8::calculateCentroid(SESAME::CFPtr &cf, SESAME::PointPtr &centroid)
{
    centroid->setIndex(-1);
    centroid->setClusteringCenter(-1);
    vector<double> ls = cf->getLS();
    for (int i = 0; i < ls.size(); i++) centroid->setFeatureItem(ls.at(i) / (double)cf->getN(), i);
}

// use Euclidean Distance
void SESAME::V8::pointToClusterDist(SESAME::PointPtr &insertPoint, SESAME::CFPtr &currentCF,
                                    double &dist)
{
    dist                      = 0;
    SESAME::PointPtr centroid = make_shared<SESAME::Point>(V8Param.dim);
    calculateCentroid(currentCF, centroid);
    for (int i = 0; i < insertPoint->getDimension(); i++)
    {
        dist += pow(centroid->getFeatureItem(i) - insertPoint->getFeatureItem(i), 2);
    }
    dist = sqrt(dist);
}

// select the closest child cluster according to Manhattan Distance
void SESAME::V8::selectCluster(SESAME::PointPtr &insertPoint, SESAME::CFPtr &currentCF)
{
    double dist = 0;
    double temp = 0;
    pointToClusterDist(insertPoint, this->Clusters.at(0), dist);
    currentCF = this->Clusters.at(0);
    for (int i = 1; i < this->Clusters.size(); i++)
    {
        pointToClusterDist(insertPoint, Clusters[i], temp);
        if (temp < dist)
        {
            dist      = temp;
            currentCF = this->Clusters.at(i);
        }
    }
    if (dist >= this->V8Param.distance_threshold)
    {
        currentCF = make_shared<CF>();
        initializeCF(currentCF, this->V8Param.dim);
        this->Clusters.push_back(currentCF);
    }
}

// calculate the radius of a cluster
double SESAME::V8::calculateRadius(SESAME::PointPtr &point, SESAME::PointPtr &centroid)
{
    timerMeter.dataInsertAccMeasure();
    double denominator = 0;
    double radius      = 0;
    for (int i = 0; i < point->getDimension(); i++)
    {
        denominator += pow(centroid->getFeatureItem(i) - point->getFeatureItem(i), 2);
    }
    radius = sqrt(denominator);
    timerMeter.dataInsertEndMeasure();
    return radius;
}

void SESAME::V8::initializeCF(SESAME::CFPtr &cf, int dim)
{
    vector<double> ls, ss;
    for (int i = 0; i < dim; i++)
    {
        ls.push_back(0);
        ss.push_back(0);
    }
    cf->setN(0);
    cf->setLS(ls);
    cf->setSS(ss);
}

void SESAME::V8::forwardInsert(SESAME::PointPtr point)
{
    if (this->Clusters.size() == 0)
    {
        auto blankCF = make_shared<CF>();
        initializeCF(blankCF, this->V8Param.dim);
        this->Clusters.push_back(blankCF);
        updateNLS(blankCF, point);
    }
    else
    {
        if (checkoutOutlier(point))
        {
            CFPtr nearestCluster = make_shared<CF>();
            selectCluster(point, nearestCluster);
            updateNLS(nearestCluster, point);
        }
        else
        {
            insertPointIntoOutliers(point);
        }
    }
}
