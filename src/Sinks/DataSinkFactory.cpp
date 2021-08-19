// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 17/8/21.
//

#include <Sinks/DataSinkFactory.hpp>

SESAME::DataSinkPtr SESAME::DataSinkFactory::create() {
  return std::make_shared<SESAME::DataSink>();
}
