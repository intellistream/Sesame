// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

#ifndef SESAME_SRC_TIMER_CYCLEMETER_HPP_
#define SESAME_SRC_TIMER_CYCLEMETER_HPP_

#include <Timer/rdtsc.hpp>/* startTimer, stopTimer */
#include <ctime>           /* gettimeofday */
#include <cstddef>
#include <cstdint>

namespace SESAME {
class TimeMeter {
 private:
  struct timespec start, stop;
 public:
  void START_MEASURE();
  void END_MEASURE();
  long MeterUSEC();//return the meter result in micro second unit.
};
}

#endif //SESAME_SRC_TIMER_CYCLEMETER_HPP_
