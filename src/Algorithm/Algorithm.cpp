// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by tuidan on 2021/7/22.
//

#include <Algorithm/Algorithm.hpp>
#include <Algorithm/StreamKM.hpp>
void SESAME::Algorithm::store(std::string outputPath,
                              int dimension,
                              std::vector<SESAME::PointPtr> result) {
  int numberOfCenters = (int)result.size();
  FILE *out = fopen(outputPath.c_str(), "w");
  for (int i = 0; i < numberOfCenters; i++) {
    int l;
    fprintf(out, "%f ", result[i]->getWeight());
    for (l = 0; l < dimension; l++) {
      fprintf(out, "%f ", result[i]->getFeatureItem(l) / result[i]->getWeight());
    }
    fprintf(out, "\n");
  }
  fclose(out);
}
