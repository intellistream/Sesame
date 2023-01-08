// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by wzru on 2022/3/24.
//

#include <filesystem>

#include "Algorithm/DataStructure/CFTree.hpp"
#include "Algorithm/DataStructure/GenericFactory.hpp"
#include "Algorithm/DesignAspect/Generic.hpp"
#include "Algorithm/OfflineRefinement/DBSCAN.hpp"
#include "Algorithm/OfflineRefinement/KMeans.hpp"
#include "Algorithm/OfflineRefinement/OfflineRefinement.hpp"
#include "Algorithm/OutlierDetection/OutlierDetection.hpp"
#include "Algorithm/WindowModel/WindowModel.hpp"
#include "Sinks/DataSinkFactory.hpp"
#include "Sources/DataSourceFactory.hpp"
#include "Utils/BenchmarkUtils.hpp"
#include "Utils/Logger.hpp"
#include "gtest/gtest.h"

using namespace SESAME;
using namespace std;

TEST(GenericTest, V1)
{
    // Parse parameters.
    param_t cmd_params;
    cmd_params.num_points                 = 3000;
    cmd_params.distance_threshold         = 100;
    cmd_params.max_in_nodes               = 10;
    cmd_params.max_leaf_nodes             = 20;
    cmd_params.num_clusters               = 7;
    cmd_params.dim                        = 54;
    cmd_params.seed                       = 10;
    cmd_params.num_clusters               = 7;
    cmd_params.time_decay                 = false;
    cmd_params.landmark                   = 1000;
    cmd_params.outlier_distance_threshold = 5000;
    cmd_params.outlier_cap                = 10;
    cmd_params.k                          = 100;

    cmd_params.input_file = filesystem::current_path().generic_string() + "/datasets/CoverType.txt";
    cmd_params.output_file = "results.txt";
    cmd_params.algo        = G1Stream;
    cmd_params.run_offline = true;

    vector<PointPtr> input;
    vector<PointPtr> results;

    // Create Spout.
    DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(cmd_params);
    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(cmd_params, sourcePtr);

    // Create Sink.
    DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(cmd_params);

    // Create Algorithm.
    AlgorithmPtr algoPtr = GenericFactory::New<
        StreamClustering<Landmark, ClusteringFeaturesTree, DistanceDetection<true, false>, KMeans>>(
        cmd_params);

    // Run algorithm producing results.
    auto res = BenchmarkUtils::runBenchmark(cmd_params, sourcePtr, sinkPtr, algoPtr);

    ASSERT_NEAR(res->purity, 0.6627, 0.03);
    ASSERT_NEAR(res->cmm, 0.9361, 0.03);
}

TEST(GenericTest, V2)
{
    // Parse parameters.
    param_t cmd_params;
    cmd_params.num_points                 = 3000;
    cmd_params.distance_threshold         = 100;
    cmd_params.max_in_nodes               = 10;
    cmd_params.max_leaf_nodes             = 20;
    cmd_params.dim                        = 54;
    cmd_params.num_clusters               = 7;
    cmd_params.time_decay                 = false;
    cmd_params.min_points                 = 10;
    cmd_params.landmark                   = 1000;
    cmd_params.epsilon                    = 200;  // 0.1
    cmd_params.run_offline                = true;
    cmd_params.outlier_distance_threshold = 5000;
    cmd_params.outlier_cap                = 10;

    cmd_params.input_file = filesystem::current_path().generic_string() + "/datasets/CoverType.txt";
    cmd_params.output_file = "results.txt";
    cmd_params.algo        = SESAME::G2Stream;

    vector<SESAME::PointPtr> input;
    vector<SESAME::PointPtr> results;

    // Create Spout.
    SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(cmd_params);
    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(cmd_params, sourcePtr);

    // Create Sink.
    SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(cmd_params);

    // Create Algorithm.
    AlgorithmPtr algoPtr = GenericFactory::New<
        StreamClustering<Landmark, ClusteringFeaturesTree, DistanceDetection<true, false>, DBSCAN>>(
        cmd_params);

    // Run algorithm producing results.
    auto res = BenchmarkUtils::runBenchmark(cmd_params, sourcePtr, sinkPtr, algoPtr);

    ASSERT_NEAR(res->purity, 0.2987, 0.02);
}

