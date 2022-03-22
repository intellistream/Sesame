// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by wzru on 2022/3/14.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_GENERICFACTORY_HPP_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_GENERICFACTORY_HPP_

#include "Algorithm/DataStructure/Point.hpp"

#include <memory>

namespace SESAME {

namespace GenericFactory {

template <class T, class... Ts> std::shared_ptr<T> New(Ts &&... ts) {
  return std::make_shared<T>(std::forward<Ts>(ts)...);
}

} // namespace GenericFactory

template <typename T> concept NodeConcept = requires(T t) {
  t->Centroid();
  t->cf.numPoints;
  t->index;
  t->Update(GenericFactory::New<Point>());
  t->Scale(1.0);
};

} // namespace SESAME
#endif