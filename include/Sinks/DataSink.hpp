//
// Created by Shuhao Zhang on 20/07/2021.
//

#ifndef SESAME_INCLUDE_SINKS_DATASINK_HPP_
#define SESAME_INCLUDE_SINKS_DATASINK_HPP_
#include <string>
#include <Algorithm/DataStructure/Point.hpp>
#include <vector>
namespace SESAME {

class DataSink {

 public:
  static void store(std::string outputPath, int numberOfCenters, int dimension, std::vector<Point> &centers);
};
}
#endif //SESAME_INCLUDE_SINKS_DATASINK_HPP_
