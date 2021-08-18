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

using namespace std;

namespace SESAME {
class DataSource;
typedef std::shared_ptr<DataSource> DataSourcePtr;

class DataSource {
 private:
  std::vector<PointPtr> input;
  std::shared_ptr<rigtorp::SPSCQueue<PointPtr>> inputQueue;
  ThreadPtr threadPtr;
  BarrierPtr barrierPtr;
 public:
  void load(int point_number, int dimension, vector <string> input);
  bool empty();
  PointPtr get();
  DataSource();
  ~DataSource();
  void runningRoutine();
  bool start();
  bool stop();
  void setBarrier(BarrierPtr barrierPtr);
  std::vector<PointPtr> getInputs();

};
}
#endif //SESAME_SRC_SOURCES_DATASOURCE_HPP_
