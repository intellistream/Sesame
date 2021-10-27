// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 20/07/2021.
//

#include <Sources/DataSource.hpp>
#include <vector>
#include <Algorithm/DataStructure/DataStructureFactory.hpp>
#include <Utils/Logger.hpp>
#include <thread>
#include <chrono>
using namespace std::chrono;
typedef time_point<system_clock, nanoseconds> Timer;

/**
 * Create input data points.
 * TODO: Remove the hard-coded part.
 * @param point_number
 * @param dimension
 * @param input
 * @return
 */
void SESAME::DataSource::load(int point_number, int dimension, vector<string> input) {

  // The step used to generate random timestamps
  const int timeStep = 10;
  for (int i = 0; i < point_number; i++) {
    int timeStamp = timeStep * i + rand() % timeStep;
    PointPtr point = DataStructureFactory::createPoint(i, 1, dimension, 0, timeStamp);
    char *charData = new char[10000];
    // Put input[i] into charData
    strcpy(charData, input[i].c_str());
    // use c_str() to convert string to char * but it's just a temp pointer we have to use strcpy to store it
    const char *sep = " ";
    // Split string into tokens, separated by " "
    char *feature = strtok(charData, sep);//TODO: why this??
    // Skip the first feature???
    feature = strtok(nullptr, sep);
    int index = 0;
    while (feature != nullptr) {
      if (index == dimension) {
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
  inputQueue = std::make_shared<rigtorp::SPSCQueue<PointPtr>>(1000);//TODO: remove hard-coded queue initialization.
  threadPtr = std::make_shared<SingleThread>();
  sourceEnd = false;
}

//TODO: we can control the source speed here.
void SESAME::DataSource::runningRoutine() {
  barrierPtr->arrive_and_wait();
  overallMeter.START_MEASURE();
  // Initialize timer with the timestamp of the first point
  Timer timer = system_clock::time_point(nanoseconds{input[0]->getTimeStamp()});
  SESAME_INFO("DataSource start to emit data");
//  // Push data without considering timestamps
//  for (PointPtr p: this->input) {
//    cout << "PointPtr" << p << endl;
//    inputQueue->push(p);
//  }
  // Increase timer using while loop
  for (PointPtr p : this->input) {
    int timestamp = (*p).getTimeStamp();
    while (timer.time_since_epoch().count() < timestamp) {
      timer += nanoseconds(1);
    }
    inputQueue->push(p);
    cout << "Time to load next point: " << timer.time_since_epoch().count() << endl;
  }
//  // Increase timer using thread timeout
//  for (PointPtr p : this->input) {
//    int timestamp = (*p).getTimeStamp();
//    this_thread::sleep_for(chrono::nanoseconds(timestamp - timer.time_since_epoch().count()));
//    inputQueue->push(p);
//    timer = system_clock::time_point(nanoseconds{timestamp});
//    cout << "Time to load next point: " << timer.time_since_epoch().count() << endl;
//  }
  sourceEnd = true;//Let engine knows that there won't be any more data coming.
  barrierPtr->arrive_and_wait();
  overallMeter.END_MEASURE();
  SESAME_INFO("DataSource sourceEnd emit data");
  printTime();
}

bool SESAME::DataSource::start(int id) {
  auto fun = [this]() {
    runningRoutine();
  };
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
    SESAME_INFO("DataSource " << ": Thread is not joinable");
    return false;
  }
  return true;
}
bool SESAME::DataSource::empty() {
  return inputQueue->empty();
}

SESAME::PointPtr SESAME::DataSource::get() {
  auto rt = *inputQueue->front();
  inputQueue->pop();
  return rt;
}

void SESAME::DataSource::setBarrier(SESAME::BarrierPtr barrierPtr) {
  this->barrierPtr = barrierPtr;
}
SESAME::DataSource::~DataSource() {
  stop();
}
vector<SESAME::PointPtr> SESAME::DataSource::getInputs() {
  return input;
}
void SESAME::DataSource::printTime() {
  SESAME_INFO("DataSource takes " << overallMeter.MeterUSEC() << " useconds to finish.");
}
bool SESAME::DataSource::sourceEnded() {
  return sourceEnd;
}

