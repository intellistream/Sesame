//
// Created by tuidan on 2021/8/24.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CFTREE_HPP_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CFTREE_HPP_

#include "Algorithm/DataStructure/FeatureVector.hpp"
#include "Algorithm/DataStructure/Point.hpp"
#include "Algorithm/DesignAspect/Param.hpp"

#include <iostream>
#include <memory>
#include <vector>

namespace SESAME {

// define the share point of the class object
class CFNode;
class CFTree;
typedef std::shared_ptr<CFNode> NodePtr;
typedef std::shared_ptr<CFTree> CFTreePtr;

class CFTree {
private:
  int maxInternalNodes; // max CF number of each internal node
  int maxLeafNodes;     // max CF number of each leaf node
  double
      thresholdDistance; // threshold radius of each sub cluster in leaf nodes
public:
  CFTree(const StreamClusteringAlgorithmParameters &param);
  CFTree(int b, int l, double t);
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
  NodePtr parent;
  int index;

public:
  CFNode();
  ~CFNode();
  CFPtr getCF();
  void setCF(CFPtr &cf);
  NodePtr getParent();
  int getIndex() const;
  std::vector<NodePtr> getChildren();
  void removeChild(NodePtr &child);
  NodePtr copy();
  bool getIsLeaf();
  void setIsLeaf(bool leaf);
  void setNode(CFPtr &Node);
  void setIndex(int Index);
  void setParent(NodePtr &Parent);
  void setChild(NodePtr &child);
  void setChildren(std::vector<NodePtr> children);
  void clearParents();
};
} // namespace SESAME
#endif // SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CFTREE_HPP_
