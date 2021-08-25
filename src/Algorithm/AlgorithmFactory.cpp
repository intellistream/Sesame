// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 26/07/2021.
//

#include <Algorithm/StreamKM.hpp>
#include <Algorithm/CluStream.hpp>
#include <Algorithm/Birch.hpp>
#include <Algorithm/AlgorithmFactory.hpp>

SESAME::AlgorithmPtr SESAME::AlgorithmFactory::create(param_t &cmd_params) {
  if (cmd_params.algoName == "StreamKMeans") {
    shared_ptr<StreamKM> streamkm = std::make_shared<StreamKM>();
    streamkm->StreamKMParam.pointNumber = cmd_params.pointNumber;
    streamkm->StreamKMParam.clusterNumber = cmd_params.clusterNumber;
    streamkm->StreamKMParam.windowSize = cmd_params.coresetSize;
    streamkm->StreamKMParam.seed = cmd_params.seed;
    streamkm->StreamKMParam.dimension = cmd_params.dimension;
    return (SESAME::AlgorithmPtr) streamkm;
  }
  if (cmd_params.algoName == "CluStream") {
    shared_ptr<CluStream> cluStream = std::make_shared<CluStream>();
    cluStream->CluStreamParam.pointNumber = cmd_params.pointNumber;
    cluStream->CluStreamParam.clusterNumber = cmd_params.onlineClusterNumber;
    cluStream->CluStreamParam.dimension = cmd_params.dimension;
    cluStream->CluStreamParam.lastArrivingNum = cmd_params.lastArrivingNum;
    cluStream->CluStreamParam.timeWindow = cmd_params.timeWindow;
    cluStream->CluStreamParam.timeInterval = cmd_params.timeInterval;
    cluStream->CluStreamParam.offlineClusterNumber = cmd_params.clusterNumber;
    cluStream->CluStreamParam.radiusFactor = cmd_params.radiusFactor;
    cluStream->CluStreamParam.initBuffer = cmd_params.initBuffer;
    cluStream->CluStreamParam.offlineTimeWindow = cmd_params.offlineTimeWindow;

    return (SESAME::AlgorithmPtr) cluStream;
  }
  if (cmd_params.algoName == "Birch") {
    shared_ptr<Birch> birch = std::make_shared<Birch>();
    birch->BirchParam.pointNumber = cmd_params.pointNumber;
    birch->BirchParam.dimension = cmd_params.dimension;
    birch->BirchParam.maxInternalNodes = cmd_params.maxInternalNodes;
    birch->BirchParam.maxLeafNodes = cmd_params.maxLeafNodes;
    birch->BirchParam.thresholdDistance = cmd_params.thresholdDistance;
    return (SESAME::AlgorithmPtr) birch;
  }
  throw std::invalid_argument("Unsupported");
}
