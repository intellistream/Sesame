//
// Created by Shuhao Zhang on 26/07/2021.
//
#include <Evaluation/Evaluation.hpp>
#include <cmath>
#include <Utils/Logger.hpp>

double getDistance(Point *inputs, Point *centres, int dimension) {
  int i;
  double f = 0.0;
  for (i = 0; i < dimension; i++) {
    f += pow((inputs->getFeatureItem(i) - centres->getFeatureItem(i)), 2);
  }
  return f;
}

void SESAME::Evaluation::euclideanCost(int numberOfPoints,
                                       int numberOfCenters,
                                       int dimention,
                                       std::vector<Point> &inputs,
                                       std::vector<Point> &results) {
  double mincost = 0.0;
  double kmeansoverall = 0.0;
  double distance = 0.0;
  for (int i = 0; i < numberOfPoints; i++) {
    mincost = getDistance(&inputs[i], &results[0], dimention);
    for (int j = 0; j < numberOfCenters; j++) {     // looking for the nearest center
      distance = getDistance(&inputs[i], &results[j], dimention); //
      if (distance < mincost)
        mincost = distance;
    }
  }
  kmeansoverall += mincost;
  SESAME_INFO("EuclideanCost:" << kmeansoverall);
}
