// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 19/07/2021.
//

#include <iostream>
#include <utility>
#include <vector>
#include <thread>
#include <Engine/SimpleEngine.hpp>
#include <Utils/SPSCQueue.hpp>
#include <Utils/Logger.hpp>

using namespace std;

SESAME::SimpleEngine::SimpleEngine(DataSourcePtr sourcePtr,
                                   DataSinkPtr sinkPtr,
                                   AlgorithmPtr algoPtr) : threadID(0) {
  this->sourcePtr = std::move(sourcePtr);
  this->sinkPtr = std::move(sinkPtr);
  this->algoPtr = std::move(algoPtr);
  threadPtr = std::make_shared<SingleThread>();
}

void SESAME::SimpleEngine::run() {

  barrierPtr = UtilityFunctions::createBarrier(4);
  this->sourcePtr->setBarrier(barrierPtr);
  this->sinkPtr->setBarrier(barrierPtr);

  //start source thread
  this->sourcePtr->start(assignID());

  //start sink thread
  this->sinkPtr->start(assignID());

  //start engine thread(s) for algorithm.
  this->start(barrierPtr, sourcePtr, sinkPtr, algoPtr, assignID());

}
bool SESAME::SimpleEngine::start(BarrierPtr barrierPtr,
                                 DataSourcePtr sourcePtr,
                                 DataSinkPtr sinkPtr,
                                 AlgorithmPtr algoPtr,
                                 int id) {
  auto fun = [this, barrierPtr, sourcePtr, sinkPtr, algoPtr]() {
    barrierPtr->arrive_and_wait();//wait for source and sink.
    runningRoutine(sourcePtr, sinkPtr, algoPtr);
  };
  threadPtr->construct(fun, id);
  SESAME_DEBUG("Engine spawn thread" << threadPtr->getID());
  return true;
}
/**
 * TODO: this is not generic enough to capture different algorithms.
 * @param sourcePtr
 * @param sinkPtr
 * @param algoPtr
 */
void SESAME::SimpleEngine::runningRoutine(DataSourcePtr sourcePtr,
                                          DataSinkPtr sinkPtr,
                                          AlgorithmPtr algoPtr) {
  //initialization
  algoPtr->Initilize();

  // run online clustering
  while (!sourcePtr->empty()) {
    auto item = sourcePtr->get();
    algoPtr->runOnlineClustering(item);
  }

  // run offline clustering
  algoPtr->runOfflineClustering(sinkPtr);

  SESAME_INFO("Algorithm finished process data");
  sinkPtr->finish();//Let sink knows that there won't be any more data coming.
  // finished execution
  barrierPtr->arrive_and_wait();
}

bool SESAME::SimpleEngine::stop() {
  if (threadPtr) {
    SESAME_DEBUG("Engine::stop try to join threads=" << threadPtr->getID());
    threadPtr->join();
    threadPtr.reset();
  } else {
    SESAME_DEBUG("Engine " << threadPtr->getID() << ": Thread is not joinable");
    return false;
  }
  return true;
}
int SESAME::SimpleEngine::assignID() {
  return threadID++;
}