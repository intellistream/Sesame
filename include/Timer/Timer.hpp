#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <chrono>

struct Timer {
  using clock_t = std::chrono::_V2::system_clock::time_point;
  clock_t start;
  int64_t sum = 0;
  void Tick() { start = std::chrono::high_resolution_clock::now(); }
  void Tock() {
    auto end = std::chrono::high_resolution_clock::now();
    sum += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
               .count();
  }
};

#endif
