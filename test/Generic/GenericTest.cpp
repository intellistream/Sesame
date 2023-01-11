// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by wzru on 2022/3/24.
//

#include <filesystem>

#include "Algorithm/DataStructure/CFTree.hpp"
#include "Algorithm/DataStructure/GenericFactory.hpp"
#include "Algorithm/DataStructure/MeyersonSketch.hpp"
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
    param_t param;
    param.num_points                 = 3000;
    param.distance_threshold         = 100;
    param.max_in_nodes               = 10;
    param.max_leaf_nodes             = 20;
    param.num_clusters               = 7;
    param.dim                        = 54;
    param.seed                       = 10;
    param.num_clusters               = 7;
    param.time_decay                 = false;
    param.landmark                   = 1000;
    param.outlier_distance_threshold = 5000;
    param.outlier_cap                = 10;
    param.k                          = 100;

    param.input_file  = filesystem::current_path().generic_string() + "/datasets/CoverType.txt";
    param.output_file = "results.txt";
    param.algo        = G1Stream;
    param.run_offline = true;

    vector<PointPtr> input;
    vector<PointPtr> results;

    // Create Spout.
    DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(param);
    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(param, sourcePtr);

    // Create Sink.
    DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(param);

    // Create Algorithm.
    AlgorithmPtr algoPtr = GenericFactory::New<
        StreamClustering<Landmark, ClusteringFeaturesTree, DistanceDetection<true, false>, KMeans>>(
        param);

    // Run algorithm producing results.
    auto res = BenchmarkUtils::runBenchmark(param, sourcePtr, sinkPtr, algoPtr);

    ASSERT_NEAR(res->purity, 0.6627, 0.03);
    ASSERT_NEAR(res->cmm, 0.9361, 0.03);
}

TEST(GenericTest, V2)
{
    // Parse parameters.
    param_t param;
    param.num_points                 = 3000;
    param.distance_threshold         = 100;
    param.max_in_nodes               = 10;
    param.max_leaf_nodes             = 20;
    param.dim                        = 54;
    param.num_clusters               = 7;
    param.time_decay                 = false;
    param.min_points                 = 10;
    param.landmark                   = 1000;
    param.epsilon                    = 200;  // 0.1
    param.run_offline                = true;
    param.outlier_distance_threshold = 5000;
    param.outlier_cap                = 10;

    param.input_file  = filesystem::current_path().generic_string() + "/datasets/CoverType.txt";
    param.output_file = "results.txt";
    param.algo        = SESAME::G2Stream;

    vector<SESAME::PointPtr> input;
    vector<SESAME::PointPtr> results;

    // Create Spout.
    SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(param);
    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(param, sourcePtr);

    // Create Sink.
    SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(param);

    // Create Algorithm.
    AlgorithmPtr algoPtr = GenericFactory::New<
        StreamClustering<Landmark, ClusteringFeaturesTree, DistanceDetection<true, false>, DBSCAN>>(
        param);

    // Run algorithm producing results.
    auto res = BenchmarkUtils::runBenchmark(param, sourcePtr, sinkPtr, algoPtr);

    ASSERT_NEAR(res->purity, 0.7303, 0.02);
    ASSERT_NEAR(res->cmm, 0.9632, 0.02);
}

TEST(GenericTest, V3)
{
    // Parse parameters.
    param_t param;
    param.num_points                 = 45690;
    param.distance_threshold         = 50;
    param.max_in_nodes               = 100;
    param.max_leaf_nodes             = 100;
    param.dim                        = 2;
    param.num_clusters               = 75;
    param.time_decay                 = true;
    param.landmark                   = 1000;
    param.outlier_distance_threshold = 500;
    param.outlier_cap                = 2000;

    param.input_file  = filesystem::current_path().generic_string() + "/datasets/EDS.txt";
    param.output_file = "results.txt";
    param.algo        = SESAME::G3Stream;

    vector<SESAME::PointPtr> input;
    vector<SESAME::PointPtr> results;

    // Create Spout.
    SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(param);
    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(param, sourcePtr);

    // Create Sink.
    SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(param);

    // Create Algorithm.
    AlgorithmPtr algoPtr =
        GenericFactory::New<StreamClustering<Landmark, ClusteringFeaturesTree,
                                             DistanceDetection<true, false>, NoRefinement>>(param);

    // Run algorithm producing results.
    auto res = BenchmarkUtils::runBenchmark(param, sourcePtr, sinkPtr, algoPtr);

    ASSERT_NEAR(res->purity, 0.8032, 0.02);
}

