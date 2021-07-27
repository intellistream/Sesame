//
// Created by Shuhao Zhang on 20/07/2021.
//

#include <Sources/DataSource.hpp>
#include <vector>
#include <Algorithm/DataStructure/DataStructureFactory.hpp>

/**
 * Create input data points.
 * TODO: Remove the hard-coded part.
 * @param point_number
 * @param dimension
 * @param input
 * @return
 */
void SESAME::DataSource::create(int point_number, int dimension, vector<string> input, vector<PointPtr> &points) {

  for (int i = 0; i < point_number; i++) {
    PointPtr point = DataStructureFactory::createPoint(i, 1, dimension, 0);
    char *charData = new char[10000];
    strcpy(charData, input[i].c_str());
    // use c_str() to convert string to char * but it's just a temp pointer we have to use strcpy to store it
    const char *sep = " ";
    char *feature = strtok(charData, sep);
    feature = strtok(nullptr, sep);
    int index = 0;
    while (feature != nullptr) {
      if (index == dimension) {
        point->setClusteringCenter(atoi(feature));
      } else {
        point->setFeatureItem(strtod(feature, nullptr), index);
        index++;
      }
      feature = strtok(nullptr, sep);
    }
    points.push_back(point);
  }
}