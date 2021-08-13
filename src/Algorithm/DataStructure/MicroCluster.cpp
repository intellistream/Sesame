//
// Created by Zhenyu on 2021/8/8.
//
#include<Algorithm/DataStructure/MicroCluster.hpp>
#include <Algorithm/DataStructure/DataStructureFactory.hpp>
#include <Utils/Logger.hpp>
#include <iterator>

SESAME::MicroCluster::MicroCluster(int dimension, int id)
{
  this->dimension=dimension;
  clusterNum=0;
  this->id.push_back(id);
}

//Release memory of the current micro cluster
SESAME::MicroCluster::~MicroCluster()
{
  std::vector <int>().swap(id);
  std::vector <double>().swap(centroid);
}
void SESAME::MicroCluster::init(PointPtr datapoint,int timestamp)
{

  clusterNum++;
  for (int i = 0;i < dimension;i++) {
    double data = datapoint->getFeatureItem(i);
    CF1x.push_back(data);
    CF2x.push_back(data * data);
    centroid.push_back(data);
  }
  CF1t=timestamp;
  CF2t=timestamp*timestamp;

}
void SESAME::MicroCluster::reInit(int id,PointPtr datapoint, int timestamp)
{
   clusterNum=1;
  std::vector <int>().swap(this->id);
  std::vector <double>().swap(this->CF1x);
  std::vector <double>().swap(this->CF2x);
  std::vector <double>().swap(this->centroid);

  for (int i = 0;i < dimension;i++) {
    double data = datapoint->getFeatureItem(i);
    CF1x.push_back(data);
    CF2x.push_back(data * data);
    centroid.push_back(data);
  }
  CF1t=timestamp;
  CF2t=timestamp*timestamp;
  this->id.push_back(id);



}
//insert a new data point from input data stream
void SESAME::MicroCluster::insert(PointPtr datapoint,int timestamp)
{
  clusterNum++;
  for(int i=0;i<CF1x.size();i++)
  {
    double data=datapoint->getFeatureItem(i);
    CF1x[i]+=data;
    CF2x[i]+=data*data;
  }

  CF1t+=timestamp;
  CF2t+=timestamp*timestamp;

  centroid=std::move(getCentroid());

}

//merge two micro-clusters
void SESAME::MicroCluster::merge(MicroCluster &other){
  clusterNum+=other.clusterNum;
  for(int i=0;i<dimension;i++)// dimension can change to CF1x.size()
  {
    CF1x[i]+=other.CF1x[i];
    CF2x[i]+=other.CF2x[i];
  }
  CF1t+=other.CF1t;
  CF2t+=other.CF2t;
  updateId(other);
  centroid=std::move(getCentroid());

}

//Calculate the process of micro cluster N(Tc-h')
void SESAME::MicroCluster::substractClusterVector(MicroCluster &other)
{
  this->clusterNum-=other.clusterNum;
  for(int i=0;i<dimension;i++)
  {
    this->CF1x[i]-=other.CF1x[i];
    this->CF2x[i]-=other.CF2x[i];
  }
  this->CF1t-= other.CF1t;
  this->CF2t-= other.CF2t;
  centroid=std::move(getCentroid());

}
bool SESAME::MicroCluster::judgeMerge(MicroCluster &other)
{   bool merge=true;
  for(unsigned int i=0;i<other.id.size();i++)
  {
    if (std::find(this->id.begin(), this->id.end(), other.id[i])==id.end())
      merge=false;
  }
  return merge;

}

//update id list of Micro cluster
void SESAME::MicroCluster::updateId(MicroCluster &other)
{
  for(unsigned int i=0;i<other.id.size();i++)
  {
    this->id.push_back(other.id[i]);
  }
  std::vector <int>().swap(other.id);
  this->id.reserve(10);
}

//obtain relevance stamp of a cluster to judge whether it needs to be deleted

