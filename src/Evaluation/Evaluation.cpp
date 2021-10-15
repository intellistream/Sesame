// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

#include <Evaluation/Evaluation.hpp>
#include <Evaluation/Euclidean.hpp>
#include <Evaluation/Purity.hpp>
#include <Evaluation/CMM.hpp>
#include <cmath>
#include <Utils/Logger.hpp>

void SESAME::Evaluation::runEvaluation(int numberOfPoints,
                                       int numberOfCenters,
                                       int dimension,
                                       const std::vector<PointPtr> &inputs,
                                       const std::vector<PointPtr> &center) {
  SESAME::Euclidean::euclideanCost(numberOfPoints,
                                   numberOfCenters,
                                   dimension,
                                   inputs,
                                   center);
  SESAME::Purity::purityCost(center, inputs, dimension);

  SESAME::CMM::CMMCost(dimension,inputs,center);
}
