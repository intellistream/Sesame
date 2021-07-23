//
// Created by tuidan on 2021/7/22.
//

#ifndef SESAME_INCLUDE_ALGORITHM_ALGORITHM_HPP_
#define SESAME_INCLUDE_ALGORITHM_ALGORITHM_HPP_
#include <Algorithm/DataStructure/Point.hpp>

namespace SESAME {
template<class T>
class Algorithm {
 public:
  Algorithm() = default;
  virtual ~Algorithm() = default;
  virtual void runOfflineClustering(int clusterNumber, int coresetSize, int dimension,
                                    Point *streamingCoreset, Point *centresStreamingCoreset) = 0;
  virtual void initialWindow(T *manager, int pointNumber, int dimension, int coresetSize, int seed) = 0;
  virtual void buildTimeWindow(int pointNumber, Point *p, Point *streamingCoreset, T *manager) = 0;
};
}

#endif //SESAME_INCLUDE_ALGORITHM_ALGORITHM_HPP_
