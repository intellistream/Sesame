//
// Created by tuidan on 2021/8/24.
//
#include <Algorithm/DesignAspect/V8.hpp>
#include <Algorithm/DataStructure/DataStructureFactory.hpp>

void SESAME::V8::Initilize() {}

void SESAME::V8::removeOutliers(){
  for (auto it = this->Outliers.begin(); it != this->Outliers.end();){
    if (!it->get()->getIsOutlier()) {
      it = this->Outliers.erase(it);
    }
    else
      ++it;
  }
}

void SESAME::V8::checkOutliers(SESAME::CFPtr &currentCF) {
  if(currentCF->getN() < this->V8Param.distanceOutliers and !currentCF->getIsOutlier()) {
    currentCF->setIsOutlier(true);
    this->Outliers.push_back(currentCF);
  } else if(currentCF->getN() >= this->V8Param.distanceOutliers) {
    currentCF->setIsOutlier(false);
    removeOutliers();
  }
}


void SESAME::V8::runOnlineClustering(const SESAME::PointPtr input) {
  // insert the root
  if(input->getIndex() >= this->V8Param.landmark){
    forwardInsert(input->copy());
  }
}


void SESAME::V8::runOfflineClustering(DataSinkPtr sinkPtr) {
  for(int i = 0; i < this->Clusters.size(); i++) {
    PointPtr centroid = DataStructureFactory::createPoint(i, 1, V8Param.dimension, 0);
    for(int j = 0; j < V8Param.dimension; j++) {
      centroid->setFeatureItem(this->Clusters[i]->getLS().at(j) / this->Clusters[i]->getN(), j);
    }
    if(this->Clusters[i]->getIsOutlier()){
      centroid->setIsOutlier(true);
    }
    else {
      centroid->setIsOutlier(false);
    }
    sinkPtr->put(centroid->copy());
  }
  timerMeter.printTime(false,false,false,false);
}

SESAME::V8::V8(param_t &cmd_params) {
  this->V8Param.pointNumber = cmd_params.pointNumber;
  this->V8Param.dimension = cmd_params.dimension;
  this->V8Param.thresholdDistance = cmd_params.thresholdDistance;
  this->V8Param.landmark = cmd_params.landmark;
  this->V8Param.distanceOutliers = cmd_params.distanceOutliers;
}
SESAME::V8::~V8() {}

// when a new point insert into the CF, update the CF N, LS and SS
void SESAME::V8::updateNLS(SESAME::CFPtr &currentCF, SESAME::PointPtr &point){
  vector<double> tmpLS = currentCF->getLS();
  vector<double> tmpSS = currentCF->getSS();
  if(tmpLS.empty()) {
    for(int i = 0; i < point->getDimension(); i++){
      tmpLS.push_back(0);
      tmpSS.push_back(0);
    }
  }
  currentCF->setN(currentCF->getN() + 1);
  for(int i = 0; i < point->getDimension(); i++){
    tmpLS[i] += point->getFeatureItem(i);
    tmpSS[i] += pow(point->getFeatureItem(i), 2);
  }
  currentCF->setLS(tmpLS);
  currentCF->setSS(tmpSS);
}

// centroid index: -1(virtual)
// centroid feature: mean of the feature of cluster points
// centroid cluster: -1
void SESAME::V8::calculateCentroid(SESAME::CFPtr &cf, SESAME::PointPtr &centroid) {
  centroid->setIndex(-1);
  centroid->setClusteringCenter(-1);
  vector<double> ls = cf->getLS();
  for(int i = 0; i < ls.size(); i++) centroid->setFeatureItem(ls.at(i) / (double)cf->getN(), i);
}

// use Euclidean Distance
void SESAME::V8::pointToClusterDist(SESAME::PointPtr &insertPoint, SESAME::CFPtr &currentCF, double & dist) {
  dist = 0;
  SESAME::PointPtr centroid = make_shared<SESAME::Point>();
  calculateCentroid(currentCF, centroid);
  for(int i = 0; i < insertPoint->getDimension(); i++) {
    dist += pow(centroid->getFeatureItem(i) - insertPoint->getFeatureItem(i), 2);
  }
  dist = sqrt(dist);
}

// select the closest child cluster according to Manhattan Distance
void SESAME::V8::selectCluster(SESAME::PointPtr &insertPoint, SESAME::CFPtr &currentCF) {
  double dist = 0;
  double temp = 0;
  pointToClusterDist(insertPoint, this->Clusters.at(0), dist);
  currentCF = this->Clusters.at(0);
  for(int i = 1; i < this->Clusters.size(); i++) {
    pointToClusterDist(insertPoint, currentCF, temp);
    if(temp < dist) {
      dist = temp;
      currentCF = this->Clusters.at(i);
    }
  }
  if(dist >= this->V8Param.thresholdDistance) {
    currentCF = make_shared<CF>();
    initializeCF(currentCF, this->V8Param.dimension);
    this->Clusters.push_back(currentCF);
  }
}

// calculate the radius of a cluster
double SESAME::V8::calculateRadius(SESAME::PointPtr &point, SESAME::PointPtr &centroid) {
  timerMeter.dataInsertAccMeasure();
  double denominator = 0;
  double radius = 0;
  for(int i = 0; i < point->getDimension(); i++) {
    denominator += pow(centroid->getFeatureItem(i) - point->getFeatureItem(i), 2);
  }
  radius = sqrt(denominator);
  timerMeter.dataInsertEndMeasure();
  return radius;
}



void SESAME::V8::initializeCF(SESAME::CFPtr &cf, int dimension) {
  vector<double> ls = cf->getLS();
  vector<double> ss = cf->getSS();
  cf->setN(0);
  for(int i = 0; i < dimension; i++) {
    ls.push_back(0);
    ss.push_back(0);
  }
  cf->setLS(ls);
  cf->setSS(ss);
}


void SESAME::V8::forwardInsert(SESAME::PointPtr point){
  if(this->Clusters.size() == 0) {
    timerMeter.dataInsertAccMeasure();
    auto blankCF = make_shared<CF>();
    initializeCF(blankCF, this->V8Param.dimension);
    this->Clusters.push_back(blankCF);
    updateNLS(blankCF, point);
    checkOutliers(blankCF);
    timerMeter.dataInsertEndMeasure();
  } else{
    CFPtr nearestCluster = make_shared<CF>();
    selectCluster(point, nearestCluster);
    updateNLS(nearestCluster, point);
    checkOutliers(nearestCluster);
  }
}
