// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 20/07/2021.
//

#ifndef SESAME_INCLUDE_SINKS_DATASINK_HPP_
#define SESAME_INCLUDE_SINKS_DATASINK_HPP_
#include <string>
#include <vector>
#include <Algorithm/DataStructure/Point.hpp>
#include <Utils/SPSCQueue.hpp>
#include <Utils/UtilityFunctions.hpp>
#include <Utils/Logger.hpp>
#include <Engine/SingleThread.hpp>
namespace SESAME {
class DataSink;
typedef std::shared_ptr<DataSink> DataSinkPtr;

class DataSink {
 private:
  std::vector<PointPtr> output;
  std::shared_ptr<rigtorp::SPSCQueue<PointPtr>> outputQueue;
  SingleThreadPtr threadPtr;
  std::atomic_bool finished;
  BarrierPtr barrierPtr;
 public:
  DataSink();
  ~DataSink();
  void put(PointPtr resultPtr);
  void runningRoutine();
  bool start(int id);
  bool stop();
  void finish();
  bool isFinished();
  std::vector<PointPtr> getResults();
  void setBarrier(BarrierPtr barrierPtr);
};
}
#endif //SESAME_INCLUDE_SINKS_DATASINK_HPP_
