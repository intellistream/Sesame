//
// Created by Shuhao Zhang on 20/07/2021.
//

#ifndef SESAME_SRC_SOURCES_DATASOURCE_HPP_
#define SESAME_SRC_SOURCES_DATASOURCE_HPP_

#include <Algorithm/DataStructure/Point.hpp>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

namespace SESAME {
class DataSource {
 public:
  static void create(int point_number, int dimension, vector<string> input, vector<PointPtr> &points);

};
}
#endif //SESAME_SRC_SOURCES_DATASOURCE_HPP_
