
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
  BenchmarkUtils::defaultParam(cmd_params);
  BenchmarkUtils::parseArgs(argc, argv, cmd_params);

  std::vector<Point> input;
  std::vector<Point> results;

  //Load input.
  BenchmarkUtils::loadData(cmd_params, input);

  //Run algorithm producing results.
  BenchmarkUtils::runBenchmark(cmd_params, input, results);

  //Evaluate results against input.
  BenchmarkUtils::evaluate(cmd_params, input, results);

  //Store results.
  BenchmarkUtils::store(cmd_params, results);
}

