// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

#include "Utils/BenchmarkUtils.hpp"
#include "Algorithm/AlgorithmFactory.hpp"
#include "Algorithm/DataStructure/GenericFactory.hpp"
#include "Engine/SimpleEngine.hpp"
#include "Evaluation/Evaluation.hpp"
#include "Evaluation/Purity.hpp"
#include "Sinks/DataSink.hpp"
#include "Sources/DataSource.hpp"
#include "Utils/Logger.hpp"
#include "Utils/UtilityFunctions.hpp"

#include <filesystem>

using namespace std;
using namespace SESAME;
using namespace std::filesystem;

/**
 * @Description: Set the default algorithm StreamKM++ and the default parameters
 * to run the algorithm void -p 1000 -c 10 -d 54 -s 100 -S 10
 * @Param: param: param_t &
 * @Return:
 */
void BenchmarkUtils::defaultParam(param_t &param)
{
    param.num_points = 542;  // number of the data points in the dataset, use
                             // the whole dataset to run benchmark
    param.seed                = 1;
    param.num_clusters        = 2;
    param.dim                 = 54;
    param.coreset_size        = 100;
    param.num_last_arr        = 60;
    param.time_window         = 6;
    param.time_interval       = 100;
    param.num_online_clusters = 10;
    param.radius              = 2;
    param.buf_size            = 500;
    param.offline_time_window = 0;
    param.max_leaf_nodes      = 3;
    param.max_in_nodes        = 3;
    param.distance_threshold  = 3550;
    param.min_points          = 10;
    param.epsilon             = 50;
    param.base                = 2;
    param.lambda              = 1.8;  // 1.8
    param.mu                  = 7;
    param.beta                = 5;  // 5

    // EDMStream
    param.alpha     = 0.998;
    param.num_cache = 100;
    param.radius    = 0.1;
    param.lambda    = 1;
    param.delta     = 10;
    param.beta      = 0.0021;
    param.opt       = 2;

    param.dataset_option = 0;
    param.input_file = std::filesystem::current_path().generic_string() + "/datasets/CoverType.txt";
    param.output_file = "results.txt";
    if (param.algo == V1Stream || param.algo == V2Stream || param.algo == G1Stream ||
        param.algo == G2Stream || param.algo == DenStreamType || param.algo == CluStreamType ||
        param.algo == StreamKMeansType || param.algo == SLKMeansType ||
        param.algo == DBStreamType || param.algo == DStreamType)
        param.run_offline = true;
    param.detect_outlier = false;
    if (param.algo == G2Stream)
    {
        param.run_group = false;
    }
    if (param.algo == BirchType)
    {
        param.outlier_cap = numeric_limits<int>::min();
    }
}

/**
 * @Description: load data from the given dataset and convert the data format
 * into the given point data structure
 * @Param: param: the dataset attribute received from the command line such
 * as p,c,d,s...
 * @Param: input: the vector to store the point list from the dataset
 * @Return: void
 */
void BenchmarkUtils::loadData(param_t &param, SESAME::DataSourcePtr dataSourcePtr)
{
    // Pass input file as a string to DataSource.
    std::vector<std::string> data;
    ifstream infile;
    infile.open(param.input_file);
    if (infile.is_open() == 0)
    {
        std::cerr << "input file not found" << std::endl;
        exit(1);
    }
    SESAME_INFO("Read from the file...");

    // insert the data once per line into the string vector, every string element
    // represents a data line
    for (int i = 0; i < param.num_points; i++)
    {
        data.emplace_back();
        getline(infile, data[i]);
    }
    SESAME_INFO("Complete reading from the file...");
    infile.close();

    // convert the string format into point vector, every string represents a
    // point
    dataSourcePtr->load(param.num_points, param.dim, data);
    SESAME_INFO("Finished loading input data");
}
BenchmarkResultPtr BenchmarkUtils::runBenchmark(param_t &param, SESAME::DataSourcePtr sourcePtr,
                                                SESAME::DataSinkPtr sinkPtr,
                                                SESAME::AlgorithmPtr algoPtr)
{
    SESAME::SimpleEngine engine(sourcePtr, sinkPtr,
                                algoPtr);  // TODO: create multithread engine in future.

    engine.run();

    while (!sinkPtr->isFinished()) usleep(100);
    // wait for sink to stop.

    std::vector<SESAME::PointPtr> inputs  = sourcePtr->getInputs();
    std::vector<SESAME::PointPtr> results = sinkPtr->getResults();
    std::vector<SESAME::PointPtr> predicts;
    // the output clusterID start from 0
    if (param.run_eval)
    {
        if (param.run_group)
        {
            SESAME::UtilityFunctions::groupByCenters(inputs, results, predicts, param.dim);
        }
        else
        {
            predicts = results;
        }
    }

    param.num_res = results.size();
    std::cerr << "results.size=" << results.size() << std::endl;

    // Store results.
    if (param.store)
    {
        algoPtr->Store(param.output_file, param.dim, sinkPtr->getResults());
    }

    BenchmarkResultPtr res;
    if (param.run_eval && results.size())
    {
        res = SESAME::Evaluation::Evaluate(param, inputs, predicts);
    }
    else
    {
        std::cerr << "no need to eval" << std::endl;
        res = GenericFactory::New<BenchmarkResult>(0.0, 0.0);
    }

    engine.stop();

    res->num_res = param.num_res;
    return res;
}
