//
// Created by Shuhao Zhang on 26/07/2021.
//

#ifndef SESAME_SRC_ALGORITHM_ALGORITHMFACTORY_HPP_
#define SESAME_SRC_ALGORITHM_ALGORITHMFACTORY_HPP_

#include "Algorithm.hpp"
namespace SESAME {
class AlgorithmFactory {

 public:
  static SESAME::AlgorithmPtr create(std::string algoName);
};
}
#endif //SESAME_SRC_ALGORITHM_ALGORITHMFACTORY_HPP_
