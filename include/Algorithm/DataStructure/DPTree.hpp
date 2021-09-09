//
// Created by tuidan on 2021/9/1.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_DPTREE_HPP_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_DPTREE_HPP_
#include <iostream>
#include <memory>
#include <vector>
#include <Algorithm/DataStructure/FeatureVector.hpp>
#include <Algorithm/DataStructure/Point.hpp>
#include <Algorithm/DataStructure/DPNode.hpp>
#include <Algorithm/DataStructure/OutlierReservoir.hpp>
namespace SESAME {

// define the share point of the class object
class DPTree;
typedef std::shared_ptr<DPTree> DPTreePtr;

class DPTree {
 private:
  long lastTime;
  int size;
  int num;
  std::vector<SESAME::DPNodePtr> Clus;

  double a;
  double lamd;
  double CluR;

  int cluLabel;
  double minDelta;
 public:
  DPTree();
  ~DPTree();
  DPTree(int num, double CluR);
  void insert(SESAME::DPNodePtr cc, double minRho, SESAME::PointPtr p, int opt);
  void init(std::vector<SESAME::DPNodePtr> clus, int size, double minRho, double minDelta,
            SESAME::OutPtr outs, std::vector<ClusterPtr> clusters);
  SESAME::DPNodePtr  findNN(PointPtr p, double coef,
                       std::vector<ClusterPtr> clusters, double minRho, int opt);
  void adjustNoDelta(int index, double minRho, SESAME::PointPtr p);
  void adjustNoOpt(int index, double minRho, SESAME::PointPtr p);
  void computeDeltaNoOpt(int index);
  void adjustOpt1(int index, double minRho, SESAME::PointPtr p);
  void computeDeltaF1(int index);
  void adjust(int index, double minRho, SESAME::PointPtr p);
  void computeHeadDelta();
  void computeDelta(int index);
  void deleteInact(SESAME::OutPtr outres, double coef,
                         double minRho, double time);
  double computeAlpha(double minDelta);
  double adjustMinDelta(double alpha);
  double djustMinDelta(double alpha, double minDelta);
  double fun(double alpha, double upavg, double downavg, double avg);
  bool check(SESAME::OutPtr outres);
  void check(std::vector<SESAME::ClusterPtr> clusters);
  void adjustCluster(std::vector<SESAME::ClusterPtr> clusters, bool minChed);
  void writeInfo(std::string string);


};




}
#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_DPTREE_HPP_
