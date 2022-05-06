// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

#include "Evaluation/Evaluation.hpp"
#include "Algorithm/DataStructure/GenericFactory.hpp"
#include "Algorithm/Param.hpp"
#include "Evaluation/CMM.hpp"
#include "Evaluation/Euclidean.hpp"
#include "Evaluation/Purity.hpp"
#include "Utils/Logger.hpp"
#include "Utils/UtilityFunctions.hpp"

#include <cmath>

namespace SESAME {

BenchmarkResultPtr Evaluation::Evaluate(const param_t &param,
                                        const std::vector<PointPtr> &inputs,
                                        const std::vector<PointPtr> &predicts) {
  std::cerr << "Evaluation::Purity begin" << std::endl;
  double purity =
      Purity::purityCost(inputs, predicts, param.dim, param.time_decay);

  std::cerr << "Evaluation::CMM begin" << std::endl;
  CMM cmm(param);
  double CMM = cmm.Evaluate(inputs, predicts);
  std::cerr << "Accuracy:" << std::endl
            << "AvgCMM: " << round(CMM * 10000) / 10000 << std::endl
            << "Purity: " << round(purity * 10000) / 10000 << std::endl;
  return GenericFactory::New<BenchmarkResult>(CMM, purity);
}

} // namespace SESAME
