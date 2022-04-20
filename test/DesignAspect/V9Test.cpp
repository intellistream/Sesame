//
// Created by 1124a on 2022/4/12.
//

//
// Created by tuidan on 2022/3/14.
//

#include "Algorithm/AlgorithmFactory.hpp"
#include "Sinks/DataSinkFactory.hpp"
#include "Sources/DataSourceFactory.hpp"
#include "Utils/BenchmarkUtils.hpp"
#include "Utils/Logger.hpp"

#include "gtest/gtest.h"
#include <filesystem>

TEST(DesignTest, V9Test) {
//Setup Logs.
setupLogging("benchmark.log", LOG_DEBUG);
//Parse parameters.
param_t cmd_params;
cmd_params.num_points = 3000;
cmd_params.dim = 54;
cmd_params.num_clusters = 7;
cmd_params.beta=0.001;
cmd_params.cm = 15;
cmd_params.cl = 2;
cmd_params.outlier_distance_threshold= 30;
cmd_params.outlier_cap = 5;
cmd_params.time_decay = false;
cmd_params.landmark = 1000;
cmd_params.run_offline = false;


cmd_params.input_file = std::filesystem::current_path().generic_string() + "/datasets/CoverType.txt";
cmd_params.output_file = "results.txt";
cmd_params.algo = SESAME::V9Stream;

std::vector<SESAME::PointPtr> input;
std::vector<SESAME::PointPtr> results;

//Create Spout.
SESAME::DataSourcePtr sourcePtr = SESAME::DataSourceFactory::create();

BenchmarkUtils::loadData(cmd_params, sourcePtr);

//Create Sink.
SESAME::DataSinkPtr sinkPtr = SESAME::DataSinkFactory::create();

//Create Algorithm.
SESAME::AlgorithmPtr algoPtr = SESAME::AlgorithmFactory::create(cmd_params);

// Run algorithm producing results.
auto res =
    BenchmarkUtils::runBenchmark(cmd_params, sourcePtr, sinkPtr, algoPtr);

}