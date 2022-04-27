// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 20/07/2021.
//

#include "Sinks/DataSink.hpp"

#include <chrono>
#include <thread>

using namespace std::chrono_literals;

SESAME::DataSink::DataSink(const SESAME::param_t &param): param(param) {
  outputQueue = std::make_shared<rigtorp::SPSCQueue<PointPtr>>(DEFAULT_QUEUE_CAPACITY);
  threadPtr = std::make_shared<SingleThread>();
  sourceEnd = false;
}

void SESAME::DataSink::runningRoutine() {
  barrierPtr->arrive_and_wait();
  SESAME_INFO("DataSink start to grab data");
  while (!sourceEnd) {
    while (!outputQueue->empty()) {
      output.push_back(*outputQueue->front());
      outputQueue->pop();
    }
    std::this_thread::sleep_for(1ms);
  }
  while (!outputQueue->empty()) {
    output.push_back(*outputQueue->front());
    outputQueue->pop();
  }
  finished = true;
  barrierPtr->arrive_and_wait();
  SESAME_INFO("DataSink sourceEnd grab data, in total:" << output.size());
}

bool SESAME::DataSink::start(int id) {
  auto fun = [this]() { runningRoutine(); };
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
    SESAME_INFO("DataSink "
                << ": Thread is not joinable" << threadPtr->getID());
    return false;
  }
  return true;
}

void SESAME::DataSink::put(SESAME::PointPtr resultPtr) {
  outputQueue->push(resultPtr);
}

std::vector<SESAME::PointPtr> SESAME::DataSink::getResults() { return output; }

void SESAME::DataSink::setBarrier(SESAME::BarrierPtr barrierPtr) {
  this->barrierPtr = barrierPtr;
}

SESAME::DataSink::~DataSink() { stop(); }

void SESAME::DataSink::Ended() { this->sourceEnd = true; }

bool SESAME::DataSink::isFinished() { return this->finished.load(); }
