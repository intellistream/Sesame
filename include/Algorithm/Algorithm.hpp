//
// Created by Shuhao Zhang on 19/07/2021.
//

#include <Algorithm/DataStructure/Point.hpp>
#include <string>
#include <Algorithm/WindowModel/LandmarkWindow.hpp>
using namespace std;
namespace SESAME {

class Algorithm {
 public:
  static Point * create(Point *input,
                        const string& algoName,
                        int pointNumber,
                        int dimension,
                        int coresetSize,
                        int clusterNumber);
  static void initManager(SESAME::LandmarkWindow::Bucketmanager *manager, int number, int dimension, int size);
};

}