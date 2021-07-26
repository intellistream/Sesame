//
// Created by tuidan on 2021/7/21.
//

#include <Algorithm/StreamKM.hpp>
#include <Algorithm/DataStructure/Point.hpp>
#include <Algorithm/OfflineClustering/KMeans.hpp>
#include <Utils/UtilityFunctions.hpp>
#include <Utils/Logger.hpp>

SESAME::StreamKM::StreamKM() {
  this->window = std::make_shared<LandmarkWindow>();
}

/**
 * @Description: initial the window setting: window_size, window_number
 * @param pointNumber
 * @param dimension
 * @param coresetSize
 * @param seed
 */
void SESAME::StreamKM::initialWindow(int pointNumber, int dimension, int coresetSize, int seed) {

  UtilityFunctions::init_genrand(seed);
  this->window->bucketManager.numberOfBuckets = ceil(log((double) pointNumber / (double) coresetSize) / log(2)) + 2;
  this->window->bucketManager.maxBucketsize = coresetSize;
  this->window->initBucket(dimension, coresetSize);
  SESAME_INFO(
      "Created manager with " << this->window->bucketManager.numberOfBuckets << " buckets of dimension: " << dimension);
}

/*
* @Description: build the landmark window, insert the data point and construct the coreset tree.
* @Param:
* @Return:
*/
void SESAME::StreamKM::buildTimeWindow(int pointNumber,
                                       vector<Point> &input) {
  for (int i = 0; i < pointNumber; i++) {
    this->window->insertPoint(input[i]);
  }
  this->window->getCoresetFromManager();//streamingCoreset = LandmarkWindow::getCoresetFromManager(manager);
}

//
//void SESAME::StreamKM::runOfflineClustering(int clusterNumber,
//                                            int coresetSize,
//                                            int dimension,
//                                            vector<Point> &output) {
//  double minCost = 0.0;
//  double curCost = 0.0;
//  KMeans km;
//  centresStreamingCoreset =
//      km.lloydPlusPlus(clusterNumber, coresetSize, dimension, streamingCoreset, &minCost);
//  curCost = minCost;
//
//  for (int i = 1; i < 5; i++) {
//    Point *tmpCentresStreamingCoreset =
//        km.lloydPlusPlus(clusterNumber, coresetSize, dimension, streamingCoreset, &curCost);
//    if (curCost < minCost) {
//      minCost = curCost;
//      centresStreamingCoreset = tmpCentresStreamingCoreset;
//    }
//  }
//}

