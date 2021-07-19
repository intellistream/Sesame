
/**
 * @brief This is the main entry point of the entire program.
 * Users will typically access this file to use the stream clustering algorithm.
 * We use this as the entry point for benchmarking.
 */
#include <util/BenchmarkUtils.hpp>
#include <Utils/Logger.hpp>
int main(int argc, char **argv) {
  //Setup Logs.
  setupLogging("benchmark.log", LOG_DEBUG);

  //Parse parameters.
  param_t cmd_params;
  BenchmarkUtils::defaultParam(&cmd_params);
  BenchmarkUtils::parseArgs(argc, argv, &cmd_params);

  //Load datasets.

}