TEST(GenericTest, V4)
{
    // Parse parameters.
    param_t param;
    param.num_points                 = 3000;
    param.distance_threshold         = 100;
    param.max_in_nodes               = 20;
    param.max_leaf_nodes             = 40;
    param.dim                        = 54;
    param.num_clusters               = 7;
    param.time_decay                 = false;
    param.sliding                    = 1000;
    param.outlier_distance_threshold = 3000;
    param.outlier_cap                = 3;

    param.input_file  = filesystem::current_path().generic_string() + "/datasets/CoverType.txt";
    param.output_file = "results.txt";
    param.algo        = SESAME::G4Stream;

    vector<SESAME::PointPtr> input;
    vector<SESAME::PointPtr> results;

    // Create Spout.
    SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(param);
    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(param, sourcePtr);

    // Create Sink.
    SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(param);

    // Create Algorithm.
    AlgorithmPtr algoPtr =
        GenericFactory::New<StreamClustering<Sliding, ClusteringFeaturesTree,
                                             DistanceDetection<true, false>, NoRefinement>>(param);

    // Run algorithm producing results.
    auto res = BenchmarkUtils::runBenchmark(param, sourcePtr, sinkPtr, algoPtr);

    ASSERT_NEAR(res->purity, 0.64, 0.02);
}

TEST(GenericTest, V5)
{
    // Parse parameters.
    param_t param;
    param.num_points                 = 3000;
    param.distance_threshold         = 1000;
    param.max_in_nodes               = 20;
    param.max_leaf_nodes             = 40;
    param.dim                        = 54;
    param.num_clusters               = 7;
    param.time_decay                 = false;
    param.lambda                     = 1.25;
    param.alpha                      = 2;
    param.outlier_distance_threshold = 500;
    param.outlier_cap                = 100;

    param.input_file  = filesystem::current_path().generic_string() + "/datasets/CoverType.txt";
    param.output_file = "results.txt";
    param.algo        = SESAME::G5Stream;

    vector<SESAME::PointPtr> input;
    vector<SESAME::PointPtr> results;

    // Create Spout.
    SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(param);
    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(param, sourcePtr);

    // Create Sink.
    SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(param);

    // Create Algorithm.
    AlgorithmPtr algoPtr =
        GenericFactory::New<StreamClustering<Damped, ClusteringFeaturesTree,
                                             DistanceDetection<true, false>, NoRefinement>>(param);

    // Run algorithm producing results.
    auto res = BenchmarkUtils::runBenchmark(param, sourcePtr, sinkPtr, algoPtr);

    ASSERT_NEAR(res->purity, 0.2987, 0.03);
    ASSERT_NEAR(res->cmm, 0.6909, 0.03);
}

TEST(GenericTest, V6)
{
    // Parse parameters.
    param_t param;

    param.num_points         = 10000;
    param.distance_threshold = 10;
    param.max_in_nodes       = 100;
    param.max_leaf_nodes     = 50;
    param.dim                = 2;
    param.num_clusters       = 363;
    param.time_decay         = false;
    param.landmark           = 1000;

    param.input_file  = filesystem::current_path().generic_string() + "/datasets/EDS.txt";
    param.output_file = "results.txt";
    param.algo        = SESAME::G6Stream;

    vector<SESAME::PointPtr> input;
    vector<SESAME::PointPtr> results;

    // Create Spout.
    SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(param);
    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(param, sourcePtr);

    // Create Sink.
    SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(param);

    // Create Algorithm.
    AlgorithmPtr algoPtr = GenericFactory::New<
        StreamClustering<Landmark, ClusteringFeaturesTree, NoDetection<true, false>, NoRefinement>>(
        param);

    // Run algorithm producing results.
    auto res = BenchmarkUtils::runBenchmark(param, sourcePtr, sinkPtr, algoPtr);

    ASSERT_NEAR(res->purity, 0.8251, 0.03);
    ASSERT_NEAR(res->cmm, 0.8571, 0.03);
}

