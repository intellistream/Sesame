//
// Created by tuidan on 2021/8/24.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CFTREE_HPP_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CFTREE_HPP_
#include <iostream>
#include <memory>
#include <vector>
#include <Algorithm/DataStructure/FeatureVector.hpp>
#include <Algorithm/DataStructure/Point.hpp>
namespace SESAME {

// define the share point of the class object
class CFNode;
class CFTree;
typedef std::shared_ptr<CFNode> NodePtr;
typedef std::shared_ptr<CFTree> CFTreePtr;

class CFTree {
 private:
  int maxInternalNodes; // max CF number of each internal node
  int maxLeafNodes; // max CF number of each leaf node
  double thresholdDistance; // threshold radius of each sub cluster in leaf nodes
 public:
  CFTree(int b, int l, double t);
  void initialTree(int b, int l, double t);
  ~CFTree();
  int getB() const;
  int getL() const;
  double getT() const;
  void setB(int b);
  void setL(int l);
  void setT(double t);
};


class CFNode {
 private:
  CFPtr curCF;
  bool isLeaf;
  std::vector<NodePtr> children;
  std::vector<NodePtr> parent;
  std:: vector<SESAME::PointPtr> clusterPoints;
  int index;
 public:
  CFNode();
  ~CFNode();
  SESAME::CFPtr getCF();
  void setCF(SESAME::CFPtr &cf);
  void insertPoint(PointPtr &p);
  std::vector<SESAME::PointPtr> getPoints();
  std::vector<SESAME::NodePtr> getParents();
  int getIndex() const;
  std::vector<SESAME::NodePtr> getChildren();
  void removeChild(SESAME::NodePtr &child);
  SESAME::NodePtr copy();
  bool getIsLeaf();
  void setIsLeaf(bool leaf);
  void setNode(SESAME::CFPtr &Node);
  void setIndex(int Index);
  void setParent(SESAME::NodePtr &Parent);
  void setChild(SESAME::NodePtr &child);
  void clearParents();
};
}
#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CFTREE_HPP_
