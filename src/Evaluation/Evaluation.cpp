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
  Timer pur_timer, cmm_timer;
  std::cerr << "Evaluation::Purity begin" << std::endl;
  pur_timer.Tick();
  double purity =
      Purity::purityCost(inputs, predicts, param.dim, param.time_decay);
  pur_timer.Tock();
  std::cerr << "Evaluation::CMM begin" << std::endl;
  cmm_timer.Tick();
  CMM cmm(param);
  double CMM = cmm.Evaluate(inputs, predicts);
  cmm_timer.Tock();
  std::cerr << "Accuracy:" << std::endl
            << "CMM: " << round(CMM * 10000) / 10000 << " et_s: "
            << pur_timer.sum / 1e9 << std::endl
            << "Purity: " << round(purity * 10000) / 10000 << " et_s: "
            << cmm_timer.sum / 1e9 << std::endl;
  return GenericFactory::New<BenchmarkResult>(CMM, purity);
}

} // namespace SESAME
