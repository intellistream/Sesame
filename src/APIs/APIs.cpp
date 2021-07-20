//
// Created by Shuhao Zhang on 19/07/2021.
//
#include <APIs/APIs.h>
#include <cstring>

Point *SESAME::APIs::createPoints(int point_number, int attribute_numbr, string *dataLine) {
  Point points[point_number];
  for (int i = 0; i < attribute_numbr; i++) {
    points[i].Initialization(i, 1, attribute_numbr, 0);
    char *charData = new char[10000];
    strcpy(charData, dataLine[i].c_str());
    // use c_str() to convert string to char * but it's just a temp pointer we have to use strcpy to store it
    const char *sep = " ";
    char *feature = strtok(charData, sep);
    feature = strtok(nullptr, sep);
    int index = 0;
    while (feature != nullptr) {
      if (index == attribute_numbr) {
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

