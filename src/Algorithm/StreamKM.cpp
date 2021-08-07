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
 * @Description: initial the Window setting: window_size, window_number
 * @param pointNumber
 * @param dimension
 * @param coresetSize
 * @param seed
 */
void SESAME::StreamKM::initialWindow(int pointNumber, int dimension, int windowSize, int seed) {

  UtilityFunctions::init_genrand(seed);
  this->window = WindowFactory::createLandmarkWindow();
  this->window->windowManager.numberOfWindow = ceil(log((double) pointNumber / (double) windowSize) / log(2)) + 2;
  this->window->windowManager.maxWindowSize = windowSize;
  this->window->initWindow(dimension, windowSize);
  this->window->tree = DataStructureFactory::createCoresetTree();
  SESAME_INFO(
      "Created manager with " << this->window->windowManager.numberOfWindow << " windows of dimension: " << dimension);
}

/**
 * @Description: build the landmark Window, insert the data point and construct the coreset tree if the window is full
 * @Param:
 * @Return: although void, but actually we store the output result(with computed clustering center) into this->streamingCoreset
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
 * @Description: we run offline KMeans++ algorithm 5 times using the final m coreset points(this->streamingCoreset)[m>k]
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

  for (int i = 1; i < KMEANS_TIMES; i++) {
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

