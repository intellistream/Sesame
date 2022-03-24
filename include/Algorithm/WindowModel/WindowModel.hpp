// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 26/07/2021.
//

#ifndef SESAME_SRC_ALGORITHM_WINDOWMODEL_WINDOW_HPP_
#define SESAME_SRC_ALGORITHM_WINDOWMODEL_WINDOW_HPP_

#include "Algorithm/DataStructure/GenericFactory.hpp"
#include "Algorithm/DataStructure/Point.hpp"
#include "Algorithm/DesignAspect/Param.hpp"

#include <cmath>
#include <queue>

namespace SESAME {

class WindowModel;
typedef std::shared_ptr<WindowModel> WindowPtr;

class WindowModel {};

class Landmark : WindowModel {
private:
  int landmark_;

public:
  Landmark(const StreamClusteringParam &param) : landmark_(param.landmark) {}
  bool Add(PointPtr input) { return input->getIndex() >= landmark_; }
};

class Sliding : WindowModel {
private:
  int sliding_;
  std::queue<PointPtr> queue_;

public:
  Sliding(const StreamClusteringParam &param) : sliding_(param.sliding) {}
  bool Add(const PointPtr input) {
    queue_.push(input);
    return true;
  }
  PointPtr Delete() {
    if (queue_.size() > sliding_) {
      auto ret = queue_.front();
      queue_.pop();
      return ret;
    }
    return nullptr;
  }
};

class Damped : WindowModel {
private:
  double alpha_, lambda_;

public:
  Damped(const StreamClusteringParam &param)
      : alpha_(param.alpha), lambda_(param.lambda) {}
  bool Add(const PointPtr input) { return true; }
  template <NodeConcept T> void Update(T node) {
    node->Scale(pow(alpha_, -lambda_));
  }
};

} // namespace SESAME

#endif // SESAME_SRC_ALGORITHM_WINDOWMODEL_WINDOW_HPP_
