//
// Created by Shuhao Zhang on 20/07/2021.
//

#ifndef SESAME_SRC_ALGORITHM_WINDOWMODEL_LANDMARKWINDOW_HPP_
#define SESAME_SRC_ALGORITHM_WINDOWMODEL_LANDMARKWINDOW_HPP_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <memory>
#include <vector>
#include <Algorithm/DataStructure/Point.hpp>
#include <Algorithm/DataStructure/CoresetTree.hpp>
#include <Algorithm/WindowModel/WindowModel.hpp>

namespace SESAME {
class LandmarkWindow;
typedef std::shared_ptr<LandmarkWindow> LandmarkWindowPtr;

class LandmarkWindow : WindowModel {
 public:
/**
DataStructure representing a single window
**/
  struct Window {
    int cursize;
    std::vector<PointPtr> points;
    std::vector<PointPtr> spillover;
  };

/**
datastructure for managing all O(log(n)) windows
**/
  struct WindowManager {
    int numberOfWindow;
    int maxWindowSize;
    std::vector<Window> windows; //    struct Window *windows;
  };

  WindowManager windowManager;

  SESAME::CoresetTreePtr tree;
  /**
   * initialize windows in the window manager.
   * @param dimension
   * @param coresetSize
   */
  void initWindow();

/**
inserts a single point into the bucketmanager
**/
  void insertPoint(PointPtr point);

/**
It may happen that the manager is not full (since n is not always a power of 2). In this case we extract the coreset
from the manager by computing a coreset of all nonempty windows

Case 1: the last bucket is full
=> n is a power of 2 and we return the contents of the last bucket

Case2: the last bucket is not full
=> we compute a coreset of all nonempty windows

this operation should only be called after the streaming process is finished
**/
  std::vector<PointPtr> getCoresetFromManager(std::vector<PointPtr> & coreset); // https://stackoverflow.com/questions/15704565/efficient-way-to-return-a-stdvector-in-c
};
}

#endif //SESAME_SRC_ALGORITHM_WINDOWMODEL_LANDMARKWINDOW_HPP_
