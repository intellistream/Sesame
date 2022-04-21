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

void SESAME::StreamKM::Init() {
  // initial the landmark window
  UtilityFunctions::init_genrand(this->StreamKMParam.seed);
  this->window = WindowFactory::createLandmarkWindow();
  this->window->windowManager.numberOfWindow =
      ceil(log((double) this->StreamKMParam.num_points / (double) this->StreamKMParam.windowSize) / log(2)) + 2;
  this->window->windowManager.maxWindowSize = this->StreamKMParam.windowSize;
  this->window->initWindow(this->StreamKMParam.windowSize);
  this->window->tree = DataStructureFactory::createCoresetTree();
  SESAME_DEBUG(
      "Created manager with " << this->window->windowManager.numberOfWindow << " windows of dim: "
          << this->StreamKMParam.dim);
  sum_timer.Tick();
}

/**
 * @Description: build the landmark Window, insert the data point and construct the coreset tree if the window is full
 * @Param:
 * @Return: although void, but actually we store the output result(with computed clustering center) into this->streamingCoreset
 */
void SESAME::StreamKM::RunOnline(const SESAME::PointPtr input) {
  ds_timer.Tick();
  this->window->insertPoint(input);
  ds_timer.Tock();
}

/**
 * @Description: we run offline KMeans++ algorithm 5 times using the final m coreset points(this->streamingCoreset)[m>k]
 * @param num_clusters
 * @param coreset_size
 * @param dim
 * @param output
 */
void SESAME::StreamKM::RunOffline(DataSinkPtr sinkPtr) {
  ref_timer.Tick();
  this->window->getCoresetFromManager(
      this->streamingCoreset); // streamingCoreset = LandmarkWindow::getCoresetFromManager(manager);
  int parNumber = this->streamingCoreset.size();
  for(int i = 0; i < parNumber; i++) {
    for(int j = 0; j < this->StreamKMParam.dim; j++) {
      this->streamingCoreset[i]->setFeatureItem(this->streamingCoreset[i]->getFeatureItem(j) / this->streamingCoreset[i]->getWeight(), j);
    }
  }
  vector <PointPtr> centers;
  vector <vector<PointPtr>> groups;

  std::vector<std::vector<PointPtr>> oldGroups, newGroups;

  this->km.runKMeans(this->StreamKMParam.num_clusters,
                     parNumber,
                     centers,
                     this->streamingCoreset,
                     oldGroups,
                     newGroups,
                     true);
  // store the result input output
  this->km.produceResult(oldGroups,sinkPtr);
  ref_timer.Tock();
  sum_timer.Tock();
}

SESAME::StreamKM::StreamKM(param_t &cmd_params) {
  this->param = cmd_params;
  this->StreamKMParam.num_points = cmd_params.num_points;
  this->StreamKMParam.num_clusters = cmd_params.num_clusters;
  this->StreamKMParam.windowSize = cmd_params.coreset_size;
  this->StreamKMParam.seed = cmd_params.seed;
  this->StreamKMParam.dim = cmd_params.dim;
}
SESAME::StreamKM::~StreamKM() {

}

