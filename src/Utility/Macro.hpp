//
// Created by tuidan on 2021/7/10.
//

#ifndef ONLINEMLBENCHMARK_MACRO_HPP_
#define ONLINEMLBENCHMARK_MACRO_HPP_

/**KMeans**/
#define TRUE 1
#define FALSE 0
#define THRESHOLD 1.000  // Determines when Lloyd terminates (should be between 0 and 1)

/**RandomGenerator**/
/* Period parameters */
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

static unsigned long mt[N]; /* the array for the state vector  */
static int mti=N+1; /* mti==N+1 means mt[N] is not initialized */





#endif //ONLINEMLBENCHMARK_MACRO_H
