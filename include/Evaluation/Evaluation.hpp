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

namespace SESAME
{

struct BenchRes
{
    double cmm = 0.0, purity = 0.0, nmi = 0.0;
    int num_res = 0;
    BenchRes() = default;
    void Evaluate(const param_t &param, const std::vector<PointPtr> &inputs,
                  const std::vector<PointPtr> &predicts);
    void Print()
    {
        std::cout << "num_res: " << num_res << std::endl;
        std::cout << "cmm: " << cmm << std::endl;
        std::cout << "purity: " << purity << std::endl;
        std::cout << "nmi: " << nmi << std::endl;
    }
};

}  // namespace SESAME
#endif  // ONLINEMLBENCHMARK_EVALUATIONMETRICS_H
