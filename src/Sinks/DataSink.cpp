// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 20/07/2021.
//

#include <Sinks/DataSink.hpp>

SESAME::DataSink::DataSink() {
  outputQueue = std::make_shared<rigtorp::SPSCQueue<PointPtr>>
      (1000);//TODO: remove hard-coded queue initialization.
  threadPtr = std::make_shared<SingleThread>();
  finished = false;
}
void SESAME::DataSink::runningRoutine() {
  while (!finished) {
    if (!outputQueue->empty()) {
      PointPtr result = *outputQueue->front();
      output.push_back(result);
      outputQueue->pop();
    }
  }
  SESAME_INFO("DataSink finished grab data, in total:" << output.size());
  barrierPtr->arrive_and_wait();
}
bool SESAME::DataSink::start(int id) {
  auto fun = [this]() {
    barrierPtr->arrive_and_wait();
    runningRoutine();
  };
  threadPtr->construct(fun, id);
  SESAME_INFO("DataSink spawn thread=" << threadPtr->getID());
  return true;
}

bool SESAME::DataSink::stop() {
  if (threadPtr) {
    SESAME_INFO("DataSink::stop try to join threads=" << threadPtr->getID());
    threadPtr->join();
    threadPtr.reset();
  } else {
    SESAME_INFO("DataSink " << ": Thread is not joinable" << threadPtr->getID());
    return false;
  }
  return true;
}
void SESAME::DataSink::put(SESAME::PointPtr resultPtr) {
  outputQueue->push(resultPtr);
}
std::vector<SESAME::PointPtr> SESAME::DataSink::getResults() {
  return output;
}
void SESAME::DataSink::setBarrier(SESAME::BarrierPtr barrierPtr) {
  this->barrierPtr = barrierPtr;
}
SESAME::DataSink::~DataSink() {
  stop();
}
void SESAME::DataSink::finish() {
  this->finished = true;
}
