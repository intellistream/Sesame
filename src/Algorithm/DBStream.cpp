//
// Created by 1124a on 2021/8/30.
//
#include <Algorithm/DBStream.hpp>
#include <Algorithm/WindowModel/WindowFactory.hpp>
#include <Algorithm/DataStructure/DataStructureFactory.hpp>

#pragma clang diagnostic push

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
  this->pointArrivingTime= clock();
  this->weakEntry= pow(dbStreamParams.base,(-1)*dbStreamParams.lambda*dbStreamParams.cleanUpInterval);
  this->aWeakEntry=weakEntry*dbStreamParams.alpha;
  this->microClusterIndex=-1;
 }

 void SESAME::DBStream::runOnlineClustering(PointPtr input) {

  if (!this->isInitial) {
    Initilize();
    this->isInitial = true;
  }
  else
  {
    if(input->getIndex())
       this->pointArrivingTime=clock();
    update(input);
  }
}
void SESAME::DBStream::runOfflineClustering(DataSinkPtr sinkPtr) {

  sinkPtr;
  reCluster(dbStreamParams.alpha);
}



void SESAME::DBStream::update(PointPtr dataPoint){
  double decayFactor=dampedWindow->decayFunction(this->pointArrivingTime, clock());
  this->pointArrivingTime=clock();
  this->microClusterNN=findFixedRadiusNN(dataPoint,decayFactor);
  std::vector<MicroClusterPtr>::size_type sizeNN=microClusterNN.size();
  if (microClusterNN.empty()) {
    microClusterIndex++;
    MicroClusterPtr newMicroCluster=SESAME::DataStructureFactory::createMicroCluster(dbStreamParams.dimension,microClusterIndex,
                                                                                     dataPoint,dbStreamParams.radius);
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
          clock_t startT= weightedAdjacencyList[microClusterPair]->updateTime;
          double decayValue = dampedWindow->decayFunction(startT,this->pointArrivingTime);
          weightedAdjacencyList[microClusterPair]->add(this->pointArrivingTime,decayValue);
        } else {
          AdjustedWeightPtr adjustedWeight = SESAME::DataStructureFactory::createAdjustedWeight(1,this->pointArrivingTime);
          DensityGraph densityGraph(microClusterPair,adjustedWeight);
          weightedAdjacencyList.insert(densityGraph);
        }
      }
    }
    if (checkMove(microClusterNN))
      for (const MicroClusterPtr& microCluster : microClusterNN) microCluster->move();
  }
 if (((pointArrivingTime-this->startTime)/CLOCKS_PER_SEC)% dbStreamParams.cleanUpInterval == 0 && dataPoint->getIndex()!=0)
  cleanUp(this->pointArrivingTime);
}

bool SESAME::DBStream::checkMove( std::vector<MicroClusterPtr> microClustersList) const
{
  bool move=true;
  if(!microClustersList.empty())
  { std::vector<MicroClusterPtr>::size_type size = microClustersList.size();
    for (int i = 0; i < size; i++){
      for (int j = i + 1; j < size; j++){
        double distance=microClustersList.at(i)->getDistance(microClustersList.at(j));
        if (distance < dbStreamParams.radius)
          move= false;
      }
    }
  }
  return move;
}

std::vector<SESAME::MicroClusterPtr> SESAME::DBStream::findFixedRadiusNN(PointPtr dataPoint, double decayFactor)
{
  std::vector<SESAME::MicroClusterPtr> result;
  unordered_set<SESAME::MicroClusterPtr>::iterator iter;
  for (iter= this->microClusters.begin();iter!= this->microClusters.end();iter++) {
    (*iter)->decayWeight(decayFactor);
    double distance =  (*iter)->getDistance(dataPoint);
    if (distance < dbStreamParams.radius) {
      result.push_back(*iter);
    }
  }
  return result;
}
void  SESAME::DBStream::cleanUp(clock_t nowTime){
  unordered_set<MicroClusterPtr> removeMicroCluster;
  unordered_set<MicroClusterPtr>::iterator iter;
  for (iter = microClusters.begin(); iter != microClusters.end(); iter++) {
    if ((*iter)->weight <= this->weakEntry){
      removeMicroCluster.insert((*iter)->copy());
      microClusters.erase(iter);
    }
  }
  WeightedAdjacencyList::iterator interW;
  for (interW = weightedAdjacencyList.begin(); interW != weightedAdjacencyList.end(); interW++){
    if (removeMicroCluster.find(interW->first->microCluster1) !=removeMicroCluster.end()
    || removeMicroCluster.find(interW->first->microCluster2)!=removeMicroCluster.end())
      weightedAdjacencyList.erase(interW);
    else{
      double decayFactor=dampedWindow->decayFunction(interW->second->updateTime,nowTime);
      if (interW->second->getCurrentWeight(decayFactor) < aWeakEntry)
        weightedAdjacencyList.erase(interW);
    }
  }
}

