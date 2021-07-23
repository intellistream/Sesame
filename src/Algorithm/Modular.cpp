//
// Created by Shuhao Zhang on 19/07/2021.
//
#include <Algorithm/Modular.hpp>
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
Point *SESAME::Modular::run(Point *input,
                              const string &algoName,
                              int pointNumber,
                              int dimension,
                              int coresetSize,
                              int clusterNumber) {

  if (algoName == "StreamKMeans") {
    auto * al = new StreamKM();
    auto * manager = new LandmarkWindow::Bucketmanager;
    auto * streamingCoreset = new Point[10];
    auto * centresStreamingCoreset = new Point[clusterNumber];

    // initialize window model
    al->initialWindow(manager, pointNumber, dimension, coresetSize, seed);

    // build window model
    al->buildTimeWindow(pointNumber, input, streamingCoreset, manager);

    // run offline clustering
    al->runOfflineClustering(clusterNumber, coresetSize, dimension, streamingCoreset,centresStreamingCoreset);


    delete(al);
    delete(manager);
    return centresStreamingCoreset;
  }

  throw std::invalid_argument("Unsupported");

}
