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

class AlgorithmParameters {
 public:
  int pointNumber;
  int dimension;
};

class Algorithm {
 public:
  Algorithm() = default;
  virtual ~Algorithm() = default;

  virtual void runOnlineClustering(const std::vector<PointPtr> &input) = 0;

  // incremental computation

  // offline pass (optional)
  virtual void runOfflineClustering(const std::vector<PointPtr> &input, std::vector<PointPtr> &output) = 0;
};
}

#endif //SESAME_INCLUDE_ALGORITHM_ALGORITHM_HPP_