void SESAME::DBStream::reCluster(double threshold){
  unordered_map<MicroClusterPtr ,unordered_set<MicroClusterPtr>> connectivityGraph;//C in DBStream paper
  WeightedAdjacencyList::iterator interW;
  for (interW = weightedAdjacencyList.begin(); interW != weightedAdjacencyList.end(); interW++){
    MicroClusterPtr microCluster1 =interW->first->microCluster1->copy();
    MicroClusterPtr microCluster2 =interW->first->microCluster2->copy();
    if (microCluster1->weight >= dbStreamParams.weightMin &&microCluster2->weight >= dbStreamParams.weightMin){
      double val = 2*interW->second->weight / (microCluster1->weight+microCluster2->weight);
      if (val > threshold) {
        insertIntoGraph(connectivityGraph,microCluster1,microCluster2);
        insertIntoGraph(connectivityGraph,microCluster2,microCluster1);
      }
      else
      {
        insertIntoGraph(connectivityGraph,microCluster1);
        insertIntoGraph(connectivityGraph,microCluster2);
      }
    }
  }
  findConnectedComponents(connectivityGraph);

}
void SESAME::DBStream::insertIntoGraph(unordered_map<MicroClusterPtr ,unordered_set<MicroClusterPtr>> connectivityGraph,
                            MicroClusterPtr microCluster,MicroClusterPtr Other){
  if (connectivityGraph.find(microCluster)!=connectivityGraph.end())
    connectivityGraph.find(microCluster)->second.insert(Other);
  else{
    microCluster->visited = false;
    unordered_set<MicroClusterPtr> newMicroClusterSet;
    newMicroClusterSet.insert(Other);
    connectivityGraph.insert(std::make_pair(microCluster,newMicroClusterSet));
  }
}
void SESAME::DBStream::insertIntoGraph(unordered_map<MicroClusterPtr ,unordered_set<MicroClusterPtr>> connectivityGraph,
                                       MicroClusterPtr microCluster){
  if (connectivityGraph.find(microCluster)==connectivityGraph.end())
  {
    microCluster->visited = false;
    unordered_set<MicroClusterPtr> newMicroClusterSet;
    connectivityGraph.insert(std::make_pair(microCluster,newMicroClusterSet));
  }
}

void SESAME::DBStream::findConnectedComponents(unordered_map<MicroClusterPtr,
                                               unordered_set<MicroClusterPtr>> connectivityGraph){
  unordered_map<MicroClusterPtr,unordered_set<MicroClusterPtr>>::iterator inter;
  for (inter = connectivityGraph.begin(); inter != connectivityGraph.end(); inter++){
    if (!inter->first->visited) {
      std::vector<MicroClusterPtr> newCluster, clusterGroup;
      newCluster.push_back(inter->first);
      while (!newCluster.empty()) {
        //after found the front one, insert it into clusterGroup and delete from the original vector
        MicroClusterPtr microCluster = newCluster.front();
        newCluster.erase(newCluster.begin());
        clusterGroup.push_back(microCluster);
        microCluster->visited = true;

        for(const auto & interS : inter->second)
        {
          if (!interS->visited)
            newCluster.push_back(interS);
        }
      }
      this->finalClusters.push_back(clusterGroup);
    }
  }

}
