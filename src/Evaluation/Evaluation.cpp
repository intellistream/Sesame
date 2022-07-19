// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

#include "Evaluation/Evaluation.hpp"
#include "Algorithm/DataStructure/GenericFactory.hpp"
#include "Algorithm/Param.hpp"
#include "Evaluation/CMM.hpp"
#include "Evaluation/Euclidean.hpp"
#include "Evaluation/Purity.hpp"
#include "Timer/Timer.hpp"
#include "Utils/Logger.hpp"
#include "Utils/UtilityFunctions.hpp"

#include <cmath>

namespace SESAME {

BenchmarkResultPtr Evaluation::Evaluate(const param_t &param,
                                        const std::vector<PointPtr> &inputs,
                                        const std::vector<PointPtr> &predicts) {
  double purity = 0.0, cmm = 0.0;
  if(predicts.size() && param.run_pur) {
    purity = Purity::purityCost(inputs, predicts, param.dim, param.time_decay);
  }
  std::cerr << "  Purity: " << round(purity * 10000) / 10000  << std::endl;
  return GenericFactory::New<BenchmarkResult>(cmm, purity);

}
} // namespace SESAME
