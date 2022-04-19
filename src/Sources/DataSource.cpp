// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 20/07/2021.
//

#include "Sources/DataSource.hpp"
#include "Algorithm/DataStructure/DataStructureFactory.hpp"
#include "Utils/Logger.hpp"
#include "Utils/UtilityFunctions.hpp"

#include <cassert>
#include <chrono>
#include <thread>
#include <vector>

using namespace std::chrono;

/**
 * Create input data points.
 * TODO: Remove the hard-coded part.
 * @param point_number
 * @param dim
 * @param input
 * @return
 */
void SESAME::DataSource::load(int point_number, int dim,
                              vector<string> input) {

  // The step used to generate random timestamps
  const int timeStep = 100000;
  for (int i = 0; i < point_number; i++) {
    int timeStamp = timeStep * i + rand() % timeStep;
    PointPtr point = DataStructureFactory::createPoint(
        i, DEFAULT_WEIGHT, dim, DEFAULT_COST, timeStamp);
    char *charData = new char[INT32_MAX];
    strcpy(charData, input[i].c_str());
    // use c_str() to convert string to char * but it's just a temp pointer we
    // have to use strcpy to store it
    const char *sep = " ";
    char *feature =
        strtok(charData, sep);      // TODO: why this?? Read token from charData
    feature = strtok(nullptr, sep); // Skip the first token (index number)
    int index = 0;
    while (feature != nullptr) {
      if (index == dim) {
        point->setClusteringCenter(atoi(feature));
      } else {
        point->setFeatureItem(strtod(feature, nullptr), index);
        index++;
      }
      feature = strtok(nullptr, sep);
    }
    this->input.push_back(point);
  }
}

SESAME::DataSource::DataSource() {
  inputQueue = std::make_shared<rigtorp::SPSCQueue<PointPtr>>(DEFAULT_QUEUE_CAPACITY);//TODO: remove hard-coded queue initialization: done
  threadPtr = std::make_shared<SingleThread>();
  sourceEnd = false;
}

// TODO: we can control the source speed here: done
void SESAME::DataSource::runningRoutine() {
  barrierPtr->arrive_and_wait();
  overallMeter.START_MEASURE();
  // Initialize timer at time 0
  auto start = high_resolution_clock::now();
  SESAME_INFO("DataSource start to emit data");
  for (PointPtr p : this->input) {
    int timestamp = p->getTimeStamp();
    auto now = high_resolution_clock::now();
    if(timestamp > duration_cast<nanoseconds>(now - start).count()) {
      std::this_thread::sleep_for(nanoseconds(timestamp - duration_cast<nanoseconds>(now - start).count()));
    }
    // Wait until (currentTime - startTime) >= timestamp to push point
    inputQueue->push(p->copy());
  }
  SESAME_INFO("sourceEnd set to true");
  sourceEnd = true; // Let engine knows that there won't be any more data
                    // coming.
  // barrierPtr->arrive_and_wait();
  overallMeter.END_MEASURE();
  SESAME_INFO("DataSource sourceEnd emit data");
  printTime();
}

bool SESAME::DataSource::start(int id) {
  auto fun = [this]() { runningRoutine(); };
  threadPtr->construct(fun, id);
  SESAME_INFO("DataSource spawn thread=" << threadPtr->getID());
  return true;
}
bool SESAME::DataSource::stop() {
  if (threadPtr) {
    SESAME_INFO("DataSource::stop try to join threads=" << threadPtr->getID());
    threadPtr->join();
    threadPtr.reset();
  } else {
    SESAME_INFO("DataSource "
                << ": Thread is not joinable");
    return false;
  }
  return true;
}
bool SESAME::DataSource::empty() { return inputQueue->empty(); }

SESAME::PointPtr SESAME::DataSource::get() {
  assert(inputQueue->size());
  auto rt = *inputQueue->front();
  inputQueue->pop();
  return rt;
}

void SESAME::DataSource::setBarrier(SESAME::BarrierPtr barrierPtr) {
  this->barrierPtr = barrierPtr;
}
SESAME::DataSource::~DataSource() { stop(); }
vector<SESAME::PointPtr> SESAME::DataSource::getInputs() { return input; }
void SESAME::DataSource::printTime() {
  SESAME_INFO("DataSource takes " << overallMeter.MeterUSEC()
                                  << " useconds to finish.");
}
bool SESAME::DataSource::sourceEnded() { return sourceEnd; }
