//
// Created by Shuhao Zhang on 17/8/21.
//

#include <Sinks/DataSinkFactory.hpp>

SESAME::DataSinkPtr SESAME::DataSinkFactory::create() {
  return std::make_shared<SESAME::DataSink>();
}
