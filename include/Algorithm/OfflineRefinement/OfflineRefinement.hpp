// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 19/07/2021.
//
#ifndef SESAME_SRC_ALGORITHM_OFFLINE_REFINEMENT_HPP_
#define SESAME_SRC_ALGORITHM_OFFLINE_REFINEMENT_HPP_

#include "Algorithm/DataStructure/Point.hpp"
#include "Algorithm/DesignAspect/Param.hpp"
#include "Sinks/DataSink.hpp"

#include <vector>

namespace SESAME {

class OfflineRefinement {
public:
  void Run(StreamClusteringParam &param, const std::vector<PointPtr> &input,
           DataSinkPtr sinkPtr) {}
};

class NoRefinement : public OfflineRefinement {
public:
  NoRefinement(const StreamClusteringParam &param) {}
  void Run(StreamClusteringParam &param, const std::vector<PointPtr> &input,
           DataSinkPtr sinkPtr) {
    for (auto p : input) {
      sinkPtr->put(p->copy());
    }
  }
};

} // namespace SESAME
#endif // SESAME_SRC_ALGORITHM_OFFLINE_CLUSTERING_HPP_