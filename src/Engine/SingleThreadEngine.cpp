//
// Created by Shuhao Zhang on 19/07/2021.
//

#include <Engine/SingleThreadEngine.hpp>

SingleThreadEngine::SingleThreadEngine() {
  struct param_t init = {568592, 10, 54, 20, 10};
  this->cmd_params = init;
  this->outputPath = "/Users/tuidan/code/C++/1/Sesame/src/Benchmark/Workload/new.txt";
}

param_t SingleThreadEngine::getParam() {
  return this->cmd_params;
}

void SingleThreadEngine::parse_args(int argc, char **argv) {
  if (argc != 6) cout << "need five parameter! PointNumber; ClusterNumber; Dimension; CoresetSize; Seed";
  else {
    this->cmd_params.pointNumber = atoi(argv[1]);
    this->cmd_params.clusterNumber = atoi(argv[2]);
    this->cmd_params.dimension = atoi(argv[3]);
    this->cmd_params.coresetSize = atoi(argv[4]);
    this->cmd_params.seed = atoi(argv[5]);
  }
}

bool SingleThreadEngine::runAlgorithm() {
  return false;
}