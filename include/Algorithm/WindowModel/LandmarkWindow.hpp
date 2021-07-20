//
// Created by Shuhao Zhang on 20/07/2021.
//

#ifndef SESAME_SRC_ALGORITHM_WINDOWMODEL_LANDMARKWINDOW_HPP_
#define SESAME_SRC_ALGORITHM_WINDOWMODEL_LANDMARKWINDOW_HPP_

#include <Algorithm/DataStructure/Point.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

namespace SESAME {
class LandmarkWindow {
 public:
  /**
datastructure representing a single bucket
**/
  struct Bucket {
    int cursize;
    Point *points;
    Point *spillover;
  };
/**
datastructure for managing all O(log(n)) buckets
**/
  struct Bucketmanager {
    int numberOfBuckets;
    int maxBucketsize;
    struct Bucket *buckets;
  };

/**
initializes a bucket
**/
  static void initBucket(Bucket *bucket, int d, int maxsize);

  /**
initializes a bucketmanager for n points with bucketsize maxsize and dimension d
**/
  static void initManager(Bucketmanager *manager, int n, int d, int maxsize);

/**
inserts a single point into the bucketmanager
**/
  static void insertPoint(Point p, struct Bucketmanager *manager);

/**
It may happen that the manager is not full (since n is not always a power of 2). In this case we extract the coreset
from the manager by computing a coreset of all nonempty buckets

Case 1: the last bucket is full
=> n is a power of 2 and we return the contents of the last bucket

Case2: the last bucket is not full
=> we compute a coreset of all nonempty buckets

this operation should only be called after the streaming process is finished
**/
  static Point *getCoresetFromManager(SESAME::LandmarkWindow::Bucketmanager *manager);
};
}

#endif //SESAME_SRC_ALGORITHM_WINDOWMODEL_LANDMARKWINDOW_HPP_
