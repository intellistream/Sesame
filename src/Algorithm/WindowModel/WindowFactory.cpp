//
// Created by Shuhao Zhang on 27/07/2021.
//

#include <Algorithm/WindowModel/WindowFactory.hpp>
std::shared_ptr<SESAME::LandmarkWindow> SESAME::WindowFactory::createLandmarkWindow() {
  return std::make_shared<LandmarkWindow>();
}