TEST(GenericTest, V3)
{
    // Parse parameters.
    param_t cmd_params;
    cmd_params.num_points                 = 45690;
    cmd_params.distance_threshold         = 50;
    cmd_params.max_in_nodes               = 100;
    cmd_params.max_leaf_nodes             = 100;
    cmd_params.dim                        = 2;
    cmd_params.num_clusters               = 75;
    cmd_params.time_decay                 = true;
    cmd_params.landmark                   = 1000;
    cmd_params.outlier_distance_threshold = 500;
    cmd_params.outlier_cap                = 2000;

    cmd_params.input_file  = filesystem::current_path().generic_string() + "/datasets/EDS.txt";
    cmd_params.output_file = "results.txt";
    cmd_params.algo        = SESAME::G3Stream;

    vector<SESAME::PointPtr> input;
    vector<SESAME::PointPtr> results;

    // Create Spout.
    SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(cmd_params);
    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(cmd_params, sourcePtr);

    // Create Sink.
    SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(cmd_params);

    // Create Algorithm.
    AlgorithmPtr algoPtr =
        GenericFactory::New<StreamClustering<Landmark, ClusteringFeaturesTree,
                                             DistanceDetection<true, false>, NoRefinement>>(
            cmd_params);

    // Run algorithm producing results.
    auto res = BenchmarkUtils::runBenchmark(cmd_params, sourcePtr, sinkPtr, algoPtr);

    ASSERT_NEAR(res->purity, 0.8032, 0.02);
}

TEST(GenericTest, V4)
{
    // Parse parameters.
    param_t cmd_params;
    cmd_params.num_points                 = 3000;
    cmd_params.distance_threshold         = 100;
    cmd_params.max_in_nodes               = 20;
    cmd_params.max_leaf_nodes             = 40;
    cmd_params.dim                        = 54;
    cmd_params.num_clusters               = 7;
    cmd_params.time_decay                 = false;
    cmd_params.sliding                    = 1000;
    cmd_params.outlier_distance_threshold = 3000;
    cmd_params.outlier_cap                = 3;

    cmd_params.input_file = filesystem::current_path().generic_string() + "/datasets/CoverType.txt";
    cmd_params.output_file = "results.txt";
    cmd_params.algo        = SESAME::G4Stream;

    vector<SESAME::PointPtr> input;
    vector<SESAME::PointPtr> results;

    // Create Spout.
    SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(cmd_params);
    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(cmd_params, sourcePtr);

    // Create Sink.
    SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(cmd_params);

    // Create Algorithm.
    AlgorithmPtr algoPtr = GenericFactory::New<StreamClustering<
        Sliding, ClusteringFeaturesTree, DistanceDetection<true, false>, NoRefinement>>(cmd_params);

    // Run algorithm producing results.
    auto res = BenchmarkUtils::runBenchmark(cmd_params, sourcePtr, sinkPtr, algoPtr);

    ASSERT_NEAR(res->purity, 0.64, 0.02);
}

TEST(GenericTest, V5)
{
    // Parse parameters.
    param_t cmd_params;
    cmd_params.num_points                 = 3000;
    cmd_params.distance_threshold         = 1000;
    cmd_params.max_in_nodes               = 20;
    cmd_params.max_leaf_nodes             = 40;
    cmd_params.dim                        = 54;
    cmd_params.num_clusters               = 7;
    cmd_params.time_decay                 = false;
    cmd_params.lambda                     = 1.25;
    cmd_params.alpha                      = 2;
    cmd_params.outlier_distance_threshold = 500;
    cmd_params.outlier_cap                = 100;

    cmd_params.input_file = filesystem::current_path().generic_string() + "/datasets/CoverType.txt";
    cmd_params.output_file = "results.txt";
    cmd_params.algo        = SESAME::G5Stream;

    vector<SESAME::PointPtr> input;
    vector<SESAME::PointPtr> results;

    // Create Spout.
    SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(cmd_params);
    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(cmd_params, sourcePtr);

    // Create Sink.
    SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(cmd_params);

    // Create Algorithm.
    AlgorithmPtr algoPtr = GenericFactory::New<StreamClustering<
        Damped, ClusteringFeaturesTree, DistanceDetection<true, false>, NoRefinement>>(cmd_params);

    // Run algorithm producing results.
    auto res = BenchmarkUtils::runBenchmark(cmd_params, sourcePtr, sinkPtr, algoPtr);

    ASSERT_NEAR(res->purity, 0.542, 0.03);
    ASSERT_NEAR(res->cmm, 0.857, 0.03);
}

