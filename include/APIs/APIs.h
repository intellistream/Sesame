//
// Created by Shuhao Zhang on 19/07/2021.
//

#ifndef SESAME_INCLUDE_APIS_APIS_H_
#define SESAME_INCLUDE_APIS_APIS_H_

#include <Algorithm/DataStructure/Point.hpp>
#include <string>
using namespace std;
namespace SESAME {

class APIs {
 public:
  Point *createPoints(int point_number, int attribute_numbr, string *dataLine);
};
}

#endif //SESAME_INCLUDE_APIS_APIS_H_
