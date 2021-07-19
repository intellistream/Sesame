//
// Created by Shuhao Zhang on 19/07/2021.
//

#include <util/BenchmarkUtils.hpp>
#include <Utils/Logger.hpp>
void BenchmarkUtils::parseArgs(int argc, char **argv, param_t *cmd_params) {
  if (argc != 6)
    SESAME_ERROR("need five parameter! PointNumber; ClusterNumber; Dimension; CoresetSize; Seed");
  else {
    cmd_params->pointNumber = atoi(argv[1]);
    cmd_params->pointNumber = atoi(argv[1]);
    cmd_params->clusterNumber = atoi(argv[2]);
    cmd_params->dimension = atoi(argv[3]);
    cmd_params->coreSize = atoi(argv[4]);
    cmd_params->seed = atoi(argv[5]);
  }
}
void BenchmarkUtils::defaultParam(param_t *cmd_params) {
  cmd_params->pointNumber = 568592;
  cmd_params->pointNumber = 10;
  cmd_params->clusterNumber = 54;
  cmd_params->dimension = 20;
  cmd_params->coreSize = 10;
}


