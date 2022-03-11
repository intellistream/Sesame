//
// Created by tuidan on 2021/10/11.
//
#include <Utils/UtilityFunctions.hpp>
#include <Evaluation/Purity.hpp>
#include <Utils/Logger.hpp>
#include <cmath>
/**
 * @Description: Please note that the order of the cluster index has to be consecutive
 */
void SESAME::Purity::pointToGroup(const std::vector<SESAME::PointPtr> &input,
                                  std::vector<std::vector<PointPtr>> &group, int number) {
  int count = 0, k = 0; // TODO: check the output cluster ID starting from 0?
  bool stop = false;
  while(!stop) {
    std::vector<SESAME::PointPtr> cluster;
    for(const auto & i : input) {
      if(i->getClusteringCenter() == k) {
        cluster.push_back(i->copy());
        count++;
      }
    }
    if(k == number) stop = true;
    else {
      group.push_back(cluster);
      k++;
    }
  }
}

/**
 * @Description: count the number of the elements in clusterA contained in clusterB?
 */
double SESAME::Purity::calculateBelongsFromTwo(std::vector<SESAME::PointPtr> &groupA,
                                            std::vector<SESAME::PointPtr> &groupB) {
  double count = 0;
  for(const auto& elA:groupA) {
    for(const auto& elB:groupB) {
      if(elA->getIndex() == elB->getIndex()) {
        count += elA->getWeight();
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
  double max = 0;
  for(auto& re:GT) {
    double temp = calculateBelongsFromTwo(singleSample, re);
    if(max <= temp) max = temp;
  }
  return max;
}

double SESAME::Purity::purityCost(const std::vector<SESAME::PointPtr> &center,
                               const std::vector<SESAME::PointPtr> &result,
                               int dimension, int GTclusterNumber, bool decay) {
  double purity;
  double size = 0;
  std::vector<PointPtr> input;
  UtilityFunctions::groupByCenters(result, center, input, dimension);
  for(int i = 0; i < input.size(); i++){
    double w = 1;
    if(decay){ // 分段函数来设置weight
      if(input.size() - input[i]->getIndex() <= 101) {
        w = 1;
      } else if(input[i]->getIndex() < input.size() / 100) {
        w = 0;
      } else {
        w = double (input[i]->getIndex() - input.size() / 100) / double (input.size() - 100 -input.size() / 100);
      }
    } else {
      w = 1;
    }
    input[i]->setWeight(w);
    size += w;
  }

  std::vector<std::vector<PointPtr>> GT;
  std::vector<std::vector<PointPtr>> sample;

  pointToGroup(result, GT, GTclusterNumber);
  pointToGroup(input, sample, (int)center.size());

  double sum = 0;
  for(auto& el: sample) {
    sum += getMaxBelongs(el, GT);
  }
  if(!result.empty()) {
    SESAME_DEBUG("Purity:" << sum / size);
    purity=sum / size;
  } else{
    SESAME_DEBUG("Purity: 0");
    purity=0;
  }
  return purity;
}