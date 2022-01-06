// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by tuidan on 2021/7/21.
//

#include <Algorithm/StreamKM.hpp>
#include <Algorithm/DataStructure/Point.hpp>
#include <Utils/UtilityFunctions.hpp>
#include <Utils/Logger.hpp>
#include <Algorithm/WindowModel/WindowFactory.hpp>
#include <Algorithm/DataStructure/DataStructureFactory.hpp>

void SESAME::StreamKM::Initilize() {
  // initial the landmark window
  UtilityFunctions::init_genrand(this->StreamKMParam.seed);
  this->window = WindowFactory::createLandmarkWindow();
  this->window->windowManager.numberOfWindow =
      ceil(log((double) this->StreamKMParam.pointNumber / (double) this->StreamKMParam.windowSize) / log(2)) + 2;
  this->window->windowManager.maxWindowSize = this->StreamKMParam.windowSize;
  this->window->initWindow();
  this->window->tree = DataStructureFactory::createCoresetTree();
  SESAME_DEBUG(
      "Created manager with " << this->window->windowManager.numberOfWindow << " windows of dimension: "
          << this->StreamKMParam.dimension);
}

/**
 * @Description: build the landmark Window, insert the data point and construct the coreset tree if the window is full
 * @Param:
 * @Return: although void, but actually we store the output result(with computed clustering center) into this->streamingCoreset
 */
void SESAME::StreamKM::runOnlineClustering(const SESAME::PointPtr input) {
  this->window->insertPoint(input);
  this->inputs.push_back(input);
}

/**
 * @Description: we run offline KMeans++ algorithm 5 times using the final m coreset points(this->streamingCoreset)[m>k]
 * @param clusterNumber
 * @param coresetSize
 * @param dimension
 * @param output
 */
void SESAME::StreamKM::runOfflineClustering(DataSinkPtr sinkPtr) {
  this->window->timerMeter.printTime(false,false,false,false);
  this->window->getCoresetFromManager(
      this->streamingCoreset); // streamingCoreset = LandmarkWindow::getCoresetFromManager(manager);
  int parNumber = this->streamingCoreset.size();
  vector <PointPtr> centers;
  vector <vector<PointPtr>> groups;

  std::vector<std::vector<PointPtr>> oldGroups, newGroups;

  this->km.runKMeans(this->StreamKMParam.clusterNumber,
                     parNumber,
                     this->streamingCoreset,
                     oldGroups,
                     newGroups,
                     true);
  // store the result input output
  this->km.produceResult(oldGroups, sinkPtr);
//  this->km.storeResult(oldGroups, centers);
//  this->km.groupPointsByCenters((int) centers.size(), (int) this->inputs.size(),
//                                const_cast<vector <PointPtr> &>(this->inputs), centers, groups);
//  // print the clustering information
//  dumpResults(centers, groups, sinkPtr);
//  cout << endl;
}
//void SESAME::StreamKM::dumpResults(vector <PointPtr> &centers,
//                                   vector <vector<SESAME::PointPtr>> groups,
//                                   DataSinkPtr sinkPtr) const {
//  int cluster = 0;
//  cout << cluster << " cluster: ";
//  for (int i = 0; i < groups.size(); i++) {
//    if (cluster != centers.at(i)->getClusteringCenter()) {
//      cluster = centers.at(i)->getClusteringCenter();
//      cout << endl << cluster << " cluster: ";
//    }
//    for (int j = 0; j < groups[i].size(); j++) {
//      groups[i][j]->setClusteringCenter(centers[i]->getClusteringCenter());
//      cout << groups[i][j]->getIndex() << " ";
//      sinkPtr->put(groups[i][j]);
//    }
//  }
//  cout << endl;
//}

SESAME::StreamKM::StreamKM(param_t &cmd_params) {
  this->StreamKMParam.pointNumber = cmd_params.pointNumber;
  this->StreamKMParam.clusterNumber = cmd_params.clusterNumber;
  this->StreamKMParam.windowSize = cmd_params.coresetSize;
  this->StreamKMParam.seed = cmd_params.seed;
  this->StreamKMParam.dimension = cmd_params.dimension;
}
SESAME::StreamKM::~StreamKM() {

}

