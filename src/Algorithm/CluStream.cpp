// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by 1124a on 2021/8/16.
//
#include<Algorithm/CluStream.hpp>
#include <Algorithm/WindowModel/WindowFactory.hpp>
#include <Algorithm/DataStructure/DataStructureFactory.hpp>
#include <iterator>
/**
 * @Description: "offline" init micro clusters using KMeans
 * @param size: The size of initial data objects,
 *  initialData:input initial data
 *@Return: void
 */
SESAME::CluStream::CluStream(param_t &cmd_params) {
  this->CluStreamParam.pointNumber = cmd_params.pointNumber;
  this->CluStreamParam.dimension = cmd_params.dimension;
  this->CluStreamParam.clusterNumber = cmd_params.onlineClusterNumber;
  this->CluStreamParam.lastArrivingNum = cmd_params.lastArrivingNum;
  this->CluStreamParam.timeWindow = cmd_params.timeWindow;
  this->CluStreamParam.timeInterval = cmd_params.timeInterval;
  this->CluStreamParam.offlineClusterNumber = cmd_params.clusterNumber;
  this->CluStreamParam.radiusFactor = cmd_params.radiusFactor;
  this->CluStreamParam.initBuffer = cmd_params.initBuffer;
  this->CluStreamParam.offlineTimeWindow = cmd_params.offlineTimeWindow;
}
SESAME::CluStream::~CluStream() {

}

void SESAME::CluStream::initOffline(vector <PointPtr> &initData, vector <PointPtr> &initialData) {
  pointsFitted = 0;
  pointsForgot = 0;
  pointsMerged = 0;
  for (int i = 0; i < CluStreamParam.clusterNumber; i++) {
    microClusters.push_back(DataStructureFactory::createMicroCluster(CluStreamParam.dimension, i));
  }
  std::vector<std::vector<PointPtr>> oldGroups, newGroups;
  this->kmeans->runKMeans(CluStreamParam.clusterNumber,
                          CluStreamParam.initBuffer,
                          initData,
                          oldGroups,
                          newGroups,
                          true);
  // store the result input output
  this->kmeans->storeResult(oldGroups, initialData);
  for (int i = 0; i < CluStreamParam.initBuffer; i++) {
    int clusterId = initialData[i]->getClusteringCenter();
    // SESAME_INFO("the belonging micro cluster id is !"<<clusterId);
    int timestamp = (int) ((clock() - startTime) / CLOCKS_PER_SEC);
    if (microClusters[clusterId]->weight == 0)
      microClusters[clusterId]->init(initialData[i], timestamp);
    else
      microClusters[clusterId]->insert(initialData[i], timestamp);
  }
}

/**
 * @Description: incrementally insert data into micro clusters,
 * first determine closest clusters to the data,
 * then judge whether it locates into the maximum boundary of this micro cluster
 * 1. data object fits into closest micro cluster;
 * 2. data does not fit in any cluster,  delete oldest one & create a new cluster;
 * 3.  merge two closest clusters & create a new cluster.
 * @Param: data: input data object
 * @Return: store the output result(with computed clustering center) into ???
 */
void SESAME::CluStream::incrementalCluster(PointPtr data) { // 1. Determine closest clusters
  MicroClusterPtr closestCluster;
  double minDistance = doubleMax;
  timerMeter.dataInsertAccMeasure();
  for (int i = 0; i < this->CluStreamParam.clusterNumber; i++) {
    double dist = microClusters[i]->calCentroidDistance(data);
    if (dist < minDistance) {
      closestCluster = microClusters[i]->copy();
      minDistance = dist;
    }
  }
  double radius = calRadius(closestCluster);
  if (minDistance < radius) {
    insertIntoCluster(data, closestCluster);
    timerMeter.dataInsertEndMeasure();

  }
/** 3. Date does not fit  -- free
 * some space to insert a new micro cluster
 * */
// 3.1 delete oldest one & create a new cluster
  timerMeter.outlierDetectionAccMeasure();
  deleteCreateCluster(data);
  timerMeter.outlierDetectionEndMeasure();

  timerMeter.conceptDriftAccMeasure();
// 3.2 merge two closest clusters & create a new cluster
  MergeCreateCluster(data);
  timerMeter.conceptDriftEndMeasure();
}

