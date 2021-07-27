//
// Created by tuidan on 2021/7/20.
//

#ifndef ONLINEMLBENCHMARK_EVALUATIONMETRICS_HPP_
#define ONLINEMLBENCHMARK_EVALUATIONMETRICS_HPP_
#include <Algorithm/DataStructure/Point.hpp>
#include <vector>
namespace SESAME {
class Evaluation {
 public:
  static void euclideanCost(int numberOfPoints,
                            int numberOfCenters,
                            int dimention,
                            const std::vector<PointPtr> &inputs,
                            const std::vector<PointPtr> &results);//EuclideanCost, defined in StreamKM++: A clustering algorithm for data streams

};

}
#endif //ONLINEMLBENCHMARK_EVALUATIONMETRICS_H
