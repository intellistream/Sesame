// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by 1124a on 2021/8/16.
//
#include <Algorithm/DataStructure/MicroCluster.hpp>
#include <Utils/Logger.hpp>
#include <iterator>

SESAME::MicroCluster::MicroCluster(int dimension, int id)
{
  this->dimension=dimension;
  weight=0;
  this->id.push_back(id);
  LST=0;
  SST=0;
  this->visited=false;
  this->createTime=clock();
  this->lastUpdateTime=this->createTime;
  radius=0;
}
SESAME::MicroCluster::MicroCluster(int dimension, int id,PointPtr dataPoint,double radius){
  this->dimension=dimension;
  weight=1;
  this->id.push_back(id);
  LST=0;
  SST=0;
  this->visited=false;
  this->createTime=clock();
  this->lastUpdateTime=this->createTime;
  this->radius=radius;
  for (int i = 0; i < dimension; i++) {
    double data = dataPoint->getFeatureItem(i);
    LS.push_back(data);
    centroid.push_back(data);
  }
}

//Release memory of the current micro cluster
SESAME::MicroCluster::~MicroCluster()
{
  std::vector <int>().swap(id);
  std::vector <double>().swap(centroid);
  std::vector <double>().swap(LS);
  std::vector <double>().swap(SS);
}
void SESAME::MicroCluster::init(PointPtr datapoint,int timestamp)
{
  weight++;
  for (int i = 0; i < dimension; i++) {
    double data = datapoint->getFeatureItem(i);
    LS.push_back(data);
    SS.push_back(data * data);
    centroid.push_back(data);
  }
  LST+=timestamp;
  SST+=timestamp*timestamp;
}


//insert a new data point from input data stream
void SESAME::MicroCluster::insert(PointPtr datapoint,int timestamp)
{
  weight++;
  for(int i=0; i<LS.size(); i++) {
    double data=datapoint->getFeatureItem(i);
    LS[i]+=data;
    SS[i]+=data*data;
  }

  LST+=timestamp;
  SST+=timestamp*timestamp;
  centroid=std::move(getCentroid());
}
void SESAME::MicroCluster::insert(PointPtr datapoint)
{
  weight++;
  double val = exp(-(pow(3 * this->distance / radius, 2) / 2));
  for(int i=0; i<LS.size(); i++) {
    double data=datapoint->getFeatureItem(i);
    LS[i] = centroid.at(i) + val * (data - centroid.at(i));
  }
}
double SESAME::MicroCluster::getDistance(PointPtr datapoint){
  this->distance=calCentroidDistance(datapoint);
  return this->distance;
}
double SESAME::MicroCluster::getDistance(MicroClusterPtr other){
  double temp = 0, dist = 0;
  for (int i = 0; i < this->dimension; i++) {
    temp = this->centroid[i] - other->centroid[i];
    dist += temp * temp;
  }
  return sqrt(dist);
}

bool SESAME::MicroCluster::insert(PointPtr datapoint,double decayFactor,double epsilon){
  bool result;
  dataPoint LSPre; LSPre.assign(this->LS.begin(),this->LS.end());
  dataPoint SSPre; SSPre.assign(this->SS.begin(),this->SS.end());
  for (int i = 0; i < this->dimension; i++) {
    double data=datapoint->getFeatureItem(i);
    LSPre[i] *= decayFactor;
    LSPre[i] += data;
    SSPre[i] *= decayFactor;
    SSPre[i] += data * data;
  }
  if (getRadius() < epsilon)
  {
    LS = LSPre;
    SS = SSPre;
    weight *= decayFactor;
    weight++;
    for (int i = 0; i < this->dimension; i++) {
      centroid.at(i) = LS.at(i) / weight;
    }
    this->lastUpdateTime=clock();
    result=true;
  }
  else
    result=false;
  return result;
}
//merge two micro-clusters
void SESAME::MicroCluster::merge(MicroClusterPtr other){
  weight+=other->weight;
  for(int i=0; i<dimension; i++)// dimension can change to CF1x.size()
  {
    LS[i]+=other->LS[i];
    SS[i]+=other->SS[i];
  }
  LST+=other->LST;
  SST+=other->SST;
  updateId(other);
  centroid=std::move(getCentroid());
}

