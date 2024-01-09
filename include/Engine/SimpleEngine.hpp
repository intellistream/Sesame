// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 19/07/2021.
//

#ifndef SESAME_INCLUDE_Engine_SINGLETHREADENGINE_H_
#define SESAME_INCLUDE_Engine_SINGLETHREADENGINE_H_
#include <Algorithm/Algorithm.hpp>
#include <Engine/Engine.hpp>
#include <Sinks/DataSink.hpp>
#include <Sources/DataSource.hpp>

using namespace std;

namespace SESAME
{

/**
 * The SingleThreadEngine will spawn one thread for source, sink and algorithm.
 * TODO: allow to pass in multiple data sources and multiple data sinks.
 */
class SimpleEngine : SESAME::Engine
{
private:
    DataSourcePtr sourcePtr;
    DataSinkPtr sinkPtr;
    AlgorithmPtr algoPtr;
    SingleThreadPtr threadPtr;  // SimpleEngine has only one thread to run algorithm.
    atomic_int threadID;
    TimeMeter overallMeter;
    int TMA_level;
    int TMA_metric;
    int PAPI_interval;

public:
    BarrierPtr barrierPtr;
    SimpleEngine(DataSourcePtr sourcePtr, DataSinkPtr sinkPtr, AlgorithmPtr algoPtr, param_t &param);
    //  void createBarrier();
    void run();  // start the engine.
    void runningRoutine(DataSourcePtr sourcePtr, DataSinkPtr sinkPtr, AlgorithmPtr algoPtr);
    bool start(DataSourcePtr sourcePtr, DataSinkPtr sinkPtr, AlgorithmPtr algoPtr,
               int id);  // start the algorithm thread.
    bool stop();
    int assignID();
    void printTime();
};
}  // namespace SESAME
#endif  // SESAME_INCLUDE_Engine_SINGLETHREADENGINE_H_
