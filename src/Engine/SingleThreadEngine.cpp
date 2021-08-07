//
// Created by Shuhao Zhang on 19/07/2021.
//

#include <Engine/SingleThreadEngine.hpp>
using namespace std;

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

void SESAME::SingleThreadEngine::runAlgorithm(const vector<PointPtr> &input,
                                              vector<PointPtr> &output,
                                              AlgorithmPtr algo) {

  // initialize Window model
  algo->initialWindow();

  // build Window model
  algo->buildTimeWindow(input);

  // run offline clustering
  algo->runOfflineClustering(input, output);
}
