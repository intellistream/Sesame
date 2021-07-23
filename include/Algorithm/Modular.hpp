//
// Created by Shuhao Zhang on 19/07/2021.
//

#include <Algorithm/DataStructure/Point.hpp>
#include <iostream>
#include <string>
#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <Algorithm/StreamKM.hpp>
using namespace std;
namespace SESAME {

class Modular {
 public:
  static Point *run(Point *input,
                    const string &algoName,
                    int pointNumber,
                    int dimension,
                    int coresetSize,
                    int clusterNumber);

};

}