//
// Created by Shuhao Zhang on 1/7/21.
//

#ifndef ONLINEMLBENCHMARK_SRC_ALGORITHM_MODULAR_HPP_
#define ONLINEMLBENCHMARK_SRC_ALGORITHM_MODULAR_HPP_

#include "OfflineClustering/DbScan.hpp"
#include "WindowModel/WindowModel.hpp"
/**
 * @brief This is where algorithm is constructed by various components.
 */

enum AlgorithmName { KMeans, clustream };

class Algorithm {

  // define the input

  // define the logic

  // define the output

public:
  Algorithm(AlgorithmName algo);
};

#endif // ONLINEMLBENCHMARK_SRC_ALGORITHM_MODULAR_HPP_
