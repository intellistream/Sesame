//
// Created by tuidan on 2021/7/21.
//

#include <Algorithm/StreamKM.hpp>
#include <Algorithm/DataStructure/Point.hpp>
#include <Algorithm/OfflineClustering/KMeans.hpp>
#include <Utils/UtilityFunctions.hpp>
#include <Utils/Logger.hpp>
#include <Algorithm/WindowModel/WindowFactory.hpp>
#include <Algorithm/DataStructure/DataStructureFactory.hpp>
/**
 * @Description: initial the window setting: window_size, window_number
 * @param pointNumber
 * @param dimension
 * @param coresetSize
 * @param seed
 */
void SESAME::StreamKM::initialWindow(int pointNumber, int dimension, int coresetSize, int seed) {

  UtilityFunctions::init_genrand(seed);
  this->window = WindowFactory::createLandmarkWindow();
  this->window->bucketManager.numberOfBuckets = ceil(log((double) pointNumber / (double) coresetSize) / log(2)) + 2;
  this->window->bucketManager.maxBucketsize = coresetSize;
  this->window->initBucket(dimension, coresetSize);
  this->window->tree = DataStructureFactory::createCoresetTree();
  SESAME_INFO(
      "Created manager with " << this->window->bucketManager.numberOfBuckets << " buckets of dimension: " << dimension);
}

/*
* @Description: build the landmark window, insert the data point and construct the coreset tree.
* @Param:
* @Return:
*/
void SESAME::StreamKM::buildTimeWindow(int pointNumber,
                                       const vector<PointPtr> &input) {
  for (int i = 0; i < pointNumber; i++) {
    this->window->insertPoint(input[i]);
  }
  this->streamingCoreset =
      this->window->getCoresetFromManager();//streamingCoreset = LandmarkWindow::getCoresetFromManager(manager);
}

/**
 *
 * @param clusterNumber
 * @param coresetSize
 * @param dimension
 * @param output
 */
void SESAME::StreamKM::runOfflineClustering(int clusterNumber, int coresetSize,
                                            int dimension, vector<PointPtr> &output) {
  double minCost = 0.0;
  double curCost = 0.0;
  output = this->km.lloydPlusPlus(clusterNumber, coresetSize, dimension, this->streamingCoreset, &minCost);
  curCost = minCost;

  for (int i = 1; i < 5; i++) {
    vector<PointPtr> tmpCentresStreamingCoreset =
        this->km.lloydPlusPlus(clusterNumber, coresetSize, dimension, this->streamingCoreset, &curCost);
    if (curCost < minCost) {
      minCost = curCost;
      output = tmpCentresStreamingCoreset;
    }
  }
}
SESAME::StreamKM::StreamKM() {

}
SESAME::StreamKM::~StreamKM() {

}

