// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 20/07/2021.
//

#ifndef SESAME_INCLUDE_SINKS_DATASINK_HPP_
#define SESAME_INCLUDE_SINKS_DATASINK_HPP_

#include "Algorithm/DataStructure/Point.hpp"
#include "Utils/SPSCQueue.hpp"
#include "Utils/UtilityFunctions.hpp"
#include "Utils/Logger.hpp"
#include "Engine/SingleThread.hpp"

#include <string>
#include <vector>
#include <queue>

namespace SESAME {
class DataSink;
typedef std::shared_ptr<DataSink> DataSinkPtr;

class DataSink {
 private:
  std::vector<PointPtr> output;
  std::shared_ptr<std::queue<PointPtr>> outputQueue;
  SingleThreadPtr threadPtr;
  std::atomic_bool sourceEnd;
  std::atomic_bool finished;
  BarrierPtr barrierPtr;
 public:
  DataSink();
  ~DataSink();
  void put(PointPtr resultPtr);
  void runningRoutine();
  bool start(int id);
  bool stop();
  void Ended();
  bool isFinished();
  std::vector<PointPtr> getResults();
  void setBarrier(BarrierPtr barrierPtr);
};
}
#endif //SESAME_INCLUDE_SINKS_DATASINK_HPP_
