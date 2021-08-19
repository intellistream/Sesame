// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by tuidan on 2021/7/22.
//

#include <Algorithm/Algorithm.hpp>
#include <Algorithm/StreamKM.hpp>
void SESAME::Algorithm::store(std::string outputPath,
                              int numberOfCenters,
                              int dimension,
                              std::vector<SESAME::PointPtr> result) {

  FILE *out = fopen(outputPath.c_str(), "w");
  for (int i = 0; i < numberOfCenters; i++) {
    int l;
    fprintf(out, "%f ", result[i]->getWeight());
    for (l = 0; l < dimension - 1; l++) {
      if (result[i]->getWeight() != 0.0) {
        fprintf(out, "%f ", result[i]->getFeatureItem(l) / result[i]->getWeight());
      } else {
        fprintf(out, "%f ", result[i]->getFeatureItem(l));
      }
    }
    if (result[i]->getWeight() != 0.0) {
      fprintf(out, "%f", result[i]->getFeatureItem(dimension - 1) / result[i]->getWeight());
    } else {
      fprintf(out, "%f", result[i]->getFeatureItem(dimension - 1));
    }
    fprintf(out, "\n");
  }
  fclose(out);
}