TEST(GenericTest, V7)
{
    // Parse parameters.
    param_t param;
    param.num_points                = 3000;
    param.distance_threshold        = 1000;
    param.max_in_nodes              = 20;
    param.max_leaf_nodes            = 40;
    param.dim                       = 54;
    param.num_clusters              = 7;
    param.time_decay                = false;
    param.landmark                  = 1000;
    param.neighbor_distance         = 5000;
    param.outlier_density_threshold = 0.5;
    param.outlier_cap               = 10;

    param.input_file  = filesystem::current_path().generic_string() + "/datasets/CoverType.txt";
    param.output_file = "results.txt";
    param.algo        = SESAME::G7Stream;

    vector<SESAME::PointPtr> input;
    vector<SESAME::PointPtr> results;

    // Create Spout.
    SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(param);
    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(param, sourcePtr);

    // Create Sink.
    SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(param);

    // Create Algorithm.
    AlgorithmPtr algoPtr =
        GenericFactory::New<StreamClustering<Landmark, ClusteringFeaturesTree,
                                             DensityDetection<true, false>, NoRefinement>>(param);

    // Run algorithm producing results.
    auto res = BenchmarkUtils::runBenchmark(param, sourcePtr, sinkPtr, algoPtr);

    ASSERT_NEAR(res->purity, 0.432, 0.02);
}

TEST(GenericTest, V8)
{
    // Parse parameters.
    param_t param;
    param.num_points                 = 45690;
    param.distance_threshold         = 50;
    param.max_in_nodes               = 100;
    param.max_leaf_nodes             = 100;
    param.dim                        = 2;
    param.num_clusters               = 75;
    param.time_decay                 = true;
    param.landmark                   = 1000;
    param.run_offline                = false;
    param.outlier_distance_threshold = 600;
    param.outlier_cap                = 2000;

    param.input_file  = filesystem::current_path().generic_string() + "/datasets/EDS.txt";
    param.output_file = "results.txt";
    param.algo        = SESAME::V8Stream;

    vector<SESAME::PointPtr> input;
    vector<SESAME::PointPtr> results;

    // Create Spout.
    SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(param);
    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(param, sourcePtr);

    // Create Sink.
    SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(param);

    // Create Algorithm.
    AlgorithmPtr algoPtr =
        GenericFactory::New<StreamClustering<Landmark, ClusteringFeaturesList,
                                             DistanceDetection<true, false>, NoRefinement>>(param);

    // Run algorithm producing results.
    auto res = BenchmarkUtils::runBenchmark(param, sourcePtr, sinkPtr, algoPtr);

    ASSERT_NEAR(res->purity, 0.9962, 0.01);
}

TEST(GenericTest, V11)
{
    // Parse parameters.
    param_t param;

    param.num_points         = 10000;
    param.distance_threshold = 10;
    param.max_in_nodes       = 100;
    param.max_leaf_nodes     = 50;
    param.dim                = 2;
    param.num_clusters       = 363;
    param.time_decay         = false;
    param.landmark           = 1000;
    param.clean_interval     = 9900;

    param.input_file  = filesystem::current_path().generic_string() + "/datasets/EDS.txt";
    param.output_file = "results.txt";
    param.algo        = SESAME::G11Stream;

    vector<SESAME::PointPtr> input;
    vector<SESAME::PointPtr> results;

    // Create Spout.
    SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(param);
    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(param, sourcePtr);

    // Create Sink.
    SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(param);

    // Create Algorithm.
    AlgorithmPtr algoPtr = GenericFactory::New<
        StreamClustering<Landmark, ClusteringFeaturesTree, NoDetection<false, true>, NoRefinement>>(
        param);

    // Run algorithm producing results.
    auto res = BenchmarkUtils::runBenchmark(param, sourcePtr, sinkPtr, algoPtr);

    ASSERT_NEAR(res->purity, 0.82, 0.01);
}

