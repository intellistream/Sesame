//
// Created by Shuhao Zhang on 19/07/2021.
//

#include <Engine/SingleThreadEngine.hpp>
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
Point *SESAME::SingleThreadEngine::runAlgorithm(Point *input,
                                                const string &algoName,
                                                int pointNumber,
                                                int dimension,
                                                int coresetSize,
                                                int clusterNumber) {
  if (algoName == "StreamKMeans") {
    auto *al = new StreamKM(clusterNumber);

    // initialize window model
    al->initialWindow(al->manager, pointNumber, dimension, coresetSize, seed);

    // build window model
    al->buildTimeWindow(pointNumber, input, al->streamingCoreset, al->manager);

    // run offline clustering
    al->runOfflineClustering(clusterNumber, coresetSize, dimension, al->streamingCoreset, al->centresStreamingCoreset);

    return al->centresStreamingCoreset;
  }
  throw std::invalid_argument("Unsupported");
}
SESAME::SingleThreadEngine::SingleThreadEngine() {

}
