//
// Created by Shuhao Zhang on 20/07/2021.
//

#include <Sinks/DataSink.hpp>
#include <cmath>
#include <Utils/Logger.hpp>

// Store the results
void SESAME::DataSink::store(std::string outputPath, int numberOfCenters, int dimension, Point *centers) {
  FILE *out = fopen(outputPath.c_str(), "w");
  for (int i = 0; i < numberOfCenters; i++) {
    int l;
    fprintf(out, "%f ", centers[i].getWeight());
    for (l = 0; l < dimension - 1; l++) {
      if (centers[i].getWeight() != 0.0) {
        fprintf(out, "%f ", centers[i].getFeatureItem(l) / centers[i].getWeight());
      } else {
        fprintf(out, "%f ", centers[i].getFeatureItem(l));
      }
    }
    if (centers[i].getWeight() != 0.0) {
      fprintf(out, "%f", centers[i].getFeatureItem(dimension - 1) / centers[i].getWeight());
    } else {
      fprintf(out, "%f", centers[i].getFeatureItem(dimension - 1));
    }
    fprintf(out, "\n");
  }
  fclose(out);
}

double getDistance(Point *inputs, Point *centres, int dimension) {
  int i;
  double f = 0.0;
  for (i = 0; i < dimension; i++) {
    f += pow((inputs->getFeatureItem(i) - centres->getFeatureItem(i)), 2);
  }
  return f;
}

void SESAME::DataSink::SSE(int numberOfPoints, int numberOfCenters, int dimention, Point *inputs, Point *results) {
  double mincost = 0.0;
  double kmeansoverall = 0.0;
  double distance = 0.0;
  for (int i = 0; i < numberOfPoints; i++) {
    mincost = getDistance(inputs, results, dimention);
    for (int j = 0; j < numberOfCenters; j++) {     // looking for the nearest center
      distance = getDistance(inputs, &results[j], dimention); //
      if (distance < mincost)
        mincost = distance;
    }
  }
  kmeansoverall += mincost;
  SESAME_INFO("SSE:" << kmeansoverall);
}