//Calculate and return the value of radius
double SESAME::CluStream::calRadius(MicroClusterPtr closestCluster) {
  double radius;
  if (closestCluster->weight == 1) {
    // Special case: estimate radius by determining the distance to the
    // next closest cluster
    radius = doubleMax;
    dataPoint centroid = closestCluster->getCentroid();
    for (int i = 0; i < this->CluStreamParam.clusterNumber; i++) {

      if (microClusters[i]->id == closestCluster->id) {
        continue;
      }
      double dist = distance(microClusters[i]->getCentroid(), centroid, this->CluStreamParam.dimension);
      radius = std::min(dist, radius);
    }
  } else
    radius = closestCluster->getRadius(this->CluStreamParam.radiusFactor);
  return radius;
}

//   Date fits case
void SESAME::CluStream::insertIntoCluster(PointPtr data, MicroClusterPtr operateCluster) {
  // SESAME_INFO("This data fits");
  pointsFitted++;
  int timestamp = (int) ((clock() - startTime) / CLOCKS_PER_SEC);
  operateCluster->insert(data, timestamp);
}

//Delete the oldest cluster and create new one case
void SESAME::CluStream::deleteCreateCluster(PointPtr data) {
  // 3.1 Try to forget old micro clusters

  int elapsedTime = (int) ((clock() - startTime) / CLOCKS_PER_SEC);
  int threshold = 0; // Kernels before this can be forgotten
  if (elapsedTime - this->CluStreamParam.timeWindow >= 0)
    threshold = elapsedTime - this->CluStreamParam.timeWindow;
  for (int i = 0; i < this->CluStreamParam.clusterNumber; i++) {
    if (microClusters[i]->getRelevanceStamp(this->CluStreamParam.lastArrivingNum) < threshold) {
      //SESAME_INFO("Need to delete");
      int newId = this->CluStreamParam.clusterNumber + pointsForgot + pointsMerged;
      DataStructureFactory::clearMicroCluster(microClusters[i]);
      microClusters[i] = DataStructureFactory::createMicroCluster(CluStreamParam.dimension, newId);
      microClusters[i]->init(std::move(data), elapsedTime);
      pointsForgot++;
      return;
    }
  }
}

// Merge two closest clusters & create a new cluster

void SESAME::CluStream::MergeCreateCluster(PointPtr data) {
  // SESAME_INFO("Micro cluster needs to merge");
  unsigned int closestA = 0;
  unsigned int closestB = 0;
  double minDistance = doubleMax;
  for (int i = 0; i < this->CluStreamParam.clusterNumber; i++) { //O(n(n+1)/2)
    dataPoint centroidA = microClusters[i]->getCentroid();
    for (int j = i + 1; j < this->CluStreamParam.clusterNumber; j++) {
      double dist = distance(centroidA, microClusters[j]->getCentroid(), this->CluStreamParam.dimension);
      if (dist < minDistance) {
        minDistance = dist;
        closestA = i;
        closestB = j;
      }
    }
  }
  int newId = this->CluStreamParam.clusterNumber + pointsForgot + pointsMerged;
  microClusters[closestA]->merge(microClusters[closestB]);
  int elapsedTime = (int) ((clock() - startTime) / CLOCKS_PER_SEC);
  DataStructureFactory::clearMicroCluster(microClusters[closestB]);
  microClusters[closestB] = DataStructureFactory::createMicroCluster(CluStreamParam.dimension, newId);
  microClusters[closestB]->init(std::move(data), elapsedTime);
  pointsMerged++;

}

