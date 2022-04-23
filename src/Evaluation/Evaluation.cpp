// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

#include "Evaluation/Evaluation.hpp"
#include "Algorithm/DataStructure/GenericFactory.hpp"
#include "Evaluation/CMM.hpp"
#include "Evaluation/Euclidean.hpp"
#include "Evaluation/Purity.hpp"
#include "Utils/Logger.hpp"

#include <cmath>

namespace SESAME {

BenchmarkResultPtr Evaluation::runEvaluation(
    int dim, int num_clusters, bool decay,
    const std::vector<PointPtr> &inputs, const std::vector<PointPtr> &center) {
  std::cerr << "Evaluation::Purity begin" << std::endl;
  double purity = Purity::purityCost(center, inputs, dim, decay);

  std::cerr << "Evaluation::CMM begin" << std::endl;
  double CMM = CMM::CMMCost(dim, inputs, center);
  std::cerr << "Accuracy:" << std::endl
            << "AvgCMM: " << round(CMM * 10000) / 10000 << std::endl
            << "Purity: " << round(purity * 10000) / 10000 << std::endl;
  return GenericFactory::New<BenchmarkResult>(CMM, purity);
}

} // namespace SESAME
