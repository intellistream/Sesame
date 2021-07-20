//
// Created by Shuhao Zhang on 20/07/2021.
//

#ifndef SESAME_SRC_SOURCES_DATASOURCE_HPP_
#define SESAME_SRC_SOURCES_DATASOURCE_HPP_

#include <Algorithm/DataStructure/Point.hpp>
#include <string>
#include <cstring>

using namespace std;

namespace SESAME {
class DataSource {
 public:
  static Point *create(int point_number, int dimension, string *input);

};
}
#endif //SESAME_SRC_SOURCES_DATASOURCE_HPP_
