// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 26/07/2021.
//

#include "Algorithm/AlgorithmFactory.hpp"
#include "Algorithm/Birch.hpp"
#include "Algorithm/CluStream.hpp"
#include "Algorithm/DBStream.hpp"
#include "Algorithm/DStream.hpp"
#include "Algorithm/DenStream.hpp"
#include "Algorithm/DesignAspect/Generic.hpp"
#include "Algorithm/DesignAspect/V1.hpp"
#include "Algorithm/DesignAspect/V2.hpp"
#include "Algorithm/DesignAspect/V3.hpp"
#include "Algorithm/DesignAspect/V4.hpp"
#include "Algorithm/DesignAspect/V5.hpp"
#include "Algorithm/DesignAspect/V8.hpp"
#include "Algorithm/EDMStream.hpp"
#include "Algorithm/OutlierDetection/OutlierDetection.hpp"
#include "Algorithm/StreamKM.hpp"

namespace SESAME {

AlgorithmPtr AlgorithmFactory::create(param_t &cmd_params) {
  if (cmd_params.algoType == StreamKMeansType) {
    shared_ptr<StreamKM> streamkm = std::make_shared<StreamKM>(cmd_params);
    return (AlgorithmPtr)streamkm;
  }
  if (cmd_params.algoType == CluStreamType) {
    shared_ptr<CluStream> cluStream = std::make_shared<CluStream>(cmd_params);
    return (AlgorithmPtr)cluStream;
  }
  if (cmd_params.algoType == BirchType) {
    shared_ptr<Birch> birch = std::make_shared<Birch>(cmd_params);
    return (AlgorithmPtr)birch;
  }
  if (cmd_params.algoType == DenStreamType) {
    shared_ptr<DenStream> denStream = std::make_shared<DenStream>(cmd_params);
    return (AlgorithmPtr)denStream;
  }
  if (cmd_params.algoType == EDMStreamType) {
    shared_ptr<EDMStream> eDMStream = std::make_shared<EDMStream>(cmd_params);
    return (AlgorithmPtr)eDMStream;
  }
  if (cmd_params.algoType == DBStreamType) {
    shared_ptr<DBStream> dbStream = std::make_shared<DBStream>(cmd_params);
    return (AlgorithmPtr)dbStream;
  }
  if (cmd_params.algoType == DStreamType) {
    shared_ptr<DStream> dStream = std::make_shared<DStream>(cmd_params);
    return (AlgorithmPtr)dStream;
  }
  if (cmd_params.algoType == V1Stream) {
    shared_ptr<V1> v1stream = std::make_shared<V1>(cmd_params);
    return (AlgorithmPtr)v1stream;
  }
  if (cmd_params.algoType == V2Stream) {
    shared_ptr<V2> V2Stream = std::make_shared<V2>(cmd_params);
    return (AlgorithmPtr)V2Stream;
  }
  if (cmd_params.algoType == V3Stream) {
    shared_ptr<V3> V3Stream = std::make_shared<V3>(cmd_params);
    return (AlgorithmPtr)V3Stream;
  }
  if (cmd_params.algoType == V4Stream) {
    shared_ptr<V4> V4Stream = std::make_shared<V4>(cmd_params);
    return (AlgorithmPtr)V4Stream;
  }
  if (cmd_params.algoType == V5Stream) {
    shared_ptr<V5> V5Stream = std::make_shared<V5>(cmd_params);
    return (AlgorithmPtr)V5Stream;
  }
  if (cmd_params.algoType == V6Stream) {
    shared_ptr<DStream> V6Stream = std::make_shared<V6>(cmd_params);
    return (V6::AlgorithmPtr) V6Stream;
  }
  if (cmd_params.algoType == V7Stream) {
    shared_ptr<V7> V7Stream = std::make_shared<V7>(cmd_params);
    return (AlgorithmPtr)V7Stream;
  }
  if (cmd_params.algoType == V8Stream) {
    shared_ptr<V8> V8Stream = std::make_shared<V8>(cmd_params);
    return (AlgorithmPtr)V8Stream;
  }
  if (cmd_params.algoType == V9Stream) {
    shared_ptr<DStream> dStream = std::make_shared<DStream>(cmd_params);
    return (AlgorithmPtr)dStream;
  }
  if (cmd_params.algoType == Generic) {
    using W = Landmark;
    using D = ClusteringFeaturesTree;
    using O = Distance;
    shared_ptr<StreamClustering<W, D, O>> generic =
        std::make_shared<StreamClustering<W, D, O>>(cmd_params);
    return (AlgorithmPtr)generic;
  }
  throw std::invalid_argument("Unsupported");
}
} // namespace SESAME
