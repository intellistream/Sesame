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
    shared_ptr<StreamKM> streamkm = std::make_shared<StreamKM>(cmd_params);;
    return (SESAME::AlgorithmPtr) streamkm;
  }
  if (cmd_params.algoName == "CluStream") {
    shared_ptr<CluStream> cluStream = std::make_shared<CluStream>(cmd_params);
    return (SESAME::AlgorithmPtr) cluStream;
  }
  if (cmd_params.algoName == "Birch") {
    shared_ptr<Birch> birch = std::make_shared<Birch>(cmd_params);
    return (SESAME::AlgorithmPtr) birch;
  }
  throw std::invalid_argument("Unsupported");
}
