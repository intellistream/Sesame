// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 20/07/2021.
//

#include <Sources/DataSource.hpp>
#include <vector>
#include <Algorithm/DataStructure/DataStructureFactory.hpp>
#include <Utils/Logger.hpp>
/**
 * Create input data points.
 * TODO: Remove the hard-coded part.
 * @param point_number
 * @param dimension
 * @param input
 * @return
 */
void SESAME::DataSource::load(int point_number, int dimension, vector<string> input) {

  for (int i = 0; i < point_number; i++) {
    PointPtr point = DataStructureFactory::createPoint(i, 1, dimension, 0);
    char *charData = new char[10000];
    strcpy(charData, input[i].c_str());
    // use c_str() to convert string to char * but it's just a temp pointer we have to use strcpy to store it
    const char *sep = " ";
    char *feature = strtok(charData, sep);//TODO: why this??
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
}

//TODO: we can control the source speed here.
void SESAME::DataSource::runningRoutine() {
  barrierPtr->arrive_and_wait();
  SESAME_INFO("DataSource start to emit data");
  for (PointPtr p: this->input) {
    inputQueue->push(p);
  }
  barrierPtr->arrive_and_wait();
  SESAME_INFO("DataSource sourceEnd emit data");
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
    SESAME_INFO("DataSource::stop try to join threads="<< threadPtr->getID());
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

