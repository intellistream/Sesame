//
// Created by Shuhao Zhang on 20/07/2021.
//

#include <Sources/DataSource.hpp>

/**
 * Create input data points.
 * TODO: Remove the hard-coded part.
 * @param point_number
 * @param dimension
 * @param dataLine
 * @return
 */
Point *SESAME::DataSource::create(int point_number, int dimension, string *dataLine) {
  Point points[point_number];
  for (int i = 0; i < point_number; i++) {
    points[i].Initialization(i, 1, dimension, 0);
    char *charData = new char[10000];
    strcpy(charData, dataLine[i].c_str());
    // use c_str() to convert string to char * but it's just a temp pointer we have to use strcpy to store it
    const char *sep = " ";
    char *feature = strtok(charData, sep);
    feature = strtok(nullptr, sep);
    int index = 0;
    while (feature != nullptr) {
      if (index == dimension) {
        points[i].setClusteringCenter(atoi(feature));
      } else {
        points[i].setFeatureItem(strtod(feature, nullptr), index);
        index++;
      }
      feature = strtok(nullptr, sep);
    }
  }
  return points;
}