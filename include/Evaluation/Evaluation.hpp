// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by tuidan on 2021/7/20.
//

#ifndef ONLINEMLBENCHMARK_EVALUATIONMETRICS_HPP_
#define ONLINEMLBENCHMARK_EVALUATIONMETRICS_HPP_

#include "Algorithm/DataStructure/Point.hpp"
#include "Algorithm/Param.hpp"

#include <vector>

namespace SESAME {

struct BenchmarkResult {
  double cmm = 0.0, purity = 0.0, nmi = 0.0;
  int num_res = 0;
  BenchmarkResult(double cmm = 0, double purity = 0, double nmi = 0)
      : cmm(cmm), purity(purity), nmi(nmi) {}
  void Print() {
    std::cout << "num_res: " << num_res << std::endl;
    std::cout << "cmm: " << cmm << std::endl;
    std::cout << "purity: " << purity << std::endl;
    std::cout << "nmi: " << nmi << std::endl;
  }
};
using BenchmarkResultPtr = std::shared_ptr<BenchmarkResult>;

// enum evaluateType { euclideanCostType, cmmType, purityType };

class Evaluation {
public:
  static BenchmarkResultPtr Evaluate(const param_t &param,
                                     const std::vector<PointPtr> &inputs,
                                     const std::vector<PointPtr> &results);
};

} // namespace SESAME
#endif // ONLINEMLBENCHMARK_EVALUATIONMETRICS_H
