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
BenchmarkResultPtr BenchmarkUtils::runBenchmark(param_t &param)
{
    // Create Spout.
    SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(param);

    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    BenchmarkUtils::loadData(param, sourcePtr);

    // Create Sink.
    SESAME::DataSinkPtr sinkPtr = GenericFactory::New<DataSink>(param);

    // Create Algorithm.
    SESAME::AlgorithmPtr algoPtr = SESAME::AlgorithmFactory::create(param);

    param.Print();

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
        SESAME::UtilityFunctions::groupByCenters(inputs, results, predicts, param.dim);
    }

    param.num_res = results.size();
    std::cerr << "results.size=" << results.size() << std::endl;

    // Store results.
    if (param.store)
    {
        algoPtr->Store(param.output_file, param.dim, sinkPtr->getResults());
    }

    BenchmarkResultPtr res;
    if (param.run_eval && results.size() && results.size() <= 40000)
    {
        res = SESAME::Evaluation::Evaluate(param, inputs, predicts);
    }
    else
    {
        std::cerr << "skip evaluation" << std::endl;
        res = GenericFactory::New<BenchmarkResult>(0.0, 0.0);
    }

    engine.stop();

    res->num_res = param.num_res;

    res->Print();

    return res;
}
