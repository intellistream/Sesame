//
// Created by Shuhao Zhang on 19/07/2021.
//

#include <Engine/SingleThreadEngine.hpp>
using namespace std;
static int seed;

SESAME::SingleThreadEngine::SingleThreadEngine() = default;

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

void SESAME::SingleThreadEngine::runAlgorithm(vector<Point> &input,
                                              vector<Point> &output,
                                              AlgorithmPtr algo,
                                              int pointNumber,
                                              int dimension,
                                              int coresetSize,
                                              int clusterNumber) {

  // initialize window model
  algo->initialWindow(pointNumber, dimension, coresetSize, seed);

  // build window model
  algo->buildTimeWindow(pointNumber, input);
//
//  // run offline clustering
//  algo.runOfflineClustering(clusterNumber, coresetSize, dimension, output);
}
