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
#include <vector>
using namespace std;

namespace SESAME {
class SingleThreadEngine : SESAME::Engine {
 public:
  SingleThreadEngine();
  void runAlgorithm(vector<Point> &input,
                    vector<Point> &output,
                    AlgorithmPtr algo,
                    int pointNumber,
                    int dimension,
                    int coresetSize,
                    int clusterNumber);

};
}
#endif //SESAME_INCLUDE_Engine_SINGLETHREADENGINE_H_
