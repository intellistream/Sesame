//
// Created by tuidan on 2021/7/21.
//

#include <Algorithm/StreamKM.hpp>
#include <Algorithm/DataStructure/Point.hpp>
#include <Algorithm/OfflineClustering/KMeans.hpp>
#include <Utils/UtilityFunctions.hpp>
#include <Utils/Logger.hpp>

SESAME::StreamKM::StreamKM() = default;

void SESAME::StreamKM::runOfflineClustering(int clusterNumber, int coresetSize, int dimension, Point *streamingCoreset, Point * centresStreamingCoreset) {
  double minCost = 0.0;
  double curCost = 0.0;
  KMeans km;
  centresStreamingCoreset =
      km.lloydPlusPlus(clusterNumber, coresetSize, dimension, streamingCoreset, &minCost);
  curCost = minCost;

  for (int i = 1; i < 5; i++) {
    Point *tmpCentresStreamingCoreset =
        km.lloydPlusPlus(clusterNumber, coresetSize, dimension, streamingCoreset, &curCost);
    if (curCost < minCost) {
      minCost = curCost;
      centresStreamingCoreset = tmpCentresStreamingCoreset;
    }
  }
}

/**
* @Description: initial the window setting: window_size, window_number
* @Param: 
* @Return:
*/

void SESAME::StreamKM::initialWindow(LandmarkWindow::Bucketmanager *manager,
                                     int pointNumber,int dimension, int coresetSize, int seed) {
  UtilityFunctions::init_genrand(seed);
  manager->numberOfBuckets = ceil(log((double) pointNumber / (double) coresetSize) / log(2)) + 2;
  manager->maxBucketsize = coresetSize;
  manager->buckets =
      (struct LandmarkWindow::Bucket *) malloc(manager->numberOfBuckets * sizeof(LandmarkWindow::Bucket));
  int i;
  for (i = 0; i < manager->numberOfBuckets; i++) {
    LandmarkWindow::initBucket(&(manager->buckets[i]), dimension, coresetSize);
  }
  SESAME_INFO("Created manager with " << manager->numberOfBuckets << " buckets of dimension: " << dimension);
}

/**
* @Description: build the landmark window, insert the data point and construct the coreset tree.
* @Param:
* @Return:
*/
void SESAME::StreamKM::buildTimeWindow(int pointNumber, Point *p, Point *streamingCoreset, SESAME::LandmarkWindow::Bucketmanager *manager) {
  for (int i = 0; i < pointNumber; i++) {
    LandmarkWindow::insertPoint(p[i], manager);
  }
  streamingCoreset = LandmarkWindow::getCoresetFromManager(manager);
}