void SESAME::CluStream::microClusterToPoint(std::vector<MicroClusterPtr> &microClusters,
                                            vector <PointPtr> &points) const {
  for (int i = 0; i < this->CluStreamParam.clusterNumber; i++) {
    PointPtr
        point = DataStructureFactory::createPoint(i, microClusters[i]->weight, microClusters[i]->centroid.size(), 0);
    for (SESAME::dataPoint::size_type j = 0; j < microClusters[i]->centroid.size(); j++)
      point->setFeatureItem(microClusters[i]->centroid[j], j);
    //points;
    points.push_back(point);
  }
}
double SESAME::CluStream::distance(dataPoint a, dataPoint b, int dim) {
  double temp = 0;
  for (int i = 0; i < dim; i++) {
    double diff = b[i] - a[i];
    temp += diff * diff;
  }
  return sqrt(temp);
}

void SESAME::CluStream::Initilize() {
  /*SESAME_INFO("configure cmd_params.pointNumber: " << CluStreamParam.pointNumber);
  SESAME_INFO("configure cmd_params.initBuffer: " << CluStreamParam.initBuffer);
  SESAME_INFO("configure cmd_params.lastArrivingNum: " << CluStreamParam.lastArrivingNum);
  SESAME_INFO("configure cmd_params.timeWindow: " << CluStreamParam.timeWindow);
  SESAME_INFO("configure cmd_params.timeInterval: " << CluStreamParam.timeInterval);
  SESAME_INFO("configure cmd_params.clusterNumber: " << CluStreamParam.clusterNumber);
  SESAME_INFO("configure cmd_params.offlineClusterNumber: " << CluStreamParam.offlineClusterNumber);
  SESAME_INFO("configure cmd_params.radiusFactor: " << CluStreamParam.radiusFactor);
  SESAME_INFO("configure cmd_params.offlineTimeWindow: " << CluStreamParam.offlineTimeWindow);*/
  this->window = WindowFactory::createLandmarkWindow();
  this->window->pyramidalWindow.timeInterval = this->CluStreamParam.timeInterval;
  this->startTime = clock();
  this->lastUpdateTime=this->startTime;
  window->initPyramidalWindow(this->window->pyramidalWindow.timeInterval);

}

/**
 * @Description: online clustering of Clustream,
 * insert every data object and cluster them incrementally,it has three conditions:
 * 1. data object fits into closest micro cluster;
 * 2. data does not fit in any cluster,  delete oldest one & create a new cluster;
 * 3.  merge two closest clusters & create a new cluster.
 * @Param:dimension: dimension of data object
 * pointNumber: total number of data objects collected in workload
 * timeInterval: time interval of pyramidal window
 * lastArrivingNum: the last arriving  m points in each micro clusters
 * radiusFactor: radius factor of micro cluster
 * size: size of initial data objects
 * input: vector of data streams
 * @Return: store the output result(with computed clustering center) into ???//
 */
void SESAME::CluStream::runOnlineClustering(SESAME::PointPtr input) {
  timerMeter.setInterval(1);
  timerMeter.onlineAccMeasure();
  if (!this->initilized) {
    //Start to count time
    timerMeter.overallStartMeasure();
    timerMeter.initialMeasure();
    // overallAccMeasure
    timerMeter.OverallPreUpdate();

    Initilize();

    this->initialInputs.push_back(input);
    if (this->initialInputs.size() == this->CluStreamParam.initBuffer) {//TODO: fix this hard code @Zhenyu
      vector <PointPtr> initData;//initialData
      initOffline(this->initialInputs,initData);
      timerMeter.initialEndMeasure();
      timerMeter.MeterInitialUSEC();
      window->pyramidalWindowProcess(startTime, microClusters);
      this->initilized = true;
      timerMeter.OverallPreUpdate();
    }
  } else{
   // timerMeter.MeterOverallAccUSEC();
    timerMeter.OverallPreUpdate();

    int interval;
    clock_t now = clock();
    interval = (int) ((now - lastUpdateTime) / CLOCKS_PER_SEC);

    timerMeter.snapshotAccMeasure();
    if (interval >= 1)
    {
      window->pyramidalWindowProcess(startTime, microClusters);
      lastUpdateTime = now;
    }
    timerMeter.snapshotEndMeasure();

   // timerMeter.MeterSnapshotAccUSEC();
    incrementalCluster(input);

  }
  timerMeter.onlineEndMeasure();
}