TEST(GenericTest, V12)
{
    // Parse parameters.
    param_t param;

    param.num_points         = 10000;
    param.distance_threshold = 10;
    param.max_in_nodes       = 100;
    param.max_leaf_nodes     = 50;
    param.dim                = 2;
    param.num_clusters       = 363;
    param.time_decay         = false;
    param.landmark           = 1000;

    param.input_file  = filesystem::current_path().generic_string() + "/datasets/EDS.txt";
    param.output_file = "results.txt";
    param.algo        = SESAME::G12Stream;

    vector<SESAME::PointPtr> input;
    vector<SESAME::PointPtr> results;

    // Create Spout.
    SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(param);
    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(param, sourcePtr);

    // Create Sink.
    SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(param);

    // Create Algorithm.
    AlgorithmPtr algoPtr =
        GenericFactory::New<StreamClustering<Landmark, ClusteringFeaturesTree,
                                             NoDetection<false, false>, NoRefinement>>(param);

    // Run algorithm producing results.
    auto res = BenchmarkUtils::runBenchmark(param, sourcePtr, sinkPtr, algoPtr);

    ASSERT_NEAR(res->purity, 0.8228, 0.03);
    ASSERT_NEAR(res->cmm, 0.8554, 0.03);
}

TEST(GenericTest, V13)
{
    // Parse parameters.
    param_t param;

    param.num_points         = 10000;
    param.distance_threshold = 10;
    param.max_in_nodes       = 100;
    param.max_leaf_nodes     = 50;
    param.dim                = 2;
    param.num_clusters       = 363;
    param.time_decay         = false;
    param.landmark           = 1000;
    param.clean_interval     = 5000;

    param.input_file  = filesystem::current_path().generic_string() + "/datasets/EDS.txt";
    param.output_file = "results.txt";
    param.algo        = SESAME::G13Stream;

    vector<SESAME::PointPtr> input;
    vector<SESAME::PointPtr> results;

    // Create Spout.
    SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(param);
    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(param, sourcePtr);

    // Create Sink.
    SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(param);

    // Create Algorithm.
    AlgorithmPtr algoPtr = GenericFactory::New<
        StreamClustering<Landmark, ClusteringFeaturesTree, NoDetection<true, true>, NoRefinement>>(
        param);

    // Run algorithm producing results.
    auto res = BenchmarkUtils::runBenchmark(param, sourcePtr, sinkPtr, algoPtr);

    ASSERT_NEAR(res->purity, 0.8344, 0.01);
}

TEST(GenericTest, V14)
{
    // Parse parameters.
    param_t param;
    param.num_points   = 3000;
    param.num_clusters = 7;
    param.dim          = 54;
    param.num_clusters = 7;
    param.k            = 100;
    param.seed         = 10;
    param.num_samples  = 100;
    param.sliding      = 5;

    param.input_file  = filesystem::current_path().generic_string() + "/datasets/CoverType.txt";
    param.output_file = "results.txt";
    param.algo        = SESAME::G14Stream;

    vector<SESAME::PointPtr> input;
    vector<SESAME::PointPtr> results;

    // Create Spout.
    SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(param);
    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(param, sourcePtr);

    // Create Sink.
    SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(param);

    // Create Algorithm.
    AlgorithmPtr algoPtr = GenericFactory::New<
        StreamClustering<Landmark, MeyersonSketch, NoDetection<true, false>, NoRefinement>>(param);

    // Run algorithm producing results.
    auto res = BenchmarkUtils::runBenchmark(param, sourcePtr, sinkPtr, algoPtr);

    ASSERT_NEAR(res->purity, 0.8113, 0.01);
    ASSERT_NEAR(res->cmm, 0.9842, 0.01);
}
