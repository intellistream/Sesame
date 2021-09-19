//
// Created by Zhenyu on 2021/8/30.
//
#include <Algorithm/DBStream.hpp>
#include <Algorithm/WindowModel/WindowFactory.hpp>
#include <Algorithm/DataStructure/DataStructureFactory.hpp>

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
void SESAME::DBStream::runOfflineClustering(DataSinkPtr sinkPtr) {
   SESAME_INFO("Start offline...");
  sinkPtr;
  reCluster(dbStreamParams.alpha);
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
 // SESAME_INFO("decayFactor is..."<<decayFactor);
  this->pointArrivingTime=clock();
  this->microClusterNN=findFixedRadiusNN(dataPoint);
  std::vector<MicroClusterPtr>::size_type sizeNN=microClusterNN.size();
  // SESAME_INFO("find suitable MCs number : "<<sizeNN);
  if (microClusterNN.empty()) {
    microClusterIndex++;

    MicroClusterPtr newMicroCluster=SESAME::DataStructureFactory::createMicroCluster(dbStreamParams.dimension,microClusterIndex,
                                                                                     dataPoint,dbStreamParams.radius);

    microClusters.push_back(newMicroCluster);
    microClusterNN.push_back(newMicroCluster);
 // SESAME_INFO("Add new MC!"<<microClusterIndex);
  }
  else {
    for (int i = 0; i < sizeNN; i++) {
      //SESAME_INFO("insert into existing MCs! id "<< microClusterNN.at(i)->id.front());
      MicroClusterPtr microCluster = microClusterNN.at(i)->copy();
      microCluster->insert(dataPoint,decayFactor); // just update weight
      // update shared density
      for (int j = i + 1; j < sizeNN; j++) {
        MicroClusterPair microClusterPair(microCluster->copy(), microClusterNN.at(j)->copy());
       // SESAME_INFO("pair "<< microClusterNN.at(i)->id.front()<<" "<< microClusterNN.at(j)->id.front());
        if (weightedAdjacencyList.find(microClusterPair) != weightedAdjacencyList.end())
        {
       // SESAME_INFO("find microClusterPair!"<<microClusterNN.at(i)->id.front()<<", "<<microClusterNN.at(j)->id.front());
          clock_t startT= weightedAdjacencyList[microClusterPair]->updateTime;
          double decayValue = dampedWindow->decayFunction(startT,this->pointArrivingTime);
          weightedAdjacencyList[microClusterPair]->add(this->pointArrivingTime,decayValue);
         // SESAME_INFO("pair weight = "<<weightedAdjacencyList[microClusterPair]->weight );
       //  SESAME_INFO(" weight is "<<weightedAdjacencyList[microClusterPair]->weight );
        }else{
        //  SESAME_INFO("Create microClusterPair!" << microClusterNN.at(i)->id.front()<<", "<<microClusterNN.at(j)->id.front());
          AdjustedWeightPtr adjustedWeight = SESAME::DataStructureFactory::createAdjustedWeight(1,this->pointArrivingTime);
          DensityGraph densityGraph( microClusterPair ,adjustedWeight);
          weightedAdjacencyList.insert(densityGraph);

         // SESAME_INFO("new one weight is "<<weightedAdjacencyList[microClusterPair]->weight );
         // SESAME_INFO("size is"<<weightedAdjacencyList.size() );
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
   SESAME_INFO("Clean !");
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
  //SESAME_INFO("now rm MCs number is "<<removeMicroCluster.size()<<", MCs is "<<microClusters.size());
  std::stringstream re;
  std::copy(idList.begin(),idList.end(),std::ostream_iterator<int>(re, " "));
  SESAME_INFO("RM list "<<re.str());
  for (auto iterW = weightedAdjacencyList.begin(); iterW != weightedAdjacencyList.end(); ++iterW)
  {
    nowTime;

   auto exist1 = std::find_if(removeMicroCluster.begin(), removeMicroCluster.end(),SESAME::finderMicroCluster(iterW->first.microCluster1->id.front()));
   auto exist2 = std::find_if(removeMicroCluster.begin(), removeMicroCluster.end(),SESAME::finderMicroCluster(iterW->first.microCluster2->id.front()));
      if ( exist1!=removeMicroCluster.end()|| exist2!=removeMicroCluster.end())
      {
       // SESAME_INFO("weightedAdjacencyList size:"<<weightedAdjacencyList.size());
       weightedAdjacencyList.erase(iterW);
        SESAME_INFO("CLEAN existing pairs...");
       // SESAME_INFO("now weightedAdjacencyList size:"<<weightedAdjacencyList.size());
      }
    else {
    // if( exist1==removeMicroCluster.end()|| exist2==removeMicroCluster.end()){
       double decayFactor=dampedWindow->decayFunction(iterW->second->updateTime,nowTime);
     //  SESAME_INFO("Check existing entries... "<<" "<<iterW->second->getCurrentWeight(decayFactor));
       if (iterW->second->getCurrentWeight(decayFactor) < aWeakEntry)

        // SESAME_INFO("erase... "<<" "<<decayFactor);
        weightedAdjacencyList.erase(iterW->first);
         //SESAME_INFO("pair is "<<iterW->first.microCluster1->id.front()<<" "<<iterW->first.microCluster2->id.front());

     }
  }
}

void SESAME::DBStream::reCluster(double threshold){
  unordered_map<MicroClusterPtr ,std::vector<MicroClusterPtr>> connectivityGraph;//Connectivity graph C in DBStream paper
  WeightedAdjacencyList::iterator interW;
  for (interW = weightedAdjacencyList.begin(); interW != weightedAdjacencyList.end(); interW++){
    MicroClusterPtr microCluster1 =interW->first.microCluster1->copy();
    MicroClusterPtr microCluster2 =interW->first.microCluster2->copy();
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
/**
 * @Description:  insert vertices and entries into connectivity graph when micro cluster pair
 * connectivity value greater than the intersection threshold
 * if the graph has testing micro cluster, add connected strong MC in the corresponding entries
 * else, create new V,E into the graph
 * @Param: connectivity graph, micro cluster 1 and 2
 * @Return: void
 */

void SESAME::DBStream::insertIntoGraph(unordered_map<MicroClusterPtr ,std::vector<MicroClusterPtr>> connectivityGraph,
                            MicroClusterPtr microCluster,MicroClusterPtr Other){
  if (connectivityGraph.find(microCluster)!=connectivityGraph.end())
    connectivityGraph.find(microCluster)->second.push_back(Other);
  else{
    microCluster->visited = false;
    std::vector<MicroClusterPtr> newMicroClusterSet;
    newMicroClusterSet.push_back(Other);
    connectivityGraph.insert(std::make_pair(microCluster,newMicroClusterSet));
  }
}


void SESAME::DBStream::insertIntoGraph(unordered_map<MicroClusterPtr ,std::vector<MicroClusterPtr>> connectivityGraph,
                                       MicroClusterPtr microCluster){
  if (connectivityGraph.find(microCluster)==connectivityGraph.end())
  {
    microCluster->visited = false;
    std::vector<MicroClusterPtr> newMicroClusterSet;
    connectivityGraph.insert(std::make_pair(microCluster,newMicroClusterSet));
  }
}

/**
 * @Description:  findConnectedComponents function visit the existing connectivity graph
 * and find all connected strong MCs that will finally form arbitrary-shaped macro clusters
 * each macro cluster will be stored as a vector of micro clusters, which will be transformed into
 * point that stores in sink later
 * @Param: connectivity graph
 * @Return: void
 */
void SESAME::DBStream::findConnectedComponents(unordered_map<MicroClusterPtr,
                                               std::vector<MicroClusterPtr>> connectivityGraph){
  unordered_map<MicroClusterPtr,std::vector<MicroClusterPtr>>::iterator iter;
  //This variable just for indicating the id of micro cluster which forming macro clusters
  std::vector<int> idList;
  for (iter = connectivityGraph.begin(); iter != connectivityGraph.end(); iter++){
    if (!iter->first->visited) {
      std::vector<MicroClusterPtr> newCluster, clusterGroup;
      newCluster.push_back(iter->first);
      for(const auto & iterS : iter->second)
      {
        if (!iterS->visited)
          newCluster.push_back(iterS);
      }
      SESAME_INFO("New formed macro cluster ... including micro cluster :");
      while (!newCluster.empty())
      {
        //after found the front one, insert it into clusterGroup and delete from the original vector
        MicroClusterPtr microCluster = newCluster.front();
        newCluster.erase(newCluster.begin());
        clusterGroup.push_back(microCluster);
        idList.push_back(microCluster->id.front());
        microCluster->visited = true;
      }
      this->finalClusters.push_back(clusterGroup);
      //just used for examine reform ,need to delete later
      std::stringstream result;
      std::copy(idList.begin(),idList.end(),std::ostream_iterator<int>(result, " "));
      SESAME_INFO("  " << result.str() );
    }
  }
}
