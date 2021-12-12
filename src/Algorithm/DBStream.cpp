//
// Created by Zhenyu on 2021/8/30.
//
#include <Algorithm/DBStream.hpp>
#include <Algorithm/WindowModel/WindowFactory.hpp>

/**
 * @Description: initialize user defined parameters,
 * @Param:
 * radius: radius of micro clusters
 * lambda: lambda in decay function
 * cleanUpInterval: time gap of clean up
 * weightMin: the minimum weight of micro cluster to identify noise MCs
 * alpha: intersection factor
 * base: decay function base -- Normally 2
 * @Return: void
 */
SESAME::DBStream::DBStream(param_t &cmd_params){
  this->dbStreamParams.pointNumber = cmd_params.pointNumber;
  this->dbStreamParams.dimension = cmd_params.dimension;
  this->dbStreamParams.radius=cmd_params.radius;
  this->dbStreamParams.lambda=cmd_params.lambda;
  this->dbStreamParams.cleanUpInterval=cmd_params.cleanUpInterval;
  this->dbStreamParams.weightMin=cmd_params.weightMin;
  this->dbStreamParams.alpha=cmd_params.alpha;
  this->dbStreamParams.base=cmd_params.base;
}
SESAME::DBStream:: ~DBStream()
= default;

/**
 * @Description: initialization of the algorithm,
 * @Param: void
 * @Return: void
 */
