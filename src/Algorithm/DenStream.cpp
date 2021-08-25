//
// Created by 1124a on 2021/8/23.
//
#include <Algorithm/DenStream.hpp>
#include <Algorithm/WindowModel/WindowFactory.hpp>
#include <Algorithm/DataStructure/DataStructureFactory.hpp>
SESAME::DenStream::DenStream(param_t &cmd_params){
  this->denStreamParams.pointNumber = cmd_params.pointNumber;
  this->denStreamParams.dimension = cmd_params.dimension;
  this->denStreamParams.minPoints = cmd_params.minPoints;
  this->denStreamParams.epsilon = cmd_params.epsilon;
  this->denStreamParams.base = cmd_params.base;
  this->denStreamParams.lambda = cmd_params.lambda;
  this->denStreamParams.mu = cmd_params.mu;
  this->denStreamParams.beta = cmd_params.beta;
  this->denStreamParams.initBufferSize = cmd_params.initBuffer;
}
SESAME::DenStream:: ~DenStream(){

}
void SESAME::DenStream::init(vector <PointPtr> &initData){
  this->pMicroClusterIndex=-1;
  this->oMicroClusterIndex=-1;
  for(int i=0;i<denStreamParams.initBufferSize;i++)
  {
    if(initData.at(i)->getClusteringCenter()==noVisited)
    {
      std::vector<int> pointIndex;
      pMicroClusterIndex++;
      MicroClusterPtr newMicroCluster=SESAME::DataStructureFactory::createMicroCluster(denStreamParams.dimension,pMicroClusterIndex);
      newMicroCluster->init(initData.at(i), 0);
      pointsNearCorePoint(initData,pointIndex,newMicroCluster);

      if(newMicroCluster->weight<=this->minWeight)//TODO need to change minweight
      {
        pMicroClusterIndex--;
        for(vector<int>::size_type a=0;a<pointIndex.size();a++)
        {
          int index=pointIndex.at(a);
          initData.at(index)->setClusteringCenter(noVisited);
        }
      }
      else
        pMicroClusters.push_back(newMicroCluster);
    }
  }
  SESAME_INFO("NOW PMC number is: "<<this->pMicroClusterIndex<<" , Init succeed!");
}
void SESAME::DenStream::pointsNearCorePoint(vector <PointPtr> &initData,std::vector<int> pointIndex,MicroClusterPtr microCluster){
  int size = denStreamParams.initBufferSize;

  for (int i = 0; i < size; i++)
  {
    if(initData.at(i)->getClusteringCenter()==noVisited)
    {
      double dist=microCluster->calCentroidDistance(initData.at(i));
      if (dist<denStreamParams.epsilon)
      {
        initData[i]->setClusteringCenter(microCluster->id.front());
        microCluster->insert(initData.at(i),0);
        pointIndex.push_back(i);
      }
    }
  }
}
void SESAME::DenStream::Initilize(){
  this->dampedWindow = WindowFactory::createDampedWindow(denStreamParams.base,  denStreamParams.lambda);
  this->dbscan=std::make_shared<DBSCAN>(denStreamParams.minPoints,denStreamParams.epsilon,denStreamParams.initBufferSize);
  this->startTime = clock();
  this->pointArrivingTime=this->startTime;
  this->minWeight=denStreamParams.beta*denStreamParams.mu;
  this->Tp=floor((1/denStreamParams.lambda)*(log(minWeight/(minWeight-1))/log(denStreamParams.base)));
}
void  SESAME::DenStream::runOnlineClustering(PointPtr input) {
  if (!this->isInitial) {
    Initilize();
    input->setClusteringCenter(noVisited);
    this->initialBuffer.push_back(input);
    if (this->initialBuffer.size() == this->denStreamParams.initBufferSize) {
      init(this->initialBuffer);
      this->isInitial = true;
    }
  }
  else {
    /*SESAME_INFO("configure cmd_params.initBuffer: " << denStreamParams.initBufferSize);
    SESAME_INFO("configure cmd_params.minPoints: " << denStreamParams.minPoints);
    SESAME_INFO("configure cmd_params.epsilon: " << denStreamParams.epsilon);
    SESAME_INFO("configure cmd_params.base: " << denStreamParams.base);
    SESAME_INFO("configure cmd_params.lambda: " << denStreamParams.lambda);
    SESAME_INFO("configure cmd_params.mu: " << denStreamParams.mu);
    SESAME_INFO("configure cmd_params.beta: " << denStreamParams.beta);
    SESAME_INFO("configure min weight: " << this->minWeight);*/

    this->pointArrivingTime=clock();
    merge(input);
    SESAME_INFO("Merge! ");
    if(((this->pointArrivingTime-this->startTime)/CLOCKS_PER_SEC) %this->Tp==0)
    {
      for(int iter=0;iter<pMicroClusters.size();iter++)
      {
        if(pMicroClusters.at(iter)->weight<minWeight)
        {
          pMicroClusters.erase(pMicroClusters.begin()+iter);
          pMicroClusterIndex--;
          SESAME_INFO("NOW PMC number is: "<<this->pMicroClusterIndex);
        }
      }
      if(oMicroClusters.size()!=0)
      {

        for(int iter=0;iter<oMicroClusters.size();iter++)
        {
          double a=(-(denStreamParams.lambda)*floor(((pointArrivingTime-oMicroClusters.at(iter)->createTime)
              /CLOCKS_PER_SEC+this->Tp)));
          double b=(-denStreamParams.lambda*this->Tp);
          double Xi=(pow(denStreamParams.base,a )-1)/(pow(denStreamParams.base,b)-1);

          if(oMicroClusters.at(iter)->weight<Xi)
          {
            oMicroClusters.erase(oMicroClusters.begin()+iter);
            oMicroClusterIndex--;
            SESAME_INFO("NOW PMC number is: "<<this->oMicroClusterIndex);
          }

        }
      }

    }
  }
}

