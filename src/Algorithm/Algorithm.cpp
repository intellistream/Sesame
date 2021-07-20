//
// Created by Shuhao Zhang on 19/07/2021.
//
#include <Algorithm/Algorithm.hpp>
#include <Algorithm/OfflineClustering/KMeans.hpp>
#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <Utils/UtilityFunctions.hpp>
#include <Utils/Logger.hpp>

using namespace std;

static int seed;

/**
 * run the algorithm
 * TODO: make it more generic to include more types of algorithms. Currently, it only fits StreamKMeans.
 * It is expected to have many duplications when more algorithms are added.
 * @param algoName
 * @param pointNumber
 * @param dimension
 * @param coresetSize
 * @param clusterNumber
 */
Point *SESAME::Algorithm::run(Point *input,
                              const string &algoName,
                              int pointNumber,
                              int dimension,
                              int coresetSize,
                              int clusterNumber) {

  if (algoName == "StreamKMeans") {
    UtilityFunctions::init_genrand(seed);
    auto *manager = (LandmarkWindow::Bucketmanager *) malloc(sizeof(LandmarkWindow::Bucketmanager));
    initManager(manager, pointNumber, dimension, coresetSize);

    // construct window
    for (int i = 0; i < pointNumber; i++) {
      LandmarkWindow::insertPoint(input[i], manager);
    }

    Point *streamingCoreset = LandmarkWindow::getCoresetFromManager(manager);

    double minCost = 0.0;
    double curCost = 0.0;
    KMeans km;
    Point *centresStreamingCoreset =
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
    return centresStreamingCoreset;
  }

  throw std::invalid_argument("Unsupported");

}
void SESAME::Algorithm::initManager(SESAME::LandmarkWindow::Bucketmanager *manager,
                                    int n,
                                    int d,
                                    int maxsize) {
  manager->numberOfBuckets = ceil(log((double) n / (double) maxsize) / log(2)) + 2;
  manager->maxBucketsize = maxsize;
  manager->buckets =
      (struct LandmarkWindow::Bucket *) malloc(manager->numberOfBuckets * sizeof(LandmarkWindow::Bucket));
  int i;
  for (i = 0; i < manager->numberOfBuckets; i++) {
    LandmarkWindow::initBucket(&(manager->buckets[i]), d, maxsize);
  }
  SESAME_INFO("Created manager with " << manager->numberOfBuckets << " buckets of dimension: " << d);
}
