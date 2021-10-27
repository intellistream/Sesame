// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by tuidan on 2021/7/20.
//

#ifndef ONLINEMLBENCHMARK_EVALUATIONMETRICS_HPP_
#define ONLINEMLBENCHMARK_EVALUATIONMETRICS_HPP_
#include <Algorithm/DataStructure/Point.hpp>
#include <vector>
namespace SESAME {

// enum evaluateType { euclideanCostType, cmmType, purityType };

class Evaluation {
 public:
  static void runEvaluation(int numberOfPoints,
                            int numberOfCenters,
                            int dimension,
                            const std::vector<PointPtr> &inputs,
                            const std::vector<PointPtr> &results);


};

}
#endif //ONLINEMLBENCHMARK_EVALUATIONMETRICS_H
