//
// Created by 1124a on 2021/8/19.
//

#ifndef SESAME_INCLUDE_ALGORITHM_WINDOWMODEL_DAMPEDWINDOW_HPP_
#define SESAME_INCLUDE_ALGORITHM_WINDOWMODEL_DAMPEDWINDOW_HPP_
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <memory>
#include <vector>
#include <Algorithm/WindowModel/WindowModel.hpp>
#include <Timer/TimeMeter.hpp>
namespace SESAME {
class DampedWindow;
typedef std::shared_ptr<DampedWindow> DampedWindowPtr;
class DampedWindow : WindowModel {
 public:
  double base;
  double lambda;
  DampedWindow(double base, double lambda);
  long decayFunction(timespec startTime, timespec currentTimestamp) const;
  double decayFunction(int startTime, int currentTimestamp) const;
};
}
#endif //SESAME_INCLUDE_ALGORITHM_WINDOWMODEL_DAMPEDWINDOW_HPP_
