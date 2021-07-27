//
// Created by tuidan on 2021/7/22.
//

#ifndef SESAME_INCLUDE_ALGORITHM_ALGORITHM_HPP_
#define SESAME_INCLUDE_ALGORITHM_ALGORITHM_HPP_
#include <Algorithm/DataStructure/Point.hpp>
#include <string>
#include <vector>
#include <memory>
using namespace std;
namespace SESAME {

class Algorithm;
typedef std::shared_ptr<Algorithm> AlgorithmPtr;

class Algorithm {
 public:
  Algorithm() = default;
  virtual ~Algorithm() = default;

  virtual void initialWindow(int pointNumber, int dimension, int coresetSize, int seed) = 0;
  virtual void buildTimeWindow(int pointNumber, const std::vector<PointPtr> &input) = 0;
  virtual void runOfflineClustering(int clusterNumber, int coresetSize, int dimension, vector<PointPtr> &output) = 0;
};
}

#endif //SESAME_INCLUDE_ALGORITHM_ALGORITHM_HPP_
