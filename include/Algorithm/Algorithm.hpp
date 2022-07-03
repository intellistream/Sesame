// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

//
// Created by tuidan on 2021/7/22.
//

#ifndef SESAME_INCLUDE_ALGORITHM_ALGORITHM_HPP_
#define SESAME_INCLUDE_ALGORITHM_ALGORITHM_HPP_

#include "Algorithm/Param.hpp"
#include "Algorithm/DataStructure/Point.hpp"
#include "Sinks/DataSink.hpp"
#include "Timer/Timer.hpp"

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

using namespace std;

namespace SESAME {

class Algorithm;
typedef std::shared_ptr<Algorithm> AlgorithmPtr;

class Algorithm {
public:
  Algorithm() = default;
  virtual ~Algorithm() = default;
  virtual void Init() = 0;
  virtual void RunOnline(SESAME::PointPtr input) = 0;
  virtual void RunOffline(SESAME::DataSinkPtr ptr) = 0;
  void Store(std::string output_file, int dim, std::vector<PointPtr> results);
  Timer ds_timer, out_timer, ref_timer, sum_timer, lat_timer;
  param_t param;
  void PrintPerf() {
    cout << "ds_us: " << ds_timer.sum / 1000 << endl;
    cout << "out_us: " << out_timer.sum / 1000 << endl;
    cout << "ref_us: " << ref_timer.sum / 1000 << endl;
    auto sum = ds_timer.sum + out_timer.sum + ref_timer.sum;
    cout << "sum_us: " << sum / 1000 << endl;
    assert(param.num_points);
    cout << "lat_us: " << lat_timer.sum / 1e3 / param.num_points << endl;
    cout << "et_s: " << sum_timer.sum / 1e9 << endl;
    cout << "qps: " << param.num_points * 1e9 / sum_timer.sum << endl;
  }
};

} // namespace SESAME

#endif // SESAME_INCLUDE_ALGORITHM_ALGORITHM_HPP_
