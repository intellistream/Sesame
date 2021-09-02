// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 20/07/2021.
//

#ifndef SESAME_SRC_SOURCES_DATASOURCE_HPP_
#define SESAME_SRC_SOURCES_DATASOURCE_HPP_

#include <Algorithm/DataStructure/Point.hpp>
#include <string>
#include <cstring>
#include <vector>
#include <Utils/UtilityFunctions.hpp>
#include <Utils/SPSCQueue.hpp>
#include <Engine/SingleThread.hpp>
#include <Timer/TimeMeter.hpp>

using namespace std;

namespace SESAME {
class DataSource;
typedef std::shared_ptr<DataSource> DataSourcePtr;

class DataSource {
 private:
  std::vector<PointPtr> input;
  std::shared_ptr<rigtorp::SPSCQueue<PointPtr>> inputQueue;
  SingleThreadPtr threadPtr;
  BarrierPtr barrierPtr;
  TimeMeter overallMeter;
  bool sourceEnd;
 public:
  void load(int point_number, int dimension, vector<string> input);
  bool empty();
  PointPtr get();
  std::vector<PointPtr> getInputs();
  DataSource();
  ~DataSource();
  void runningRoutine();
  bool start(int i);
  bool stop();
  void setBarrier(BarrierPtr barrierPtr);
  void printTime();
  bool sourceEnded();
};
}
#endif //SESAME_SRC_SOURCES_DATASOURCE_HPP_
