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
 * @Description: build the landmark Window, insert the data point and construct the coreset tree if the window is full
 * @Param:
 * @Return: although void, but actually we store the output result(with computed clustering center) into this->streamingCoreset
 */
void SESAME::StreamKM::runOnlineClustering(const vector<PointPtr> &input) {

  // initial the landmark window
  UtilityFunctions::init_genrand(this->StreamKMParam.seed);
  this->window = WindowFactory::createLandmarkWindow();
  this->window->windowManager.numberOfWindow =
      ceil(log((double) this->StreamKMParam.pointNumber / (double) this->StreamKMParam.windowSize) / log(2)) + 2;
  this->window->windowManager.maxWindowSize = this->StreamKMParam.windowSize;
  this->window->initWindow();
  this->window->tree = DataStructureFactory::createCoresetTree();
  SESAME_INFO(
      "Created manager with " << this->window->windowManager.numberOfWindow << " windows of dimension: "
                              << this->StreamKMParam.dimension);

  // insert data points into the window
  for (int i = 0; i < this->StreamKMParam.pointNumber; i++) {
    this->window->insertPoint(input[i]);
  }
  this->window->getCoresetFromManager(this->streamingCoreset);//streamingCoreset = LandmarkWindow::getCoresetFromManager(manager);
}

/**
 * @Description: we run offline KMeans++ algorithm 5 times using the final m coreset points(this->streamingCoreset)[m>k]
 * @param clusterNumber
 * @param coresetSize
 * @param dimension
 * @param output
 */
void SESAME::StreamKM::runOfflineClustering(const std::vector<PointPtr> &input, vector<PointPtr> &output) {
  int parNumber = this->streamingCoreset.size();
  vector<PointPtr> centers;
  vector<vector<PointPtr>> groups;

  this->km.runKMeans(this->StreamKMParam.clusterNumber, parNumber, this->streamingCoreset, centers, true);
  this->km.groupPointsByCenters((int) centers.size(), (int) input.size(),
                                const_cast<vector<PointPtr> &>(input), centers, groups);
  // print the clustering information
  int cluster = 0;
  cout << cluster << " cluster: ";
  for (int i = 0; i < groups.size(); i++) {
    if (cluster != centers.at(i)->getClusteringCenter()) {
      cluster = centers.at(i)->getClusteringCenter();
      cout << endl << cluster << " cluster: ";
    }
    for (int j = 0; j < groups[i].size(); j++) {
      groups[i][j]->setClusteringCenter(centers[i]->getClusteringCenter());
      cout << groups[i][j]->getIndex() << " ";
      output.push_back(groups[i][j]);
    }
  }
  cout << endl;
}
SESAME::StreamKM::StreamKM() {

}
SESAME::StreamKM::~StreamKM() {

}

