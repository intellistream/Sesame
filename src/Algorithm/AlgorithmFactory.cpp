//
// Created by Shuhao Zhang on 26/07/2021.
//

#include <Algorithm/StreamKM.hpp>
#include <Algorithm/AlgorithmFactory.hpp>

SESAME::AlgorithmPtr SESAME::AlgorithmFactory::create(std::string algoName) {
  if (algoName == "StreamKMeans") {
    return std::make_shared<StreamKM>();
  }
  throw std::invalid_argument("Unsupported");
}
