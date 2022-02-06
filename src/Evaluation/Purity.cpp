//
// Created by tuidan on 2021/10/11.
//
#include <Utils/UtilityFunctions.hpp>
#include <Evaluation/Purity.hpp>
#include <Utils/Logger.hpp>

/**
 * @Description: Please note that the order of the cluster index has to be consecutive
 */
void SESAME::Purity::pointToGroup(const std::vector<SESAME::PointPtr> &input,
                                  std::vector<std::vector<PointPtr>> &group, int number) {
  int count = 0, k = 1; // TODO: cluster ID starting from 1 ?
  bool stop = false;
  while(!stop) {
    std::vector<SESAME::PointPtr> cluster;
    for(const auto & i : input) {
      if(i->getClusteringCenter() == k) {
        cluster.push_back(i->copy());
        count++;
      }
    }
    if(k == number + 1) stop = true;
    else {
      group.push_back(cluster);
      k++;
    }
  }
}

/**
 * @Description: count the number of the elements in clusterA contained in clusterB?
 */
int SESAME::Purity::calculateBelongsFromTwo(std::vector<SESAME::PointPtr> &groupA,
                                            std::vector<SESAME::PointPtr> &groupB) {
  int count = 0;
  for(const auto& elA:groupA) {
    for(const auto& elB:groupB) {
      if(elA->getIndex() == elB->getIndex()) {
        count++;
        break;
      }
    }
  }
  return count;
}

/**
 * @Description: return the max count of the cluster
 */
double SESAME::Purity::getMaxBelongs(std::vector<SESAME::PointPtr> &singleSample, std::vector<std::vector<PointPtr>> &GT) {
  int max = 0;
  for(auto& re:GT) {
    int temp = calculateBelongsFromTwo(singleSample, re);
    if(max <= temp) max = temp;
  }
  return max;
}

double SESAME::Purity::purityCost(const std::vector<SESAME::PointPtr> &center,
                               const std::vector<SESAME::PointPtr> &result,
                               int dimension) {
  double purity;
  std::vector<PointPtr> input;
  UtilityFunctions::groupByCenters(result, center, input, dimension);
  std::vector<std::vector<PointPtr>> GT;
  std::vector<std::vector<PointPtr>> sample;

  pointToGroup(result, GT, 2);
  pointToGroup(input, sample, (int)center.size());

  double sum = 0;
  for(auto& el: sample) {
    sum += getMaxBelongs(el, GT);
  }
  if(!result.empty()) {
    SESAME_DEBUG("Purity:" << sum / (double)result.size());
    purity=sum / (double)result.size();
  } else{
    SESAME_DEBUG("Purity: 0");
    purity=0;
  }
  return purity;
}