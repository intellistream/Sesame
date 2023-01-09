// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by tuidan on 2021/8/25.
//

#include <gtest/gtest.h>

#include <filesystem>

#include "Algorithm/AlgorithmFactory.hpp"
#include "Algorithm/DataStructure/GenericFactory.hpp"
#include "Sinks/DataSinkFactory.hpp"
#include "Sources/DataSourceFactory.hpp"
#include "Utils/BenchmarkUtils.hpp"
#include "Utils/Logger.hpp"

TEST(SystemTest, Birch)
{
    // [529, 999, 1270, 1624, 2001, 2435, 2648, 3000]
    // [3, 3, 4, 6, 6, 7, 9, 9]
    // Parse parameters.
    param_t param;
    param.num_points         = 10000;
    param.max_in_nodes       = 400;
    param.max_leaf_nodes     = 100;
    param.distance_threshold = 600;
    param.dim                = 54;
    param.num_clusters       = 7;
    param.landmark           = 1000;
    param.run_cmm            = false;

    param.input_file = std::filesystem::current_path().generic_string() + "/datasets/CoverType.txt";
    param.output_file = "results.txt";
    param.algo        = SESAME::BirchType;

    std::vector<SESAME::PointPtr> input;
    std::vector<SESAME::PointPtr> results;

    // Create Spout.
    SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(param);
    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(param, sourcePtr);

    // Create Sink.
    SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(param);

    // Create Algorithm.
    SESAME::AlgorithmPtr algoPtr = SESAME::AlgorithmFactory::create(param);

    // Run algorithm producing results.
    auto res = BenchmarkUtils::runBenchmark(param, sourcePtr, sinkPtr, algoPtr);

    ASSERT_NEAR(res->purity, 0.3749, 0.02);
}
