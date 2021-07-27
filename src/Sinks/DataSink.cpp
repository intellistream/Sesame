//
// Created by Shuhao Zhang on 20/07/2021.
//

#include <Sinks/DataSink.hpp>

// Store the results
void SESAME::DataSink::store(std::string outputPath, int numberOfCenters, int dimension,
                             const std::vector<PointPtr> &centers) {
  FILE *out = fopen(outputPath.c_str(), "w");
  for (int i = 0; i < numberOfCenters; i++) {
    int l;
    fprintf(out, "%f ", centers[i]->getWeight());
    for (l = 0; l < dimension - 1; l++) {
      if (centers[i]->getWeight() != 0.0) {
        fprintf(out, "%f ", centers[i]->getFeatureItem(l) / centers[i]->getWeight());
      } else {
        fprintf(out, "%f ", centers[i]->getFeatureItem(l));
      }
    }
    if (centers[i]->getWeight() != 0.0) {
      fprintf(out, "%f", centers[i]->getFeatureItem(dimension - 1) / centers[i]->getWeight());
    } else {
      fprintf(out, "%f", centers[i]->getFeatureItem(dimension - 1));
    }
    fprintf(out, "\n");
  }
  fclose(out);
}
