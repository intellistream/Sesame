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
class MicroClusterPair;
typedef std::shared_ptr<MicroClusterPair> MicroClusterPairPtr;
class MicroClusterPair{
 public:
  MicroClusterPtr microCluster1;
  MicroClusterPtr microCluster2;
  MicroClusterPair( MicroClusterPtr microCluster1,MicroClusterPtr microCluster2);
  bool operator==(const MicroClusterPair &other) const;
};

struct hashMicroClusterPair{
 size_t operator()(const MicroClusterPair &microClusterPair) const
  {
    return (std::hash<MicroClusterPtr>()(microClusterPair.microCluster1)) + (std::hash<MicroClusterPtr>()(microClusterPair.microCluster2));
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

typedef std::unordered_map<MicroClusterPair,AdjustedWeightPtr,hashMicroClusterPair> WeightedAdjacencyList;
typedef std::pair<MicroClusterPair, AdjustedWeightPtr>  DensityGraph;
//S in paper, represent Weighted Adjacency List

}


#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_WEIGHTEDADJACENCYLIST_HPP_