//Calculate the process of micro cluster N(Tc-h')
void SESAME::MicroCluster::subtractClusterVector(MicroClusterPtr other)
{
  this->weight-=other->weight;
  for(int i=0; i<dimension; i++) {
    this->LS[i]-=other->LS[i];
    this->SS[i]-=other->SS[i];
  }
  this->LST-= other->LST;
  this->SST-= other->SST;
  centroid=std::move(getCentroid());
}
bool SESAME::MicroCluster::judgeMerge(MicroClusterPtr other)
{
  bool merge=true;
  for(unsigned int i=0; i<other->id.size(); i++) {
    if (std::find(this->id.begin(), this->id.end(), other->id[i])==id.end())
      merge=false;
  }
  return merge;
}

//update id list of Micro cluster
void SESAME::MicroCluster::updateId(MicroClusterPtr other)
{
  for(unsigned int i=0; i<other->id.size(); i++) {
    this->id.push_back(other->id[i]);
  }
  std::vector <int>().swap(other->id);
  this->id.reserve(20);
}

//reset the unique id in MICRO CLUSTER OF DENSTREAM
void SESAME::MicroCluster::resetID(int index){
  this->id.pop_back();
  this->id.push_back(index);
}

//obtain relevance stamp of a cluster to judge whether it needs to be deleted
double SESAME::MicroCluster::getRelevanceStamp(int lastArrivingNum) const
{
  if(weight<(2*lastArrivingNum))
    return getMutime();
  return getMutime() + getSigmaTime() * getQuantile( ((double)lastArrivingNum)/(2*weight) );
}

//mean_timestamp

double SESAME::MicroCluster::getMutime() const{
  return LST/weight;
}

//standard_deviation_timestamp

double SESAME::MicroCluster::getSigmaTime() const
{
  return sqrt(SST/weight - (LST/weight)*(LST/weight));
}

double SESAME::MicroCluster::getQuantile(double z)
{
  assert(z >= 0 && z <= 1 );
  return sqrt( 2 ) * inverseError( 2*z - 1 );
}

double SESAME::MicroCluster::getRadius(double radiusFactor){
  if(weight==1)
    return 0;
  if(radiusFactor<=0)
    radiusFactor=1.8;

  return getDeviation()*radiusFactor;
}

//Calculate the radius of the current micro cluster in DenStream
double SESAME::MicroCluster::getRadius(){
  double radius = 0;
  for (int i = 0; i < this->dimension; i++) {
    radius += (SS.at(i) - (pow(LS.at(i), 2) / weight));
  }
  return sqrt(radius / weight);
}

//calculate RMS deviation very confused

double SESAME::MicroCluster::getDeviation(){
  SESAME::dataPoint variance=getVarianceVector();
  double sumOfDeviation=0;
  for(int i=0; i<dimension; i++){
    sumOfDeviation += sqrt(variance[i]);
  }

  return  sumOfDeviation/dimension;
}

//calculate centroid of a  cluster
SESAME::dataPoint SESAME::MicroCluster::getCentroid(){
  if(weight==1)
    return LS;
  dataPoint dataObject(LS.size());//double
  if(weight>1){
    for(int i=0; i<centroid.size(); i++){
      dataObject[i]=(LS[i]/weight);
    }
  }
  return dataObject;
}

double SESAME::MicroCluster::getInclusionProbability(PointPtr datapoint,double radiusFactor) {
  if(weight==1) {
    double distance=0;
    for(int i=0; i<dimension; i++){
      double d=LS[i]-datapoint->getFeatureItem(i);
      distance+=d*d;
    }
    distance=sqrt(distance);
    if(distance<EPSILON)
      return 1;
    return 0;
  } else{
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

  for(int i=0; i<dimension; i++){
    double linearSum= LS[i];
    double squaredSum= SS[i];
    double aveLinearSum= linearSum/weight;
    double squaredAveLinearSum=	aveLinearSum*aveLinearSum;
    double squaredSumSquared= squaredSum/weight;
    datapoint.push_back(squaredSumSquared-squaredAveLinearSum);
    if(datapoint[i]<=0.0)
      datapoint[i]=MIN_VARIANCE;
  }
  return datapoint;
}


// calculate the distance between input data point and centroid of micro-clusters

double SESAME::MicroCluster::calCentroidDistance(PointPtr datapoint){

  double temp=0;
  for(int i=0; i<dimension; i++){
    double diff=centroid[i]-datapoint->getFeatureItem(i);
    temp+=(diff*diff);
  }
  return sqrt(temp);
}

//Still need to modify
void SESAME::MicroCluster::move(){
  this->centroid=this->LS;
}

void SESAME::MicroCluster::decayWeight(double decayFactor){
  this->weight *=decayFactor;
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

SESAME::MicroClusterPtr SESAME::MicroCluster::copy() {
  return std::make_shared<MicroCluster>(*this);
}