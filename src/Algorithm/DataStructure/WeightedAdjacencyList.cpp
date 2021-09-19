//
// Created by 1124a on 2021/8/30.
//
#include <Algorithm/DataStructure/WeightedAdjacencyList.hpp>
SESAME::MicroClusterPair::MicroClusterPair( MicroClusterPtr microCluster1,MicroClusterPtr microCluster2){
  this->microCluster1=microCluster1->copy();
  this->microCluster2=microCluster2->copy();
}

bool SESAME::MicroClusterPair::operator==(const MicroClusterPair &other) const{
  bool equal=false;
  if(other.microCluster1==this->microCluster1&&other.microCluster2==this->microCluster2)
    equal=true;
  if(other.microCluster1==this->microCluster2&&other.microCluster2==this->microCluster1)
    equal=true;
  printf("Judging ...",equal);
  return equal;
}
SESAME::AdjustedWeight::AdjustedWeight(double weight, clock_t pointTime){
  this->weight=weight;
  this->updateTime=pointTime;
}
void SESAME::AdjustedWeight::add(clock_t startTime,double decayValue) {
  if ( startTime == this->updateTime) {
    weight++;
  }
  else {
    weight *=   decayValue + 1;
    this->updateTime = clock();
  }
}


double SESAME::AdjustedWeight::getCurrentWeight(double decayFactor){
  return weight * decayFactor;
}