void SESAME::DBStream::Initilize() {
  this->dampedWindow = WindowFactory::createDampedWindow(dbStreamParams.base,  dbStreamParams.lambda);
  this->startTime = clock();
  this->pointArrivingTime= clock();
  this->lastCleanTime=clock();
  this->weakEntry= ceil(pow(dbStreamParams.base,(-1)*dbStreamParams.lambda*dbStreamParams.cleanUpInterval));
  this->aWeakEntry=ceil(weakEntry*dbStreamParams.alpha);
  this->microClusterIndex=-1;
  connectedRegions = ConnectedRegions(dbStreamParams.alpha, dbStreamParams.weightMin);
 }
 /**
  * @Description: online clustering stage, input data point incrementally and update the MC list and weight adjacency lists,
  * @Param: void
  * @Return: void
  */
 void SESAME::DBStream::runOnlineClustering(PointPtr input) {
  if (!this->isInitial) {
    SESAME_INFO("Start initialize...");
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


/**
 * @Description: Insert data point into existing MCs,
 * first find the MCs which data point locates in, if finding no MCs,
 * create new MC with this data point, else if finding MCs can accept this data
 * update these MCs and the corresponding Sij in Weighted adjacency list S;
 * After inserting, we check whether moving center of MCs will collapse,
 * if it will, we roll back moving center actions,
 * finally, we clean up the MCs which is less than Wmin
 * @Param: data point
 * @Return: void
 */

void SESAME::DBStream::update(PointPtr dataPoint){
  double decayFactor=dampedWindow->decayFunction(this->pointArrivingTime, clock());
  this->pointArrivingTime=clock();
  this->microClusterNN=findFixedRadiusNN(dataPoint);
  std::vector<MicroClusterPtr>::size_type sizeNN=microClusterNN.size();
  // SESAME_INFO("find suitable MCs number : "<<sizeNN);
  /**
   * If this point fits in no micro clusters
   * */
  if (microClusterNN.empty()) {
    microClusterIndex++;
    MicroClusterPtr newMicroCluster = SESAME::DataStructureFactory::createMicroCluster(dbStreamParams.dimension,microClusterIndex,
                                                                                     dataPoint,dbStreamParams.radius);
    microClusters.push_back(newMicroCluster);
    microClusterNN.push_back(newMicroCluster);
    //SESAME_INFO("Add new MC!"<<microClusterIndex);
  }
  /**
 * If this point fits in at least one micro cluster
 * */
  else {
    for (int i = 0; i < sizeNN; i++) {
      microClusterNN[i]->insert(dataPoint,decayFactor); // just update weight
      for (int j = i + 1; j < sizeNN; j++) {
        MicroClusterPair microClusterPair(microClusterNN[i], microClusterNN.at(j));
        if (weightedAdjacencyList.find(microClusterPair) != weightedAdjacencyList.end())
        {
          clock_t startT= weightedAdjacencyList[microClusterPair]->updateTime;
          double decayValue = dampedWindow->decayFunction(startT,this->pointArrivingTime);
          weightedAdjacencyList[microClusterPair]->add(this->pointArrivingTime,decayValue);
       //  SESAME_INFO(" weight is "<<weightedAdjacencyList[microClusterPair]->weight );
        } else{
        //  SESAME_INFO("Create microClusterPair!" << microClusterNN.at(i)->id.front()<<", "<<microClusterNN.at(j)->id.front());
          AdjustedWeightPtr adjustedWeight = SESAME::DataStructureFactory::createAdjustedWeight(1,this->pointArrivingTime);
          DensityGraph densityGraph( microClusterPair ,adjustedWeight);
          weightedAdjacencyList.insert(densityGraph);
        }
      }
    }
    if (checkMove(microClusterNN))
      for (const MicroClusterPtr& microCluster : microClusterNN) microCluster->move();
  }
 if (((pointArrivingTime-this->lastCleanTime)/CLOCKS_PER_SEC)>= dbStreamParams.cleanUpInterval && dataPoint->getIndex()!=0)
 {
   cleanUp(this->pointArrivingTime);
   this->lastCleanTime=this->pointArrivingTime;
 }

}


std::vector<SESAME::MicroClusterPtr> SESAME::DBStream::findFixedRadiusNN(PointPtr dataPoint)
{
   std::vector<SESAME::MicroClusterPtr> result;
   std::vector<SESAME::MicroClusterPtr>::size_type iter;
   for (iter= 0;iter< microClusters.size();iter++) {
     //microClusters.at(iter)>decayWeight(decayFactor); //add this line into Micro Cluster insert data functions
     double distance =  microClusters.at(iter)->getDistance(dataPoint);
    // SESAME_INFO("distance is "<<distance);
     if (distance < dbStreamParams.radius)
       result.push_back(microClusters.at(iter));
   }
   return result;
}


bool SESAME::DBStream::checkMove( std::vector<MicroClusterPtr> microClustersList) const
{
  bool move=true;
  if(!microClustersList.empty())
  {
    std::vector<MicroClusterPtr>::size_type i ,j ;
    for ( i = 0; i < microClustersList.size(); i++){
      for (j = i + 1; j < microClustersList.size(); j++){
        double distance=microClustersList.at(i)->getDistance(microClustersList.at(j));
        if (distance < dbStreamParams.radius)
          move= false;
      }
    }
  }
  else
    move=false;
  return move;
}

void  SESAME::DBStream::cleanUp(clock_t nowTime){
  std::vector<MicroClusterPtr> removeMicroCluster;
  std::vector<MicroClusterPtr>::size_type iter;
  //Check the current micro Clusters whether they have weak MCs
  //This just test for remove id
  std::vector<int> idList;
  for (iter=0;iter<microClusters.size();iter++)
  {
    if (microClusters.at(iter)->weight <= this->weakEntry)
    {
      removeMicroCluster.push_back(microClusters.at(iter)->copy());
      idList.push_back(microClusters.at(iter)->id.front());
      microClusters.erase(microClusters.begin()+int(iter));//Delete this MC from current MC list
    }
  }
// SESAME_INFO("now rm MCs number is "<<removeMicroCluster.size()<<", MCs is "<<microClusters.size());
  std::stringstream re;
  std::copy(idList.begin(),idList.end(),std::ostream_iterator<int>(re, " "));
 //SESAME_INFO("RM list "<<re.str());
  auto iterW=weightedAdjacencyList.begin();
  while ( iterW != weightedAdjacencyList.end())
  {
   auto exist1 = std::find_if(removeMicroCluster.begin(), removeMicroCluster.end(),SESAME::finderMicroCluster(iterW->first.microCluster1->id.front()));
   auto exist2 = std::find_if(removeMicroCluster.begin(), removeMicroCluster.end(),SESAME::finderMicroCluster(iterW->first.microCluster2->id.front()));

   if ( exist1!=removeMicroCluster.end()|| exist2!=removeMicroCluster.end())
     iterW=weightedAdjacencyList.erase(iterW);
   else {
       double decayFactor=dampedWindow->decayFunction(iterW->second->updateTime,nowTime);
       if (iterW->second->getCurrentWeight(decayFactor) < aWeakEntry)
         iterW=weightedAdjacencyList.erase(iterW);
       else
         iterW++;
   }
  }
//  SESAME_INFO("CLEAN! now weightedAdjacencyList size:"<<weightedAdjacencyList.size());
}


void SESAME::DBStream::runOfflineClustering(DataSinkPtr sinkPtr) {
  SESAME_INFO("micro clusters "<<microClusters.size());
  SESAME_INFO("weightedAdjacencyList  "<<weightedAdjacencyList.size());
  connectedRegions.connection(microClusters,
                   weightedAdjacencyList);

  std::vector<PointPtr> points = connectedRegions.ResultsToDataSink();
  for(auto & point : points)
    sinkPtr->put(point->copy());

}


/*
   * reCluster(dbStreamParams.alpha);
  for(auto iter=0; iter!=finalClusters.size();iter++)
  {   //initialize pseudo point of macro clusters
    PointPtr point = DataStructureFactory::createPoint(iter, 0, finalClusters.at(iter).front()->dimension, 0);
    //This is just for testing, need to delete
    std::vector<double> centroid(finalClusters.at(iter).front()->dimension,0);
    for(auto j=0; j!=finalClusters.at(iter).size();j++)
    {
      double currentWeight=point->getWeight()+finalClusters.at(iter).at(j)->weight;
      point->setWeight(currentWeight);
      for(auto a =0;a<finalClusters.at(iter).at(j)->dimension;a++)
      {
        if(j==0)
          point->setFeatureItem(0,a);
        point->setFeatureItem(point->getFeatureItem(a)+finalClusters.at(iter).at(j)->centroid.at(a),a);
        centroid[a]=point->getFeatureItem(a);//testing
        if(j==finalClusters.at(iter).size()-1)
        {
          point->setFeatureItem(point->getFeatureItem(a)/finalClusters.at(iter).at(j)->dimension,a);
          centroid[a] =centroid[a]/finalClusters.at(iter).at(j)->dimension;//testing
        }
      }
    }
    std::stringstream re;
    std::copy(centroid.begin(),centroid.end(),std::ostream_iterator<double>(re, " "));
    SESAME_INFO("The NO."<<iter<<" Centroid is "<<re.str());
    sinkPtr->put(point->copy()); // point index start from 0
  }

void SESAME::DBStream::reCluster(double threshold){
  WeightedAdjacencyList::iterator iterW;
  for (iterW = weightedAdjacencyList.begin(); iterW != weightedAdjacencyList.end(); iterW++){
    if (iterW->first.microCluster1->weight >= dbStreamParams.weightMin &&iterW->first.microCluster2->weight >= dbStreamParams.weightMin){
      double val = 2*iterW->second->weight / (iterW->first.microCluster1->weight+iterW->first.microCluster2->weight);
      if (val > threshold) {
        insertIntoGraph( iterW->first.microCluster1->id.front(),iterW->first.microCluster2->id.front());
        insertIntoGraph(iterW->first.microCluster2->id.front(), iterW->first.microCluster1->id.front());
      }
      else
      {
        insertIntoGraph(iterW->first.microCluster1->id.front());
        insertIntoGraph(iterW->first.microCluster2->id.front());
      }
    }
  }
  findConnectedComponents();
}

void SESAME::DBStream::insertIntoGraph(int microClusterId,int OtherId){
  if (connecvtivityGraphId.find(microClusterId)!=connecvtivityGraphId.end())
  {
    if(std::find(connecvtivityGraphId[microClusterId].begin(),connecvtivityGraphId[microClusterId].end(),OtherId)==connecvtivityGraphId[microClusterId].end())
      connecvtivityGraphId[microClusterId].push_back(OtherId);
  } else{
    auto microCluster = std::find_if(microClusters.begin(), microClusters.end(),SESAME::finderMicroCluster(microClusterId));
    (*microCluster)->visited=false;
    std::vector<int> newMicroClusterIdSet;
    newMicroClusterIdSet.push_back(OtherId);
    connecvtivityGraphId.insert(make_pair(microClusterId,OtherId));
    //SESAME_INFO("Key cluster size: "<<keyMicroCluster.size()<<" Clusters size "<<ValueClusters.size());
  }
}

void SESAME::DBStream::insertIntoGraph(int microClusterId){
  if (connecvtivityGraphId.find(microClusterId)==connecvtivityGraphId.end())
  {
    auto microCluster = std::find_if(microClusters.begin(), microClusters.end(),SESAME::finderMicroCluster(microClusterId));
    (*microCluster)->visited=false;
    std::vector<int> newMicroClusterIdSet;
    connecvtivityGraphId.insert(make_pair(microClusterId,newMicroClusterIdSet));
  }
}

void SESAME::DBStream::findConnectedComponents(){
  unordered_map<int,std::vector<int>>::iterator iter;
  //This variable just for indicating the id of micro cluster which forming macro clusters
  for (iter = connecvtivityGraphId.begin(); iter != connecvtivityGraphId.end(); iter++){
    std::vector<int> idList;
    auto microClusterKey = std::find_if(microClusters.begin(), microClusters.end(),SESAME::finderMicroCluster(iter->first));
    if (!(*microClusterKey)->visited) {
      std::vector<MicroClusterPtr> newCluster;
      newCluster.push_back((*microClusterKey));
      idList.push_back(iter->first);
      for(int iterValue : iter->second)
      {
        auto microClusterElement = std::find_if(microClusters.begin(), microClusters.end(),SESAME::finderMicroCluster(iterValue));
        if (!(*microClusterElement)->visited)
        {
          newCluster.push_back((*microClusterElement));
          (*microClusterElement)->visited = true;
          idList.push_back((*microClusterElement)->id.front());
        }
      }
      this->finalClusters.push_back(newCluster);
      //just used for examine reform ,need to delete later
      std::stringstream result;
      std::copy(idList.begin(),idList.end(),std::ostream_iterator<int>(result, " "));
      //SESAME_INFO("New formed macro cluster ... including micro cluster :");
      //SESAME_INFO("  " << result.str() );
    }
  }
}
 */