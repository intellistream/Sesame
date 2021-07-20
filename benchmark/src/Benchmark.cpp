
/**
 * @brief This is the main entry point of the entire program.
 * Users will typically access this file to use the stream clustering algorithm.
 * We use this as the entry point for benchmarking.
 */
#include <util/BenchmarkUtils.hpp>
#include <Utils/Logger.hpp>

using namespace std;
int main(int argc, char **argv) {
  //Setup Logs.
  setupLogging("benchmark.log", LOG_DEBUG);

  //Parse parameters.
  param_t cmd_params;
  BenchmarkUtils::defaultParam(&cmd_params);
  BenchmarkUtils::parseArgs(argc, argv, &cmd_params);

  //Load datasets.
  string *data = BenchmarkUtils::loadData(&cmd_params);

  //Run algorithm.
  BenchmarkUtils::runBenchmark(&cmd_params, data);

  //Evaluate results.
  //TODO.

}

