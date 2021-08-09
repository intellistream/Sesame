//
// Created by shuhao zhang on 8/8/2021.
//

#include <gtest/gtest.h>
#include <Utils/BenchmarkUtils.hpp>
#include <Utils/Logger.hpp>

TEST(SystemTest, SampleRun) {
    //Setup Logs.
    setupLogging("benchmark.log", LOG_INFO);

    //Parse parameters.
    param_t cmd_params;
    BenchmarkUtils::defaultParam(cmd_params);

    std::vector<SESAME::PointPtr> input;
    std::vector<SESAME::PointPtr> results;
    // input: i,w1,w2,...wn,C
    // output: i,w1,w2,...wn,C

    //Load input.
    BenchmarkUtils::loadData(cmd_params, input);

    //Run algorithm producing results.
    BenchmarkUtils::runBenchmark(cmd_params, input, results);

    //Evaluate results against input.
    BenchmarkUtils::evaluate(cmd_params, input, results);

    //Store results.
    BenchmarkUtils::store(cmd_params, results);

    //TODO: specify here what is the expected results after running the algorithm.
    std::vector<SESAME::PointPtr> expected_results;

//    EXPECT_EQ(results, expected_results);
}