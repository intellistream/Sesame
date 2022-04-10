// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 20/07/2021.
//

#ifndef SESAME_SRC_UTILS_UTILITYFUNCTIONS_HPP_
#define SESAME_SRC_UTILS_UTILITYFUNCTIONS_HPP_

#include "Algorithm/DataStructure/Point.hpp"

#include <barrier>
#include <experimental/filesystem>
#include <functional>
#include <string>
#include <random>

using uint64 = unsigned long long;
using int64 = long long;

/* Period parameters */
const int N = 624;
const int M = 397;
const uint64 MATRIX_A = 0x9908b0dfUL;   /* constant vector a */
const uint64 UPPER_MASK = 0x80000000UL; /* most significant w-r bits */
const uint64 LOWER_MASK = 0x7fffffffUL; /* least significant r bits */
const int TRUE = 1;
const int FALSE = 0;
const int DEFAULT_WEIGHT = 1;
const int DEFAULT_COST = 0;
const int DEFAULT_QUEUE_CAPACITY = 3000;
const int KMEANS_TIMES = 5;
const int CMM_KNN = 10;
const double CMM_A = 0.998;
const int CMM_LAMDA = 1;
const int CMM_THRESHOLD = 542;
const int UNCLASSIFIED = (-2);
const int CORE_POINT = 1;
const int NOISE = (-1);
const int SUCCESS = 0;
const int FAILURE = (-3);
/*
Determines when Lloyd terminates (should be between 0 and 1)
*/
const double THRESHOLD = 1.000;

namespace SESAME {
typedef std::shared_ptr<std::barrier<>> BarrierPtr;

class UtilityFunctions {

public:
  UtilityFunctions();
  static void init_genrand(unsigned long s);
  static double genrand_real3();
  static long genrand_int31(void);
  static unsigned long genrand_int32(void);
  static std::shared_ptr<std::barrier<>> createBarrier(int count);
  static void groupByCenters(const std::vector<PointPtr> &input,
                             const std::vector<PointPtr> &centers,
                             std::vector<PointPtr> &output, int dimension);
  static void groupByCentersWithOffline(const std::vector<PointPtr> &input,
                                        const std::vector<PointPtr> &centers,
                                        std::vector<PointPtr> &output,
                                        int dimension);
};

static std::mt19937 r(std::random_device{}());
static int random_uniform(int min, int max) {
  std::uniform_int_distribution<int> dist(min, max);
  return dist(r);
}
static double random_uniform(double min, double max) {
  std::uniform_real_distribution<double> dist(min, max);
  return dist(r);
}
static bool bernoulli(double p){
  std::bernoulli_distribution dist(p);
  return dist(r);
}

} // namespace SESAME
#endif // SESAME_SRC_UTILS_UTILITYFUNCTIONS_HPP_
