// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

#include "Utils/UtilityFunctions.hpp"

#include <cfloat>
#include <cmath>
#include <random>

#include <omp.h>

static unsigned long mt[N]; /* the array for the state vector  */
static int mti;             /* mti==N+1 means mt[N] is not initialized */

long SESAME::UtilityFunctions::genrand_int31() {
  return long(genrand_int32() >> 1);
}
unsigned long SESAME::UtilityFunctions::genrand_int32() {
  unsigned long y;
  static unsigned long mag01[2] = {0x0UL, MATRIX_A};
  /* mag01[x] = x * MATRIX_A  for x=0,1 */

  if (mti >= N) { /* generate N words at one time */
    int kk;

    if (mti == N + 1)       /* if init_genrand() has not been called, */
      init_genrand(5489UL); /* a default initial seed is used */

    for (kk = 0; kk < N - M; kk++) {
      y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
      mt[kk] = mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }
    for (; kk < N - 1; kk++) {
      y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
      mt[kk] = mt[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }
    y = (mt[N - 1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
    mt[N - 1] = mt[M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

    mti = 0;
  }

  y = mt[mti++];

  /* Tempering */
  y ^= (y >> 11);
  y ^= (y << 7) & 0x9d2c5680UL;
  y ^= (y << 15) & 0xefc60000UL;
  y ^= (y >> 18);

  return y;
}
SESAME::UtilityFunctions::UtilityFunctions() {
  mti = N + 1; /* mti==N+1 means mt[N] is not initialized */
}

/**
 * initializes mt[N] with a seed
 * @param s
 */
void SESAME::UtilityFunctions::init_genrand(unsigned long s) {
  /* initializes mt[N] with a seed */
  mt[0] = s & 0xffffffffUL;
  for (mti = 1; mti < N; mti++) {
    mt[mti] = (1812433253UL * (mt[mti - 1] ^ (mt[mti - 1] >> 30)) + mti);
    /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
    /* In the previous versions, MSBs of the seed affect   */
    /* only MSBs of the array mt[].                        */
    /* 2002/01/09 modified by Makoto Matsumoto             */
    mt[mti] &= 0xffffffffUL;
    /* for >32 bit machines */
  }
}

/**
 *  generates a random number on (0,1)-real-interval
 * @return
 */
double SESAME::UtilityFunctions::genrand_real3() {
  return (((double)genrand_int32()) + 0.5) * (1.0 / 4294967296.0);
  /* divided by 2^32 */
}
std::shared_ptr<std::barrier<>>
SESAME::UtilityFunctions::createBarrier(int count) {
  return std::make_shared<std::barrier<>>(count);
}
//void SESAME::UtilityFunctions::groupByCenters(
//    const std::vector<PointPtr> &input, const std::vector<PointPtr> &centers,
//    std::vector<PointPtr> &output, int dim) {
//  auto n = input.size();
//  for (int i = 0; i < n; i++)
//    output.push_back(input[i]->copy());
//#pragma omp parallel for
//  for (int i = 0; i < n; i++) {
//    auto min = DBL_MAX;
//    int selectCenterIndex = -1;
//    bool outlier = false;
//    for (int j = 0; j < centers.size(); j++) {
//      double dis = output[i]->L2Dist(centers[j]);
//      if (min > dis) {
//        selectCenterIndex = j;
//        min = dis;
//        if(centers[j]->getOutlier()){
//          outlier = true;
//        } else {
//          outlier = false;
//        }
//      }
//    }
//    output[i]->setOutlier(outlier);
//    output[i]->setClusteringCenter(selectCenterIndex);
//  }
//}

void SESAME::UtilityFunctions::groupByCenters(
    const std::vector<PointPtr> &input, const std::vector<PointPtr> &centers,
    std::vector<PointPtr> &output, int dim) {
  auto n = input.size();
  for (int i = 0; i < n; i++)
    output.push_back(input[i]->copy());
#pragma omp parallel for
  for (int i = 0; i < n; i++) {
    auto min = DBL_MAX;
    for (int j = 0; j < centers.size(); j++) {
      double dis = output[i]->L2Dist(centers[j]);
      if (min > dis) {
        output[i]->setClusteringCenter(centers[j]->getClusteringCenter());
        min = dis;
      }
    }
//    if(output[i]->getClusteringCenter() == -1){
//      output[i]->setOutlier(true);
//    } else {
//      output[i]->setOutlier(false);
//    }
  }
}
