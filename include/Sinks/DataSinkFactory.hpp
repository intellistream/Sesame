//
// Created by Shuhao Zhang on 17/8/21.
//

#ifndef SESAME_SRC_SINKS_DATASINKFACTORY_HPP_
#define SESAME_SRC_SINKS_DATASINKFACTORY_HPP_
#include <Sinks/DataSink.hpp>
namespace SESAME {
class DataSinkFactory {

 public:
  static SESAME::DataSinkPtr create();
};
}

#endif //SESAME_SRC_SINKS_DATASINKFACTORY_HPP_
