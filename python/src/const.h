/*
 * Copyright 2023 IntelliStream team (https://github.com/intellistream)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SESAME_CONST_H
#define SESAME_CONST_H

#include "Utils/BenchmarkUtils.hpp"
#include "Sources/DataSourceFactory.hpp"
#include "Sinks/DataSinkFactory.hpp"
#include "Algorithm/DataStructure/GenericFactory.hpp"
#include "Algorithm/AlgorithmFactory.hpp"
#include <filesystem>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>
#include <pybind11/pybind11.h>

void warning();

void warning() {
#ifndef NDEBUG
    std::cerr << "\033[1;31m#####################################################"
                     "#######\n"
                  << "#                                                          #\n"
                  << "#           DON'T run in debug mode.                       #\n"
                  << "#                                                          #\n"
                  << "############################################################"
                     "\033[0m\n";
        sleep(2);
#endif

}

#endif //SESAME_CONST_H
