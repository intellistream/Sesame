//
// Created by 1124a on 2021/8/30.
//
#include <Algorithm/DataStructure/WeightedAdjacencyList.hpp>



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


