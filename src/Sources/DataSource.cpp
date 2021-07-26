//
// Created by Shuhao Zhang on 20/07/2021.
//

#include <Sources/DataSource.hpp>
#include <vector>

/**
 * Create input data points.
 * TODO: Remove the hard-coded part.
 * @param point_number
 * @param dimension
 * @param input
 * @return
 */
void SESAME::DataSource::create(int point_number, int dimension, string *input, std::vector<Point> &points) {

  for (int i = 0; i < point_number; i++) {
    points[i].Initialization(i, 1, dimension, 0);
    char *charData = new char[10000];
    strcpy(charData, input[i].c_str());
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
}