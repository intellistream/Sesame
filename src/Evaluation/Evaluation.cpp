// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

#include <Evaluation/Evaluation.hpp>
#include <Evaluation/Euclidean.hpp>
#include <Evaluation/Purity.hpp>
#include <cmath>
#include <Utils/Logger.hpp>

void SESAME::Evaluation::runEvaluation(int numberOfPoints,
                                       int numberOfCenters,
                                       int dimension,
                                       const std::vector<PointPtr> &results,
                                       const std::vector<PointPtr> &center) {
  SESAME::Euclidean::euclideanCost(numberOfPoints,
                                   numberOfCenters,
                                   dimension,
                                   results,
                                   center);
  SESAME::Purity::purityCost(center, results, dimension);
}
