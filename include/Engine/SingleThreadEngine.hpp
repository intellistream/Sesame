//
// Created by Shuhao Zhang on 19/07/2021.
//

#ifndef SESAME_INCLUDE_Engine_SINGLETHREADENGINE_H_
#define SESAME_INCLUDE_Engine_SINGLETHREADENGINE_H_
#include <cstdio>
#include <iostream>
#include <cstdlib>

using namespace std;

struct param_t{
  int pointNumber;
  int clusterNumber;
  int dimension;
  int coreSize;
  int seed;
};

class Executor{
 private:
  param_t cmd_params;
  char * intputPath;
  char * outputPath;
 public:
  Executor();
  param_t getParam();
  void parse_args(int argc, char **argv);
  bool runAlgorithm();
  void run();

};


#endif //SESAME_INCLUDE_Engine_SINGLETHREADENGINE_H_
