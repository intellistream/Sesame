//
// Created by tuidan on 2021/9/15.
//

#include <gtest/gtest.h>

#include <filesystem>

#include "Algorithm/AlgorithmFactory.hpp"
#include "Algorithm/DataStructure/GenericFactory.hpp"
#include "Sinks/DataSinkFactory.hpp"
#include "Sources/DataSourceFactory.hpp"
#include "Utils/BenchmarkUtils.hpp"
#include "Utils/Logger.hpp"

TEST(SystemTest, V10Test)
{
    // [529, 999, 1270, 1624, 2001, 2435, 2648, 3000]
    // [3, 3, 4, 6, 6, 7, 9, 9]
    // Parse parameters.
    param_t cmd_params;
    cmd_params.num_points = 1000;
    cmd_params.landmark   = 0;
    cmd_params.dim        = 5;
    cmd_params.alpha      = 0.9;
    cmd_params.beta       = 0.7;
    cmd_params.num_cache  = 100;
    cmd_params.radius     = 1.6;  // 220
    cmd_params.lambda     = 0.6;
    cmd_params.delta      = 300;

    cmd_params.beta = 0.001;
    cmd_params.opt  = 2;
    // [529, 999, 1270, 1624, 2001, 2435, 2648, 3000]
    // [3, 3, 4, 6, 6, 7, 9, 9]
    cmd_params.num_clusters = 54;
    cmd_params.time_decay   = true;

    cmd_params.input_file =
        std::filesystem::current_path().generic_string() + "/datasets/sensor.txt";
    cmd_params.output_file = "results.txt";
    cmd_params.algo        = SESAME::V10Stream;

    std::vector<SESAME::PointPtr> input;
    std::vector<SESAME::PointPtr> results;

    // Create Spout.
    SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(cmd_params);
    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(cmd_params, sourcePtr);

    // Create Sink.
    SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(cmd_params);

    // Create Algorithm.
    SESAME::AlgorithmPtr algoPtr = SESAME::AlgorithmFactory::create(cmd_params);

    // Run algorithm producing results.
    BenchmarkUtils::runBenchmark(cmd_params, sourcePtr, sinkPtr, algoPtr);
}