double SESAME::MicroCluster::getRelevanceStamp(int m) const
{
  if(clusterNum<(2*m))
    return getMutime();
  return getMutime() + getSigmaTime() * getQuantile( ((double)m)/(2*clusterNum) );
}

//mean_timestamp

double SESAME::MicroCluster::getMutime() const{
  return CF1t/clusterNum;
}

//standard_deviation_timestamp

double SESAME::MicroCluster::getSigmaTime() const
{
  return sqrt(CF2t/clusterNum - (CF1t/clusterNum)*(CF1t/clusterNum));
}


double SESAME::MicroCluster::getQuantile(double z)
{
  assert( z >= 0 && z <= 1 );
  return sqrt( 2 ) * inverseError( 2*z - 1 );
}


double SESAME::MicroCluster::getRadius(double radiusFactor){
  if(clusterNum==1)
    return 0;
  if(radiusFactor<=0)
    radiusFactor=radiusFactor;

  return getDeviation()*radiusFactor;
}

//calculate RMS deviation very confused

double SESAME::MicroCluster::getDeviation(){
  SESAME::dataPoint variance=getVarianceVector();
  double sumOfDeviation=0;
  for(int i=0;i<dimension;i++){
    sumOfDeviation += sqrt(variance[i]);
  }

  return  sumOfDeviation/dimension;
}

//calculate centroid of a  cluster
SESAME::dataPoint SESAME::MicroCluster::getCentroid(){
  if(clusterNum==1)
    return CF1x;
  dataPoint dataObject(CF1x.size());//double
  if(clusterNum>1){
    for(int i=0;i<centroid.size();i++){
      dataObject[i]=(CF1x[i]/clusterNum);
    }
  }
  return dataObject;
}

double SESAME::MicroCluster::getInclusionProbability(PointPtr datapoint,double radiusFactor) {
  if(clusterNum==1)
  {
    double distance=0;
    for(int i=0;i<dimension;i++){
      double d=CF1x[i]-datapoint->getFeatureItem(i);
      distance+=d*d;
    }
    distance=sqrt(distance);
    if(distance<EPSILON)
      return 1;
    return 0;
  }
  else{
    double dist= calCentroidDistance(datapoint);
    if(dist<=getRadius(radiusFactor))
      return 0;
    else
      return 1;
  }
}

//Calculate the standard deviation of vector in micro clusters

SESAME::dataPoint SESAME::MicroCluster::getVarianceVector(){
  dataPoint datapoint;

  for(int i=0;i<dimension;i++){
    double linearSum=CF1x[i];
    double squaredSum=CF2x[i];

    double aveLinearSum=			linearSum/clusterNum;
    double squaredAveLinearSum=	aveLinearSum*aveLinearSum;
    double squaredSumSquared=		squaredSum/clusterNum;
    datapoint.push_back(squaredSumSquared-squaredAveLinearSum);

    if(datapoint[i]<=0.0)
      datapoint[i]=MIN_VARIANCE;
  }
  return datapoint;
}


// calculate the distance between input data point and centroid of micro-clusters

double SESAME::MicroCluster::calCentroidDistance(PointPtr datapoint){

  double temp=0;
  for(int i=0;i<dimension;i++){
    double diff=centroid[i]-datapoint->getFeatureItem(i);
    temp+=(diff*diff);
  }
  return sqrt(temp);
}

double SESAME::MicroCluster::inverseError(double x){
  double z = sqrt(M_PI) * x;
  double res = (z) / 2;

  double z2 = z * z;
  double zProd = z * z2; // z^3
  res += (1.0 / 24) * zProd;

  zProd *= z2;  // z^5
  res += (7.0 / 960) * zProd;

  zProd *= z2;  // z^7
  res += (127 * zProd) / 80640;

  zProd *= z2;  // z^9
  res += (4369 * zProd) / 11612160;

  zProd *= z2;  // z^11
  res += (34807 * zProd) / 364953600;

  zProd *= z2;  // z^13
  res += (20036983 * zProd) / 797058662400;

  return res;
}

