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
void SESAME::DataSource::load(int point_number, int dimension, vector <string> input) {

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
}

//TODO: we can control the source speed here.
void SESAME::DataSource::runningRoutine() {
  for (PointPtr p: this->input) {
    inputQueue->push(p);
  }
  SESAME_INFO("DataSource finished emit data");
  barrierPtr->arrive_and_wait();
}

bool SESAME::DataSource::start() {
  threadPtr = std::make_shared<std::thread>([this]() {
    barrierPtr->arrive_and_wait();
    runningRoutine();
  });
  SESAME_DEBUG("DataSource spawn thread" << threadPtr->get_id());
  return true;
}
bool SESAME::DataSource::stop() {
  if (threadPtr) {
    SESAME_DEBUG("DataSource::stop try to join threads=" << threadPtr->get_id());
    threadPtr->join();
    threadPtr.reset();
  } else {
    SESAME_DEBUG("DataSource " << threadPtr->get_id() << ": Thread is not joinable");
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