void SESAME::DenStream::merge(PointPtr dataPoint){

  bool merged=false;
  int index=-1;
  if(!this->pMicroClusters.empty())
    merged=mergeToMicroCluster(dataPoint,this->pMicroClusters,index);
  SESAME_INFO(merged);
  if(!merged&&!this->oMicroClusters.empty())
  {
    merged=mergeToMicroCluster(dataPoint,this->oMicroClusters,index);
    if(merged)
    { double decayFactor= this->dampedWindow->decayFunction(this->oMicroClusters.at(index)->lastUpdateTime,pointArrivingTime);
      if((this->oMicroClusters.at(index)->weight)*decayFactor>minWeight)
      {
        pMicroClusterIndex++;
        oMicroClusters.at(index)->resetID(pMicroClusterIndex);
        pMicroClusters.push_back(oMicroClusters.at(index)->copy());
        oMicroClusters.erase(oMicroClusters.begin()+index);
      }
    }
  }
  if(!merged)
  {
    oMicroClusterIndex++;
    oMicroClusters.push_back(DataStructureFactory::createMicroCluster(denStreamParams.dimension, oMicroClusterIndex));
  }
}
void SESAME::DenStream::runOfflineClustering(DataSinkPtr sinkPtr) {
  vector <PointPtr> transformedPoints;
  std::vector<std::vector<PointPtr>> oldGroups;
  microClusterToPoint(pMicroClusters,transformedPoints);
  this->dbscan->run(transformedPoints);

  this->dbscan->produceResult(transformedPoints,sinkPtr);

}
void SESAME::DenStream::microClusterToPoint(std::vector<MicroClusterPtr> &microClusters,
                                            vector <PointPtr> &points){
  for (std::vector<MicroClusterPtr>::size_type i=0; i < microClusters.size(); i++) {
    PointPtr
    point = DataStructureFactory::createPoint(i, microClusters.at(i)->weight, microClusters.at(i)->centroid.size(), 0);
    for (int j = 0; j < microClusters.at(i)->centroid.size(); j++)
      point->setFeatureItem(microClusters[i]->centroid[j], j);
    points.push_back(point);
  }
}
bool SESAME::DenStream::mergeToMicroCluster(PointPtr dataPoint,std::vector <MicroClusterPtr> microClusters,int index ){
  bool succeed=false;
  index=nearestNeighbor(dataPoint,microClusters);
  if(index!=-1)
  {
    double decayFactor= this->dampedWindow->decayFunction(microClusters.at(index)->lastUpdateTime,pointArrivingTime);
    if(microClusters.at(index)->insert(dataPoint,decayFactor,denStreamParams.epsilon))
      succeed=true;
  }
  return succeed;
}
int SESAME::DenStream::nearestNeighbor(PointPtr dataPoint,std::vector <MicroClusterPtr> microClusters){
  int index=-1;
  double dist=0,minDist= std::numeric_limits<double>::max();
  for(int i=0;i<microClusters.size();i++)
  {
    dist=microClusters.at(i)->calCentroidDistance(dataPoint);
    if(dist<minDist)
    {
      minDist=dist;
      index=i;
    }
  }
  return index;
}
