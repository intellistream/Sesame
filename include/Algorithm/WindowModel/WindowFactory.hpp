// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 27/07/2021.
//

#ifndef SESAME_INCLUDE_ALGORITHM_WINDOWMODEL_WINDOWFACTORY_HPP_
#define SESAME_INCLUDE_ALGORITHM_WINDOWMODEL_WINDOWFACTORY_HPP_

#include <memory>
#include <Algorithm/WindowModel/LandmarkWindow.hpp>
namespace SESAME {
class WindowFactory {

 public:
  static std::shared_ptr<LandmarkWindow> createLandmarkWindow();
};
}
#endif //SESAME_INCLUDE_ALGORITHM_WINDOWMODEL_WINDOWFACTORY_HPP_
