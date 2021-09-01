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
namespace SESAME {

// define the share point of the class object
class DPNode;
class DPTree;
typedef std::shared_ptr<DPNode> DPNodePtr;
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

};


class DPNode {
 private:
  int cid;
  int Cid;
  int num;
  double rho;
  double delta;
  SESAME::DPNodePtr dep;
  SESAME::PointPtr center;
  long lastTime;
  bool active;
  std::vector<SESAME::DPNodePtr> sucs;
  SESAME::DPTreePtr cluster;
  // public double sumDelta;
  // public int sucNum;
  long inactiveTime;

  /**
   * we will use dis to quickly update the delta of CluCell
   */
  double dis;
  int id = 0;
 public:
  DPNode();
  ~DPNode();
  explicit DPNode(SESAME::PointPtr p, long startTime);
  void insert(SESAME::PointPtr p);
  void add(SESAME::PointPtr p, double coef);
  void addSuccessor(SESAME::DPNodePtr &node);
  void removeSuccessor(SESAME::DPNodePtr &node);
  bool hasSuccessor();

};
}
#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_DPTREE_HPP_
