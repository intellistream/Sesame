// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

#include <Timer/TimeMeter.hpp>
void SESAME::TimeMeter::START_MEASURE() {
  gettimeofday(&this->start, NULL);
}
void SESAME::TimeMeter::END_MEASURE() {
  gettimeofday(&this->end, NULL);
}
long SESAME::TimeMeter::MeterUSEC() {
  return (((this->end).tv_sec * 1000000L + (this->end).tv_usec)
      - ((this->start).tv_sec * 1000000L + (this->start).tv_usec));
}
