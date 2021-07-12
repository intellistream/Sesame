//
// Created by Shuhao Zhang on 9/6/21.
//

#include "OfflineClustering.hpp"

/* generates a random number on [0,0x7fffffff]-interval */
long OfflineClustering::genrand_int31() {
    return (long)(genrand_int32()>>1);
}

/* generates a random number on (0,1)-real-interval */
double OfflineClustering::genrand_real3() {
    return (((double)genrand_int32()) + 0.5)*(1.0/4294967296.0);
    /* divided by 2^32 */}
}

/* initializes mt[N] with a seed */
void OfflineClustering::init_genrand(unsigned long s) {
    /* initializes mt[N] with a seed */
    mt[0]= s & 0xffffffffUL;
    for (mti=1; mti<N; mti++) {
        mt[mti] =
                (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti);
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array mt[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        mt[mti] &= 0xffffffffUL;
        /* for >32 bit machines */
    }
}