void SESAME::CluStream::runOfflineClustering(SESAME::DataSinkPtr sinkPtr) {

  timerMeter.refinementAccMeasure();

  clock_t now = clock();
  vector <vector<PointPtr>> groups;
  int elapsedTime = (int) ((now - startTime) / CLOCKS_PER_SEC);
  int landmarkTime = elapsedTime - this->CluStreamParam.offlineTimeWindow;
  if (this->CluStreamParam.offlineTimeWindow > elapsedTime)
    landmarkTime = 0;
  SESAME_INFO("Start offline...");
  SESAME::SnapshotPtr landmarkSnapshot;
  SESAME::SnapshotPtr subtractMiroCluster;
  //If offlineTimeWindow ==0, Only Observe the end results of micro clusters
  subtractMiroCluster =
      DataStructureFactory::createSnapshot(microClusters, (int) ((now - startTime) / CLOCKS_PER_SEC));
  //SESAME_INFO("Now Micro Cluster is...");
  for (int i = 0; i < CluStreamParam.clusterNumber; i++) {
    std::stringstream result, re2;
    std::copy(subtractMiroCluster->microClusters[i]->id.begin(),
              subtractMiroCluster->microClusters[i]->id.end(),
              std::ostream_iterator<int>(re2, " "));
//    SESAME_INFO("The ID is " << re2.str() << "weight is " << subtractMiroCluster->microClusters[i]->weight);
  }

  //The offline is to observe a process of data stream clustering
  if (CluStreamParam.offlineTimeWindow != 0) {//
    landmarkSnapshot = SESAME::Snapshot::findSnapshot(window->orderSnapShots,
                                                      landmarkTime, elapsedTime, window->pyramidalWindow.currentOrder);

   // SESAME_INFO("Landmark Miro Cluster is...");
    for (int i = 0; i < CluStreamParam.clusterNumber; i++) {
      std::stringstream re2;
      std::copy(landmarkSnapshot->microClusters[i]->id.begin(),
                landmarkSnapshot->microClusters[i]->id.end(),
                std::ostream_iterator<int>(re2, " "));
      //SESAME_INFO("The ID is " << re2.str() << "weight is " << landmarkSnapshot->microClusters[i]->weight);
    }
    if (landmarkSnapshot->elapsedTime == -1)
      landmarkSnapshot = subtractMiroCluster;

    subtractMiroCluster = SESAME::Snapshot::substractSnapshot(subtractMiroCluster, landmarkSnapshot,
                                                               this->CluStreamParam.clusterNumber);
  }
 // SESAME_INFO("subtract Miro Cluster is...");
  for (int i = 0; i < CluStreamParam.clusterNumber; i++) {
    std::stringstream  re2;
    std::copy(subtractMiroCluster->microClusters[i]->id.begin(),
              subtractMiroCluster->microClusters[i]->id.end(),
              std::ostream_iterator<int>(re2, " "));
   // SESAME_INFO("The ID is " << re2.str() << "weight is " << subtractMiroCluster->microClusters[i]->weight);
  }
  vector <PointPtr> TransformedSnapshot;
  microClusterToPoint(subtractMiroCluster->microClusters, TransformedSnapshot);

 // SESAME_INFO("offline Cluster Number " << this->CluStreamParam.offlineClusterNumber << "Total number of p: " << TransformedSnapshot.size());

  std::vector<std::vector<PointPtr>> oldGroups, newGroups;

  this->kmeans->runKMeans(this->CluStreamParam.offlineClusterNumber, this->CluStreamParam.clusterNumber,
                          TransformedSnapshot, oldGroups, newGroups, true);

  timerMeter.refinementEndMeasure();
  timerMeter.MeterRefinementUSEC();


  //Count overall time
  timerMeter.OverallPreUpdate();
  timerMeter.overallEndMeasure();
  timerMeter.MeterOverallUSEC();
  timerMeter.breakdown_global(true, true,false,true);
  // store the result input output
  this->kmeans->produceResult(oldGroups, sinkPtr);

}


