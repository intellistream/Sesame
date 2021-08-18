//
// Created by Shuhao Zhang on 26/07/2021.
//

#include <Algorithm/StreamKM.hpp>
#include <Algorithm/CluStream.hpp>
#include <Algorithm/AlgorithmFactory.hpp>

SESAME::AlgorithmPtr SESAME::AlgorithmFactory::create(std::string algoName,
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
                                                      int offlineTimeWindow) {
  if (algoName == "StreamKMeans") {
    shared_ptr<StreamKM> streamkm = std::make_shared<StreamKM>();
    streamkm->StreamKMParam.pointNumber = pointNumber;
    streamkm->StreamKMParam.clusterNumber = clusterNumber;
    streamkm->StreamKMParam.windowSize = coresetSize;
    streamkm->StreamKMParam.seed = seed;
    streamkm->StreamKMParam.dimension = dimension;
    return (SESAME::AlgorithmPtr) streamkm;
  }
  if (algoName == "CluStream") {
    shared_ptr<CluStream> cluStream = std::make_shared<CluStream>();
    cluStream->CluStreamParam.pointNumber = pointNumber;
    cluStream->CluStreamParam.clusterNumber = onlineClusterNumber;
    cluStream->CluStreamParam.dimension = dimension;
    cluStream->CluStreamParam.lastArrivingNum = lastArrivingNum;
    cluStream->CluStreamParam.timeWindow = timeWindow;
    cluStream->CluStreamParam.timeInterval = timeInterval;
    cluStream->CluStreamParam.offlineClusterNumber = clusterNumber;
    cluStream->CluStreamParam.radiusFactor = radiusFactor;
    cluStream->CluStreamParam.initBuffer = initBuffer;
    cluStream->CluStreamParam.offlineTimeWindow = offlineTimeWindow;

    return (SESAME::AlgorithmPtr) cluStream;
  }
  throw std::invalid_argument("Unsupported");
}
