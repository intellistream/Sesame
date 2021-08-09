//
// Created by 1124a on 2021/8/8.
//


#include<Algorithm/CluStream.hpp>
#include <Algorithm/WindowModel/WindowFactory.hpp>
#include <Algorithm/DataStructure/DataStructureFactory.hpp>
#include <iterator>
SESAME::CluStream::CluStream()
{

}
SESAME::CluStream::~CluStream()
{

}
/**
 * @Description: "offline" init micro clusters using KMeans
 * @param size: The size of initial data obects,
 *  initialData:input intial data
 *@Return: void
 */
void SESAME::CluStream::initOffline(vector<PointPtr> &initData, vector<PointPtr> &initialData)
{

  pointsFitted = 0;
  pointsForgot = 0;
  pointsMerged = 0;

 for(int i=0;i<CluStreamParam.clusterNumber;i++)
  {
    microClusters.push_back(MicroCluster(CluStreamParam.dimension,i));
  }
  this->km.runKMeans(CluStreamParam.clusterNumber, CluStreamParam.initBuffer, initData,initialData,true);
  for (int i = 0;i <  CluStreamParam.initBuffer;i++)
  {
    int clusterId=initialData[i]->getClusteringCenter();
   // SESAME_INFO("the belonging micro cluster id is !"<<clusterId);
    if(microClusters[clusterId].clusterNum==0 )
    {
      //microClusters[clusterId]=SESAME::DataStructureFactory::createMicroCluster(CluStreamParam.dimension,clusterId);
      microClusters[clusterId].init(initialData[i],
                                     ((int)(clock()-startTime))/CLOCKS_PER_SEC);
    }
    else
    {  microClusters[clusterId].insert(initialData[i],
                                        ((int)(clock()-startTime))/CLOCKS_PER_SEC);}

  }


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
 * @Return: store the output result(with computed clustering center) into ???//TODO still need to refactor
 */
void SESAME::CluStream::runOnlineClustering(const vector<PointPtr> &input)
{
  SESAME_INFO("configure cmd_params.pointNumber: " << CluStreamParam.pointNumber);
  SESAME_INFO("configure cmd_params.initBuffer: " << CluStreamParam.initBuffer);
  SESAME_INFO("configure cmd_params.lastArrivingNum: " << CluStreamParam.lastArrivingNum);
  SESAME_INFO("configure cmd_params.timeWindow: " << CluStreamParam.timeWindow);
  SESAME_INFO("configure cmd_params.timeInterval: " << CluStreamParam.timeInterval);
  SESAME_INFO("configure cmd_params.clusterNumber: " << CluStreamParam.clusterNumber);
  SESAME_INFO("configure cmd_params.offlineClusterNumber: " << CluStreamParam.offlineClusterNumber);
  SESAME_INFO("configure cmd_params.radiusFactor: " << CluStreamParam.radiusFactor);
  SESAME_INFO("configure cmd_params.offlineTimeWindow: " << CluStreamParam.offlineTimeWindow);

  this->window = WindowFactory::createLandmarkWindow();
  this->window->pyramidalWindow.timeInterval=this->CluStreamParam.timeInterval;
  this->startTime=clock();

  int interval;
  clock_t lastTime = startTime;
  window->initPyramidalWindow(this->window->pyramidalWindow.timeInterval);
  vector<PointPtr> initData;//initialData
  initOffline(const_cast<vector<PointPtr> &>(input),initData);
  SESAME_INFO("INITIALIZATION SUCCEED!");
  for(int i=this->CluStreamParam.initBuffer;i<this->CluStreamParam.pointNumber;i++)
  {
    clock_t now = clock();
    interval=(int)((now-lastTime)/CLOCKS_PER_SEC);
    /*if(i%500==0)
    {
      for(int j=0;j<15;j++)
      {
        std::stringstream result;
        std::copy(microClusters[j]->id.begin(),microClusters[j]->id.end(), std::ostream_iterator<int>(result, " "));
        SESAME_INFO("The micro cluster id "<<result.str()<<"weight is "<<microClusters[j]->clusterNum);
      }
    }*/
  if(interval>=1)//
    {
      window->pyramidalWindowProcess(startTime,microClusters);
      lastTime=now;
   }
    incrementalCluster(input[i]);
  }
  SESAME_INFO("Online part succeed!");
}
/**
 * @Description: incrementally insert data into micro clusters,
 * first determine closest clusters to the data,
 * then judge whether it locates into the maximum boundary of this micro cluster
 * 1. data object fits into closest micro cluster;
 * 2. data does not fit in any cluster,  delete oldest one & create a new cluster;
 * 3.  merge two closest clusters & create a new cluster.
 * @Param: data: input data object
 * @Return: store the output result(with computed clustering center) into ???//TODO still need to refactor
 */
void SESAME::CluStream::incrementalCluster(PointPtr data)
{ // 1. Determine closest clusters
  MicroCluster closestCluster;
  double minDistance=doubleMax;
  for (int i = 0; i < this->CluStreamParam.clusterNumber; i++) {
    double dist =  microClusters[i].calCentroidDistance(data);

    if (dist < minDistance)
    {
      closestCluster=std::move(microClusters[i]);
      minDistance = dist;
    }
  }

  double radius= calRadius(closestCluster);
  if (minDistance < radius) {
  insertIntoCluster(data,closestCluster);
    return;
  }
/** 3. Date does not fit  -- free
 * some space to insert a new micro cluster
 * */
// 3.1 delete oldest one & create a new cluster
 deleteCreateCluster(data);
// 3.2 merge two closest clusters & create a new cluster
  MergeCreateCluster(data);

}

//Calculate and return the value of radius
double SESAME::CluStream::calRadius(MicroCluster &closestCluster)
{
  double radius;
  if (closestCluster.clusterNum == 1) {
    // Special case: estimate radius by determining the distance to the
    // next closest cluster
    radius =doubleMax;
    dataPoint centroid = closestCluster.getCentroid();
    for (int i = 0; i <this->CluStreamParam.clusterNumber; i++) {
     // SESAME_INFO("i is for wegf"<<i);
      if (microClusters[i].id == closestCluster.id) //TODO Need to modify
      {
        continue;
      }
      double dist = distance(microClusters[i].getCentroid(), centroid,this->CluStreamParam.dimension);
      radius = std::min(dist, radius);
    }
  }
  else
    radius = closestCluster.getRadius(this->CluStreamParam.radiusFactor);

  return radius;
}


//   Date fits case
void SESAME::CluStream::insertIntoCluster(PointPtr data,  MicroCluster &operateCluster)
{
  //SESAME_INFO("This data fits");
  pointsFitted++;
  operateCluster.insert(data, ((int)(clock()-startTime))/CLOCKS_PER_SEC);

}

//Delete oldest cluster and create new one case
void SESAME::CluStream::deleteCreateCluster(PointPtr data)
{
 // SESAME_INFO("Micro cluster needs to delete");
  // 3.1 Try to forget old micro clusters
  int threshold = ((int)(clock()-startTime))/CLOCKS_PER_SEC - this->CluStreamParam.timeWindow; // Kernels before this can be forgotten

  for (int i = 0; i <this->CluStreamParam.clusterNumber; i++) {
    if (microClusters[i].getRelevanceStamp(this->CluStreamParam.lastArrivingNum) < threshold)
    {
      unsigned int newId=this->CluStreamParam.clusterNumber+pointsForgot+pointsMerged;
      //microClusters[i].reset(new MicroCluster(data,((double)(clock()-startTime))/CLOCKS_PER_SEC, radiusFactor,lastArrivingNum, dimension,newId));
      microClusters.erase(microClusters.begin()+i);
     // microClusters[i]=MicroCluster(this->CluStreamParam.dimension,newId);
      microClusters[i].clusterNum=0;
      vector <int>().swap(microClusters[i].id);
      microClusters[i].id.push_back(newId);
      microClusters[i].init(data,
                             ((int)(clock()-startTime))/CLOCKS_PER_SEC);
      pointsForgot++;
      return;
    }
  }

}

// Merge two closest clusters & create a new cluster

void SESAME::CluStream::MergeCreateCluster(PointPtr data)
{
  //SESAME_INFO("Micro cluster needs to merge");
  unsigned int closestA = 0;
  unsigned int closestB = 0;
  double minDistance = doubleMax;
  for (int i = 0; i < this->CluStreamParam.clusterNumber; i++) { //O(n(n+1)/2)
    dataPoint centroid_a = microClusters[i].getCentroid();
    for (int j = i + 1; j < this->CluStreamParam.clusterNumber; j++) {
      double dist = distance(centroid_a, microClusters[j].getCentroid(),this->CluStreamParam.dimension);
      if (dist < minDistance) {
        minDistance = dist;
        closestA = i;
        closestB = j;
      }
    }
  }
  pointsMerged++;
  microClusters[closestA].merge(microClusters[closestB]);
  unsigned int newId=this->CluStreamParam.clusterNumber+pointsForgot+pointsMerged;

  microClusters[closestB].clusterNum=0;
  vector <int>().swap(microClusters[closestB].id);
  microClusters[closestB].id.push_back(newId);
  microClusters[closestB].init(data,
                                ((int)(clock()-startTime))/CLOCKS_PER_SEC);

}
// TODO : need to modify KMeans.cpp for initialization part
void SESAME::CluStream::runOfflineClustering(const std::vector<PointPtr> &input, vector<PointPtr> &output)
{
    input; output;
    clock_t now=clock();
    vector<vector<PointPtr>> groups;
    int elapsedTime=(int)((now-startTime)/CLOCKS_PER_SEC);
    int landmarkTime=elapsedTime-this->CluStreamParam.offlineTimeWindow;
    if(this->CluStreamParam.offlineTimeWindow>elapsedTime)
       landmarkTime=0;
    SESAME_INFO("Start offline...");
    SESAME::SnapshotPtr landmarkSnapshot=SESAME::DataStructureFactory::createSnapshot();
    SESAME::SnapshotPtr substractMiroCluster=SESAME::DataStructureFactory::createSnapshot();
  //If offlineTimeWindow ==0, Only Observe the end results of micro clusters
    substractMiroCluster->microClusters=std::move(microClusters);
    substractMiroCluster->elapsedTime=elapsedTime;
    //The offline is to observe a process of data stream clustering
    if(CluStreamParam.offlineTimeWindow!=0)
    {//
      landmarkSnapshot=SESAME::Snapshot::findSnapshot(window->orderSnapShots,
                                                      landmarkTime,elapsedTime,window->pyramidalWindow.currentOrder);
      SESAME_INFO("Finish find Snapshot...");
      SESAME_INFO("Current MC: ");
      for(int i=0;i<CluStreamParam.clusterNumber;i++)
      {
        std::stringstream result;
        std::copy(substractMiroCluster->microClusters[i].id.begin(), substractMiroCluster->microClusters[i].id.end(), std::ostream_iterator<int>(result, " "));
        SESAME_INFO("The micro cluster id "<<result.str()<<"weight is "<<substractMiroCluster->microClusters[i].clusterNum);
      }
      SESAME_INFO("landmarkSnapshot MC: ");
      for(int i=0;i<CluStreamParam.clusterNumber;i++)
      {
        std::stringstream result;
        std::copy(landmarkSnapshot->microClusters[i].id.begin(), landmarkSnapshot->microClusters[i].id.end(), std::ostream_iterator<int>(result, " "));
        SESAME_INFO("The micro cluster id "<<result.str()<<"weight is "<<landmarkSnapshot->microClusters[i].clusterNum);
      }
     substractMiroCluster=SESAME::Snapshot::substractSnapshot(substractMiroCluster,landmarkSnapshot,
                                                              this->CluStreamParam.clusterNumber);
    }


   vector<PointPtr> TransformedSnapshot;
   microClusterToPoint(substractMiroCluster->microClusters,TransformedSnapshot);
  SESAME_INFO("offline Cluster Number "<<this->CluStreamParam.offlineClusterNumber<<"Total number of p: "<<TransformedSnapshot.size());
   this->km.runKMeans(this->CluStreamParam.offlineClusterNumber, this->CluStreamParam.clusterNumber,
                        TransformedSnapshot,output,true);
  /*
  for(int i=0;i<this->CluStreamParam.clusterNumber;i++)
  {
      int clusterId=centers[i]->getClusteringCenter();
  }
  this->km.groupPointsByCenters((int)centers.size(), (int)input.size(),
                            const_cast<vector<PointPtr> &>(input), centers, groups);*/

  /*
      // print the clustering information
      int cluster = 0;
      cout << cluster <<" cluster: ";
      for(int i = 0; i < groups.size(); i++) {
        if(cluster != centers.at(i)->getClusteringCenter()) {
          cluster = centers.at(i)->getClusteringCenter();
          cout << endl << cluster <<" cluster: ";
        }
        for(int j = 0; j < groups[i].size(); j++) {
          groups[i][j]->setClusteringCenter(centers[i]->getClusteringCenter());
          cout << groups[i][j]->getIndex() << " ";
          output.push_back(groups[i][j]);
        }
      }
      cout << endl;*/
}

void SESAME::CluStream::microClusterToPoint(std::vector <MicroCluster> microClusters, vector<PointPtr> &points)
{
  for(int i=0;i<this->CluStreamParam.clusterNumber;i++)
  {
    SESAME_INFO("Start transform"<<i);
    PointPtr point = DataStructureFactory::createPoint(i, microClusters[i].clusterNum, microClusters[i].centroid.size(), 0);
    for(int j=0;j< microClusters[i].centroid.size();j++)
      point->setFeatureItem(microClusters[i].centroid[j],j);
    points.push_back(point);
  }


}
double SESAME::CluStream::distance(dataPoint a,dataPoint b, int dim) {
  double temp = 0;
  for (int i = 0; i < dim; i++) {
    double diff = b[i] - a[i];
    temp += diff * diff;
  }
  return sqrt(temp);
}