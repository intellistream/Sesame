// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

#include <Evaluation/Evaluation.hpp>
#include <Evaluation/Euclidean.hpp>
#include <Evaluation/Purity.hpp>
#include <Evaluation/CMM.hpp>
#include <cmath>
#include <Utils/Logger.hpp>

void SESAME::Evaluation::runEvaluation(//int numberOfPoints,
                                       //int numberOfCenters,
                                       int dimension,
                                       int GTClusterNumber,bool decay,
                                       const std::vector<PointPtr> &inputs,
                                       const std::vector<PointPtr> &center) {
 /*SESAME::Euclidean::euclideanCost(numberOfPoints,
                                   numberOfCenters,
                                   dimension,
                                   inputs,
                                   center);*/
 double purity =  SESAME::Purity::purityCost(center, inputs, dimension, GTClusterNumber, decay);

 double CMM = SESAME::CMM::CMMCost(dimension,inputs,center);
 std::cout << "Accuracy:\n"
 <<"AveCMM: " << round(CMM * 10000) / 10000 <<
 "\npurity: " << round(purity * 10000) / 10000 << std::endl;
}
