// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

#include <Timer/TimeMeter.hpp>
void SESAME::TimeMeter::START_MEASURE() {
  clock_gettime(CLOCK_REALTIME, &start);
}
void SESAME::TimeMeter::END_MEASURE() {
  clock_gettime(CLOCK_REALTIME, &stop);
}
long SESAME::TimeMeter::MeterUSEC() {
  return (((this->stop).tv_sec * 1000000L + (this->stop).tv_nsec / 1000L)
      - ((this->start).tv_sec * 1000000L + (this->start).tv_nsec / 1000L));
}
