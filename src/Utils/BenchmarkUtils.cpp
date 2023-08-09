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

BenchmarkResultPtr BenchmarkUtils::RunBenchmark(param_t &param)
{
    // Create Spout.
    SESAME::DataSourcePtr sourcePtr = GenericFactory::New<DataSource>(param);

    // Directly load data from file. TODO: configure it to load from external
    // sensors, e.g., HTTP.
    sourcePtr->load();

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
