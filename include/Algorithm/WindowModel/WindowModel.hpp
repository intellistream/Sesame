// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 26/07/2021.
//

#ifndef SESAME_SRC_ALGORITHM_WINDOWMODEL_WINDOW_HPP_
#define SESAME_SRC_ALGORITHM_WINDOWMODEL_WINDOW_HPP_

#include "Algorithm/DataStructure/Point.hpp"
#include "Algorithm/DesignAspect/Param.hpp"

#include <memory>

namespace SESAME {

class WindowModel;
typedef std::shared_ptr<WindowModel> WindowPtr;

class WindowModel {};

class Landmark : WindowModel {
private:
  int landmark;

public:
  Landmark(const StreamClusteringParam &param)
      : landmark(param.landmark) {}
  bool addPoint(const PointPtr input) { return input->getIndex() >= landmark; }
};

class Sliding : WindowModel {
private:
  int slidingCount;
  int size;

public:
  Sliding(const StreamClusteringParam &param)
      : slidingCount(param.slidingCount) {}
  bool addPoint(const PointPtr input) {
    ++size;
    return true;
  }
  bool delPoint() {
    if (size > slidingCount) {
      size = slidingCount;
      return true;
    }
    return false;
  }
};

class Damped : WindowModel {
private:
  double alpha, lambda;

public:
  Damped(const StreamClusteringParam &param)
      : alpha(param.alpha), lambda(param.lambda) {}
  bool addPoint(const PointPtr input) { return true; }
};

} // namespace SESAME

#endif // SESAME_SRC_ALGORITHM_WINDOWMODEL_WINDOW_HPP_
