// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 15/8/2021.
//
#include <Sources/DataSourceFactory.hpp>

SESAME::DataSourcePtr SESAME::DataSourceFactory::create() {
  return std::make_shared<SESAME::DataSource>();
}