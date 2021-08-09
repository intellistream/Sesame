//
// Created by 1124a on 2021/8/8.
//


#include<Algorithm/CluStream.hpp>
#include <Utils/Logger.hpp>
#include <Algorithm/WindowModel/WindowFactory.hpp>
#include <Algorithm/DataStructure/DataStructureFactory.hpp>
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
    microClusters[i]=NULL;
  }

  this->km.runKMeans(CluStreamParam.clusterNumber, CluStreamParam.initBuffer, initData,initialData,true);
  for (int i = 0;i <  CluStreamParam.initBuffer;i++)
  { int clusterId=initialData[i]->getClusteringCenter();
    if(microClusters[clusterId]==NULL )
    {
      // microClusters[cluster_id].reset(new MicroCluster(radiusFactor, lastArrivingNum,dimension,cluster_id));
      microClusters[clusterId]=SESAME::DataStructureFactory::createMicroCluster(CluStreamParam.dimension,clusterId);
      microClusters[clusterId]->init(initialData[i],
                                     ((double)(clock()-startTime))/CLOCKS_PER_SEC);
    }
    microClusters[clusterId]->insert(initialData[i],
                                     ((double)(clock()-startTime))/CLOCKS_PER_SEC);
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
  this->window = WindowFactory::createLandmarkWindow();
  this->startTime=clock();
  clock_t lastTime = startTime;
  window->initPyramidalWindow(this->CluStreamParam.timeInterval);
  vector<PointPtr> initData;//initialData
 // initialData.assign(input.begin() ,input.begin()+this->CluStreamParam.initBuffer);
  initOffline(const_cast<vector<PointPtr> &>(input),initData);

  for(int i=this->CluStreamParam.initBuffer;i<this->CluStreamParam.pointNumber;i++)
  {
    clock_t now = clock();
    if((int)((now-lastTime)/CLOCKS_PER_SEC)>1000)
    {
      window->pyramidalWindow(now,microClusters);
    }
    incrementalCluster(input[i]);
  }

  SESAME_INFO("Initialization succeed!");
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
  MicroClusterPtr closestCluster;
  double minDistance=doubleMax;
  for (int i = 0; i < this->CluStreamParam.clusterNumber; i++) {
    double dist =  microClusters[i]->calCentroidDistance(data);
    if (dist < minDistance)
    {
      closestCluster=microClusters[i];
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
double SESAME::CluStream::calRadius(MicroClusterPtr closestCluster)
{
  double radius;
  if (closestCluster->clusterNum == 1) {
    // Special case: estimate radius by determining the distance to the
    // next closest cluster
    radius =doubleMax;
    dataPoint centroid = closestCluster->getCentroid();
    for (int i = 0; i <this->CluStreamParam.clusterNumber; i++) {
      if (microClusters[i]->id == closestCluster->id) //TODO Need to modify
      {
        continue;
      }
      double dist = distance(microClusters[i]->getCentroid(), centroid,this->CluStreamParam.dimension);
      radius = std::min(dist, radius);
    }
  }
  else
    radius = closestCluster->getRadius(this->CluStreamParam.radiusFactor);

  return radius;
}


//   Date fits case
void SESAME::CluStream::insertIntoCluster(PointPtr data,  MicroClusterPtr operateCluster)
{
  SESAME_INFO("This data fits");
  pointsFitted++;
  operateCluster->insert(data, ((double)(clock()-startTime))/CLOCKS_PER_SEC);
}

//Delete oldest cluster and create new one case
void SESAME::CluStream::deleteCreateCluster(PointPtr data)
{
  // 3.1 Try to forget old micro clusters
  double threshold = ((double)(clock()-startTime))/CLOCKS_PER_SEC - this->CluStreamParam.timeWindow; // Kernels before this can be forgotten

  for (int i = 0; i <this->CluStreamParam.clusterNumber; i++) {
    if (microClusters[i]->getRelevanceStamp(this->CluStreamParam.lastArrivingNum) < threshold)
    {
      unsigned int newId=this->CluStreamParam.clusterNumber+pointsForgot+pointsMerged;
      //microClusters[i].reset(new MicroCluster(data,((double)(clock()-startTime))/CLOCKS_PER_SEC, radiusFactor,lastArrivingNum, dimension,newId));
      microClusters[i].reset();//TODO consider moving it
      microClusters[i]=SESAME::DataStructureFactory::createMicroCluster(this->CluStreamParam.dimension,newId);
      microClusters[i]->init(data,
                             ((double)(clock()-startTime))/CLOCKS_PER_SEC);
      pointsForgot++;
      return;
    }
  }

}

// Merge two closest clusters & create a new cluster

void SESAME::CluStream::MergeCreateCluster(PointPtr data)
{
  unsigned int closestA = 0;
  unsigned int closestB = 0;
  double minDistance = doubleMax;
  for (int i = 0; i < this->CluStreamParam.clusterNumber; i++) { //O(n(n+1)/2)
    dataPoint centroid_a = microClusters[i]->getCentroid();
    for (int j = i + 1; j < this->CluStreamParam.clusterNumber; j++) {
      double dist = distance(centroid_a, microClusters[j]->getCentroid(),this->CluStreamParam.dimension);
      if (dist < minDistance) {
        minDistance = dist;
        closestA = i;
        closestB = j;
      }
    }
  }
  pointsMerged++;
  microClusters[closestA]->merge(microClusters[closestB]);
  unsigned int newId=this->CluStreamParam.clusterNumber+pointsForgot+pointsMerged;
  /*
  microClusters[closest_b].reset(new MicroCluster(data,
                                                  ((double)(clock()-startTime))/CLOCKS_PER_SEC,
                                                  radiusFactor,
                                                  clusterNumber, dimension,new_id));*/
  microClusters[closestB].reset();//TODO consider moving it
  microClusters[closestB]=SESAME::DataStructureFactory::createMicroCluster(this->CluStreamParam.dimension,newId);
  microClusters[closestB]->init(data,
                                ((double)(clock()-startTime))/CLOCKS_PER_SEC);

}
// TODO : need to modify KMeans.cpp for initialization part
void SESAME::CluStream::runOfflineClustering(const std::vector<PointPtr> &input, vector<PointPtr> &output)
{
  clock_t now=clock();
  vector<PointPtr> centers;
  vector<vector<PointPtr>> groups;
  int elapsedTime=(int)((now-startTime)/CLOCKS_PER_SEC);
  int landmarkTime=elapsedTime-this->CluStreamParam.timeWindow;
  SESAME::MicroClustersPtr landmarkSnapshot;
  landmarkSnapshot=SESAME::Snapshot::findSnapshot(window->orderSnapShots,
                                                  landmarkTime,elapsedTime,window->currentOrder);
  SESAME::MicroClustersPtr substractMiroCluster;
  substractMiroCluster=SESAME::Snapshot::substractSnapshot(microClusters,landmarkSnapshot,this->CluStreamParam.clusterNumber);
  vector<PointPtr> TransformedSnapshot;
  microClusterToPoint(substractMiroCluster,TransformedSnapshot);
  this->km.runKMeans(this->CluStreamParam.offlineClusterNumber, this->CluStreamParam.clusterNumber,
                     TransformedSnapshot,centers,true);
  this->km.groupPointsByCenters((int)centers.size(), (int)input.size(),
                                const_cast<vector<PointPtr> &>(input), centers, groups);
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
  cout << endl;
}

void SESAME::CluStream::microClusterToPoint(SESAME::MicroClustersPtr microClusters, vector<PointPtr> &points)
{
  for(int i=0;i<this->CluStreamParam.clusterNumber;i++)
  {
    PointPtr point = DataStructureFactory::createPoint(i, microClusters[i]->clusterNum, 5, 0);
    for(int j=0;j< microClusters[i]->centroid.size();j++)
      point->setFeatureItem(microClusters[i]->centroid[j],j);
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