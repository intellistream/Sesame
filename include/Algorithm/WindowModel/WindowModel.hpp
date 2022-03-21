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
  int landmark_;

public:
  Landmark(const StreamClusteringParam &param) : landmark_(param.landmark) {}
  bool Add(PointPtr input) { return input->getIndex() >= landmark_; }
  bool Del(PointPtr input) { return false; }
};

class Sliding : WindowModel {
private:
  int slidingCount_;
  int size_;

public:
  Sliding(const StreamClusteringParam &param)
      : slidingCount_(param.slidingCount) {}
  bool Add(const PointPtr input) {
    ++size_;
    return true;
  }
  bool Del() {
    if (size_ > slidingCount_) {
      size_ = slidingCount_;
      return true;
    }
    return false;
  }
};

class Damped : WindowModel {
private:
  double alpha_, lambda_;

public:
  Damped(const StreamClusteringParam &param)
      : alpha_(param.alpha), lambda_(param.lambda) {}
  bool Add(const PointPtr input) { return true; }
  bool Del(const PointPtr input) { return false; }
};

} // namespace SESAME

#endif // SESAME_SRC_ALGORITHM_WINDOWMODEL_WINDOW_HPP_
