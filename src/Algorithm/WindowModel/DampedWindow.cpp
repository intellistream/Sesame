//
// Created by 1124a on 2021/8/19.
//
#include <Algorithm/WindowModel/DampedWindow.hpp>
SESAME::DampedWindow::DampedWindow(double base, double lambda){
  this->base=base;
  this->lambda=lambda;
}

double SESAME::DampedWindow::decayFunction(clock_t startTime, clock_t currentTimestamp) const
{
  double elapsedTime = (int) ((currentTimestamp - startTime) / CLOCKS_PER_SEC);
  return pow(this->base, -1 * this->lambda * elapsedTime);
}
