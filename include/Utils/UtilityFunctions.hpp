//
// Created by Shuhao Zhang on 20/07/2021.
//

#ifndef SESAME_SRC_UTILS_UTILITYFUNCTIONS_HPP_
#define SESAME_SRC_UTILS_UTILITYFUNCTIONS_HPP_
#include <filesystem>
/* Period parameters */
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */
#define TRUE 1
#define FALSE 0
/*
Determines when Lloyd terminates (should be between 0 and 1)
*/
#define THRESHOLD 1.000



class UtilityFunctions {

 public:
    UtilityFunctions();
  static int genrand_int31();
  static int genrand_int32();
  static void init_genrand(unsigned long s);
  static double genrand_real3();
  static std::string getDir();
};
#endif //SESAME_SRC_UTILS_UTILITYFUNCTIONS_HPP_
