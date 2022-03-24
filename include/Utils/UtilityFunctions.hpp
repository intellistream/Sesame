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

/* Period parameters */
const int N = 624;
const int M = 397;
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */
#define TRUE 1
#define FALSE 0
#define DEFAULT_WEIGHT 1
#define DEFAULT_COST 0
#define DEFAULT_QUEUE_CAPACITY 1000
#define KMEANS_TIMES 5
#define CMM_KNN 10
#define CMM_A 0.998
#define CMM_LAMDA 1
#define CMM_THRESHOLD 542
#define UNCLASSIFIED (-2)
#define CORE_POINT 1
#define NOISE (-1)
#define SUCCESS 0
#define FAILURE (-3)
/*
Determines when Lloyd terminates (should be between 0 and 1)
*/
#define THRESHOLD 1.000

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
} // namespace SESAME
#endif // SESAME_SRC_UTILS_UTILITYFUNCTIONS_HPP_
