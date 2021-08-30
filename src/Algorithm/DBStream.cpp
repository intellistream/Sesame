//
// Created by 1124a on 2021/8/30.
//
#include <Algorithm/DBStream.hpp>
#include <Algorithm/WindowModel/WindowFactory.hpp>
#include <Algorithm/DataStructure/DataStructureFactory.hpp>

SESAME::DBStream::DBStream(param_t &cmd_params){
  this->dbStreamParams.radius=cmd_params.radius;
  this->dbStreamParams.lambda=cmd_params.lambda;
  this->dbStreamParams.cleanUpInterval=cmd_params.cleanUpInterval;
  this->dbStreamParams.weightMin=cmd_params.weightMin;
  this->dbStreamParams.alpha=cmd_params.alpha;
  this->dbStreamParams.base=cmd_params.base;
}
SESAME::DBStream:: ~DBStream()
= default;
void SESAME::DBStream::Initilize() {
  this->dampedWindow = WindowFactory::createDampedWindow(dbStreamParams.base,  dbStreamParams.lambda);
  this->startTime = clock();
  this->weakEntry= pow(dbStreamParams.base,(-1)*dbStreamParams.lambda*dbStreamParams.cleanUpInterval);
  this->aWeakEntry=weakEntry*dbStreamParams.alpha;
  microClusters=unordered_set<MicroClusterPtr>();
  this->microClusterIndex=-1;
 }

 void SESAME::DBStream::runOnlineClustering(PointPtr input) {
  input;
  if (!this->isInitial) {
    Initilize();
    this->isInitial = true;
  }
  else
  {

  }
}
void SESAME::DBStream::runOfflineClustering(DataSinkPtr sinkPtr) {
  sinkPtr;
}
void SESAME::DBStream::update(PointPtr dataPoint){
  if(dataPoint->getIndex()==0)
  {
   this->pointArrivingTime=clock();
  }
  double decayFactor=dampedWindow->decayFunction(this->pointArrivingTime, clock());
  this->pointArrivingTime=clock();
  std::vector<MicroClusterPtr> microClusterNN=findFixedRadiusNN(dataPoint,decayFactor);
  int sizeNN=microClusterNN.size();
  if (microClusterNN.size() < 1) {
    microClusterIndex++;
    MicroClusterPtr newMicroCluster=SESAME::DataStructureFactory::createMicroCluster(dbStreamParams.dimension,
                                                                                     microClusterIndex,dataPoint,
                                                                                     dbStreamParams.radius);
    microClusters.insert(newMicroCluster);
    microClusterNN.push_back(newMicroCluster);
  }
  else {
    for (int i = 0; i < sizeNN; i++) {
      MicroClusterPtr microCluster = microClusterNN.at(i);
      microCluster->insert(dataPoint); // just update weight
      // update shared density
      for (int j = i + 1; j < sizeNN; j++) {
        MicroClusterPairPtr microClusterPair =SESAME::DataStructureFactory::createMicroClusterPair(microCluster,
                                                                                                   microClusterNN.at(j));
        if (weightedAdjacencyList.find(microClusterPair) != weightedAdjacencyList.end()) {
          clock_t startTime= weightedAdjacencyList[microClusterPair]->updateTime;
          double decayValue = dampedWindow->decayFunction(startTime,this->pointArrivingTime);
          weightedAdjacencyList[microClusterPair]->add(this->pointArrivingTime,decayValue);
        } else {
          AdjustedWeightPtr adjustedWeight = SESAME::DataStructureFactory::createAdjustedWeight(1,this->pointArrivingTime);
          DensityGraph densityGraph(microClusterPair,adjustedWeight);
          weightedAdjacencyList.insert(densityGraph);
        }
      }
  }
    if (checkMove(microClusterNN)) {
      for (MicroClusterPtr microCluster : microClusterNN) {
        microCluster->move();
      }
    }
    if (pointArrivingTime% dbStreamParams.cleanUpInterval == 0 && dataPoint->getIndex()!=0) {
      cleanUp(pointArrivingTime);

    }
  //  return microClusterNN;
  }
}
bool SESAME::DBStream::checkMove( std::vector<MicroClusterPtr> microClusters){
  int size = microClusters.size();
  for (int i = 0; i < size; i++) {
    for (int j = i + 1; j < size; j++) {
      if (microClusters.at(i)->getDistance(microClusters.at(j)) < dbStreamParams.radius) {
        return false;
      }
    }
  }
  return true;
}

//TODO Stop here
std::vector<SESAME::MicroClusterPtr> SESAME::DBStream::findFixedRadiusNN(PointPtr dataPoint, double decayFactor)
{
  std::vector<SESAME::MicroClusterPtr>  microClusterList;
  dataPoint;
  decayFactor;
  return microClusterList;

}
void SESAME::DBStream::cleanUp(clock_t time){
  time;
}

