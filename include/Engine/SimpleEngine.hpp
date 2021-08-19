// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 19/07/2021.
//

#ifndef SESAME_INCLUDE_Engine_SINGLETHREADENGINE_H_
#define SESAME_INCLUDE_Engine_SINGLETHREADENGINE_H_
#include <Sources/DataSource.hpp>
#include <Sinks/DataSink.hpp>
#include <Algorithm/Algorithm.hpp>
#include <Engine/Engine.hpp>

using namespace std;

namespace SESAME {

/**
 * The SingleThreadEngine will spawn one thread for source, sink and algorithm.
 * TODO: allow to pass in multiple data sources and multiple data sinks.
 */
class SimpleEngine : SESAME::Engine {
 private:
  DataSourcePtr sourcePtr;
  DataSinkPtr sinkPtr;
  AlgorithmPtr algoPtr;
  SingleThreadPtr threadPtr;//SimpleEngine has only one thread to run algorithm.
  atomic_int threadID;
 public:
  BarrierPtr barrierPtr;
  SimpleEngine(DataSourcePtr sourcePtr, DataSinkPtr sinkPtr, AlgorithmPtr algoPtr);
  //  void createBarrier();
  void run();//start the engine.
  void runningRoutine(DataSourcePtr sourcePtr, DataSinkPtr sinkPtr, AlgorithmPtr algoPtr);
  bool start(BarrierPtr barrierPtr,
             DataSourcePtr sourcePtr,
             DataSinkPtr sinkPtr,
             AlgorithmPtr algoPtr,
             int id);//start the algorithm thread.
  bool stop();
  int assignID();
};
}
#endif //SESAME_INCLUDE_Engine_SINGLETHREADENGINE_H_
