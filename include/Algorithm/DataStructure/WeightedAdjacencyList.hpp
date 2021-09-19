//
// Created by 1124a on 2021/8/30.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_WEIGHTEDADJACENCYLIST_HPP_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_WEIGHTEDADJACENCYLIST_HPP_
#include <algorithm>
#include <cmath>
#include <cassert>
#include <functional>
#include <iostream>
#include <unordered_map>
#include<Algorithm/DataStructure/Point.hpp>
#include<Algorithm/DataStructure/MicroCluster.hpp>
#include<Algorithm/WindowModel/DampedWindow.hpp>
namespace SESAME {
struct MicroClusterPair;
typedef std::shared_ptr<MicroClusterPair> MicroClusterPairPtr;
struct MicroClusterPair{
  MicroClusterPtr microCluster1;
  MicroClusterPtr microCluster2;
  MicroClusterPair( MicroClusterPtr microCluster1,MicroClusterPtr microCluster2){
    this->microCluster1=microCluster1->copy();
    this->microCluster2=microCluster2->copy();
  }
 //bool operator==(const MicroClusterPair &other) const;

};

struct KeyHasher{
 std::size_t operator()(const MicroClusterPair &microClusterPair) const
  {
   return (std::hash<int>()(microClusterPair.microCluster1->id.front())) ^ (std::hash<int>()(microClusterPair.microCluster2->id.front()));
  }
};

struct EqualKey
    {
  bool operator() (const MicroClusterPair &MCPair1, const MicroClusterPair &MCPair2) const
  {
     bool equal=false;
    if( MCPair1.microCluster1->id.front()==MCPair2.microCluster1->id.front() &&MCPair1.microCluster2->id.front()==MCPair2.microCluster2->id.front() )
      equal=true;
    if(MCPair1.microCluster1->id.front() ==MCPair2.microCluster2->id.front() &&MCPair1.microCluster2->id.front()==MCPair2.microCluster1->id.front() )
      equal=true;

    return equal;
  }
    };


class AdjustedWeight;
typedef std::shared_ptr<AdjustedWeight> AdjustedWeightPtr;
class AdjustedWeight{
 public:
  double weight;
  clock_t updateTime;
  AdjustedWeight(double weight, clock_t pointTime);
  void add(clock_t startTime,double decayValue);
  double getCurrentWeight(double decayFactor);
};

typedef std::unordered_map<MicroClusterPair,AdjustedWeightPtr,KeyHasher,EqualKey> WeightedAdjacencyList;
typedef std::pair<MicroClusterPair, AdjustedWeightPtr>  DensityGraph;
}
#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_WEIGHTEDADJACENCYLIST_HPP_
