//
// Created by Shuhao Zhang on 19/07/2021.
//

#ifndef SESAME_INCLUDE_Engine_SINGLETHREADENGINE_H_
#define SESAME_INCLUDE_Engine_SINGLETHREADENGINE_H_
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <Engine/Engine.hpp>
#include <Algorithm/DataStructure/Point.hpp>
#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <Algorithm/StreamKM.hpp>
using namespace std;

namespace SESAME {
class SingleThreadEngine : SESAME::Engine {
 public:
  SingleThreadEngine();
  Point *runAlgorithm(Point *input,
                      const string &algoName,
                      int pointNumber,
                      int dimension,
                      int coresetSize,
                      int clusterNumber);

};
}
#endif //SESAME_INCLUDE_Engine_SINGLETHREADENGINE_H_
