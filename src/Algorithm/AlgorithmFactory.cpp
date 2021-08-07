//
// Created by Shuhao Zhang on 26/07/2021.
//

#include <Algorithm/StreamKM.hpp>
#include <Algorithm/AlgorithmFactory.hpp>

SESAME::AlgorithmPtr SESAME::AlgorithmFactory::create(std::string algoName,
                                                      int pointNumber,
                                                      int clusterNumber,
                                                      int dimension,
                                                      int coresetSize,
                                                      int seed) {
  if (algoName == "StreamKMeans") {
    shared_ptr<StreamKM> streamkm = std::make_shared<StreamKM>();
    streamkm->StreamKMParam.pointNumber = pointNumber;
    streamkm->StreamKMParam.clusterNumber = clusterNumber;
    streamkm->StreamKMParam.windowSize = coresetSize;
    streamkm->StreamKMParam.seed = seed;
    streamkm->StreamKMParam.dimension = dimension;
    return (SESAME::AlgorithmPtr) streamkm;
  }
  throw std::invalid_argument("Unsupported");
}
