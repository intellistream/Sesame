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
  double purity = 0.0, cmm = 0.0;
  pur_timer.Tick();
  if(predicts.size() && param.run_pur) {
    std::cerr << "Evaluation::Purity begin" << std::endl;
    purity = Purity::purityCost(inputs, predicts, param.dim, param.time_decay);
  }
  pur_timer.Tock();
  cmm_timer.Tick();
  if(predicts.size() && param.run_cmm) {
    std::cerr << "Evaluation::CMM begin" << std::endl;
    CMM eval(param);
    cmm = eval.Evaluate(inputs, predicts);
  }
  cmm_timer.Tock();
  std::cerr << "Accuracy:" << std::endl
            << "CMM: " << round(cmm * 10000) / 10000 << " et_s: "
            << cmm_timer.sum / 1e9 << std::endl
            << "Purity: " << round(purity * 10000) / 10000 << " et_s: "
            << pur_timer.sum / 1e9 << std::endl;
  return GenericFactory::New<BenchmarkResult>(cmm, purity);
}

} // namespace SESAME