TEST(GenericTest, V6)
{
    // Parse parameters.
    param_t cmd_params;

    cmd_params.num_points         = 10000;
    cmd_params.distance_threshold = 10;
    cmd_params.max_in_nodes       = 100;
    cmd_params.max_leaf_nodes     = 50;
    cmd_params.dim                = 2;
    cmd_params.num_clusters       = 363;
    cmd_params.time_decay         = false;
    cmd_params.landmark           = 1000;

    cmd_params.input_file  = filesystem::current_path().generic_string() + "/datasets/EDS.txt";
    cmd_params.output_file = "results.txt";
    cmd_params.algo        = SESAME::G6Stream;

    vector<SESAME::PointPtr> input;
    vector<SESAME::PointPtr> results;

    // Create Spout.
    SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(cmd_params);
    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(cmd_params, sourcePtr);

    // Create Sink.
    SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(cmd_params);

    // Create Algorithm.
    AlgorithmPtr algoPtr = GenericFactory::New<
        StreamClustering<Landmark, ClusteringFeaturesTree, NoDetection<true, false>, NoRefinement>>(
        cmd_params);

    // Run algorithm producing results.
    auto res = BenchmarkUtils::runBenchmark(cmd_params, sourcePtr, sinkPtr, algoPtr);

    ASSERT_NEAR(res->purity, 0.8251, 0.03);
    ASSERT_NEAR(res->cmm, 0.8571, 0.03);
}

TEST(GenericTest, V7)
{
    // Parse parameters.
    param_t cmd_params;
    cmd_params.num_points                = 3000;
    cmd_params.distance_threshold        = 1000;
    cmd_params.max_in_nodes              = 20;
    cmd_params.max_leaf_nodes            = 40;
    cmd_params.dim                       = 54;
    cmd_params.num_clusters              = 7;
    cmd_params.time_decay                = false;
    cmd_params.landmark                  = 1000;
    cmd_params.neighbor_distance         = 5000;
    cmd_params.outlier_density_threshold = 0.5;
    cmd_params.outlier_cap               = 10;

    cmd_params.input_file = filesystem::current_path().generic_string() + "/datasets/CoverType.txt";
    cmd_params.output_file = "results.txt";
    cmd_params.algo        = SESAME::G7Stream;

    vector<SESAME::PointPtr> input;
    vector<SESAME::PointPtr> results;

    // Create Spout.
    SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(cmd_params);
    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(cmd_params, sourcePtr);

    // Create Sink.
    SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(cmd_params);

    // Create Algorithm.
    AlgorithmPtr algoPtr = GenericFactory::New<StreamClustering<
        Landmark, ClusteringFeaturesTree, DensityDetection<true, false>, NoRefinement>>(cmd_params);

    // Run algorithm producing results.
    auto res = BenchmarkUtils::runBenchmark(cmd_params, sourcePtr, sinkPtr, algoPtr);

    ASSERT_NEAR(res->purity, 0.432, 0.02);
}

TEST(GenericTest, V8)
{
    // Parse parameters.
    param_t cmd_params;
    cmd_params.num_points                 = 45690;
    cmd_params.distance_threshold         = 50;
    cmd_params.max_in_nodes               = 100;
    cmd_params.max_leaf_nodes             = 100;
    cmd_params.dim                        = 2;
    cmd_params.num_clusters               = 75;
    cmd_params.time_decay                 = true;
    cmd_params.landmark                   = 1000;
    cmd_params.run_offline                = false;
    cmd_params.outlier_distance_threshold = 600;
    cmd_params.outlier_cap                = 2000;

    cmd_params.input_file  = filesystem::current_path().generic_string() + "/datasets/EDS.txt";
    cmd_params.output_file = "results.txt";
    cmd_params.algo        = SESAME::V8Stream;

    vector<SESAME::PointPtr> input;
    vector<SESAME::PointPtr> results;

    // Create Spout.
    SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(cmd_params);
    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(cmd_params, sourcePtr);

    // Create Sink.
    SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(cmd_params);

    // Create Algorithm.
    AlgorithmPtr algoPtr =
        GenericFactory::New<StreamClustering<Landmark, ClusteringFeaturesList,
                                             DistanceDetection<true, false>, NoRefinement>>(
            cmd_params);

    // Run algorithm producing results.
    auto res = BenchmarkUtils::runBenchmark(cmd_params, sourcePtr, sinkPtr, algoPtr);

    ASSERT_NEAR(res->purity, 0.9962, 0.01);
}

