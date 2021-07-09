//
// Created by Shuhao Zhang on 9/6/21.
//

#ifndef ONLINEMLBENCHMARK_SRC_OFFLINECLUSTERING_OFFLINECLUSTERING_HPP_
#define ONLINEMLBENCHMARK_SRC_OFFLINECLUSTERING_OFFLINECLUSTERING_HPP_

#include "../../Benchmark/DataLoader/Point.hpp"
#include <stdio.h>

/* Period parameters */
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

static unsigned long mt[N]; /* the array for the state vector  */
static int mti=N+1; /* mti==N+1 means mt[N] is not initialized */

class OfflineClustering {
public:
    void init_genrand(unsigned long s);
    long genrand_int31(void);
    double genrand_real3(void);
};

#endif // ONLINEMLBENCHMARK_SRC_OFFLINECLUSTERING_OFFLINECLUSTERING_HPP_
