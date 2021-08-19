// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 26/07/2021.
//

#ifndef SESAME_SRC_ALGORITHM_ALGORITHMFACTORY_HPP_
#define SESAME_SRC_ALGORITHM_ALGORITHMFACTORY_HPP_

#include <Algorithm/Algorithm.hpp>
namespace SESAME {
class AlgorithmFactory {

 public:
  static SESAME::AlgorithmPtr create(std::string algoName,
                                     int pointNumber,
                                     int clusterNumber,
                                     int dimension,
                                     int coresetSize,
                                     int seed,
                                     int lastArrivingNum,
                                     int timeWindow,
                                     unsigned int timeInterval,
                                     int onlineClusterNumber,
                                     double radiusFactor,
                                     int initBuffer,
                                     int offlineTimeWindow);
};
}
#endif //SESAME_SRC_ALGORITHM_ALGORITHMFACTORY_HPP_