TEST(GenericTest, V11)
{
    // Parse parameters.
    param_t cmd_params;

    cmd_params.num_points         = 10000;
    cmd_params.distance_threshold = 10;
    cmd_params.max_in_nodes       = 100;
    cmd_params.max_leaf_nodes     = 50;
    cmd_params.dim                = 2;
    cmd_params.num_clusters       = 363;
    cmd_params.time_decay         = false;
    cmd_params.landmark           = 1000;
    cmd_params.clean_interval     = 9900;

    cmd_params.input_file  = filesystem::current_path().generic_string() + "/datasets/EDS.txt";
    cmd_params.output_file = "results.txt";
    cmd_params.algo        = SESAME::G11Stream;

    vector<SESAME::PointPtr> input;
    vector<SESAME::PointPtr> results;

    // Create Spout.
    SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(cmd_params);
    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(cmd_params, sourcePtr);

    // Create Sink.
    SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(cmd_params);

    // Create Algorithm.
    AlgorithmPtr algoPtr = GenericFactory::New<
        StreamClustering<Landmark, ClusteringFeaturesTree, NoDetection<false, true>, NoRefinement>>(
        cmd_params);

    // Run algorithm producing results.
    auto res = BenchmarkUtils::runBenchmark(cmd_params, sourcePtr, sinkPtr, algoPtr);

    ASSERT_NEAR(res->purity, 0.82, 0.01);
}

TEST(GenericTest, V12)
{
    // Parse parameters.
    param_t cmd_params;

    cmd_params.num_points         = 10000;
    cmd_params.distance_threshold = 10;
    cmd_params.max_in_nodes       = 100;
    cmd_params.max_leaf_nodes     = 50;
    cmd_params.dim                = 2;
    cmd_params.num_clusters       = 363;
    cmd_params.time_decay         = false;
    cmd_params.landmark           = 1000;

    cmd_params.input_file  = filesystem::current_path().generic_string() + "/datasets/EDS.txt";
    cmd_params.output_file = "results.txt";
    cmd_params.algo        = SESAME::G12Stream;

    vector<SESAME::PointPtr> input;
    vector<SESAME::PointPtr> results;

    // Create Spout.
    SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(cmd_params);
    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(cmd_params, sourcePtr);

    // Create Sink.
    SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(cmd_params);

    // Create Algorithm.
    AlgorithmPtr algoPtr =
        GenericFactory::New<StreamClustering<Landmark, ClusteringFeaturesTree,
                                             NoDetection<false, false>, NoRefinement>>(cmd_params);

    // Run algorithm producing results.
    auto res = BenchmarkUtils::runBenchmark(cmd_params, sourcePtr, sinkPtr, algoPtr);

    ASSERT_NEAR(res->purity, 0.8228, 0.03);
    ASSERT_NEAR(res->cmm, 0.8554, 0.03);
}

TEST(GenericTest, V13)
{
    // Parse parameters.
    param_t cmd_params;

    cmd_params.num_points         = 10000;
    cmd_params.distance_threshold = 10;
    cmd_params.max_in_nodes       = 100;
    cmd_params.max_leaf_nodes     = 50;
    cmd_params.dim                = 2;
    cmd_params.num_clusters       = 363;
    cmd_params.time_decay         = false;
    cmd_params.landmark           = 1000;
    cmd_params.clean_interval     = 5000;

    cmd_params.input_file  = filesystem::current_path().generic_string() + "/datasets/EDS.txt";
    cmd_params.output_file = "results.txt";
    cmd_params.algo        = SESAME::G13Stream;

    vector<SESAME::PointPtr> input;
    vector<SESAME::PointPtr> results;

    // Create Spout.
    SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(cmd_params);
    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(cmd_params, sourcePtr);

    // Create Sink.
    SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(cmd_params);

    // Create Algorithm.
    AlgorithmPtr algoPtr = GenericFactory::New<
        StreamClustering<Landmark, ClusteringFeaturesTree, NoDetection<true, true>, NoRefinement>>(
        cmd_params);

    // Run algorithm producing results.
    auto res = BenchmarkUtils::runBenchmark(cmd_params, sourcePtr, sinkPtr, algoPtr);

    ASSERT_NEAR(res->purity, 0.8344, 0.01);
}
