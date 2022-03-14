//
// Created by tuidan on 2021/8/24.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CFTREE_HPP_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CFTREE_HPP_

#include "Algorithm/DataStructure/FeatureVector.hpp"
#include "Algorithm/DataStructure/Point.hpp"
#include "Algorithm/DesignAspect/Param.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <ranges>
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
  CFTree(const StreamClusteringParam &param);
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

class ClusteringFeaturesTree {
private:
  int maxInternalNodes; // max CF number of each internal node
  int maxLeafNodes;     // max CF number of each leaf node
  double
      thresholdDistance; // threshold radius of each sub cluster in leaf nodes
public:
  class Node;
  using NodePtr = std::shared_ptr<Node>;
  ClusteringFeaturesTree(const StreamClusteringParam &param);
  ClusteringFeaturesTree(int maxInternalNodes, int maxLeafNodes,
                         double thresholdDistance);
  ~ClusteringFeaturesTree();
  int getMaxInternalNodes() const;
  int getMaxLeafNodes() const;
  double getThresholdDistance() const;
  void setMaxInternalNodes(int b);
  void setMaxLeafNodes(int l);
  void setThresholdDistance(double t);
  NodePtr root;
  class Node {
  private:
    NodePtr parent;
    std::vector<NodePtr> children;
    bool isLeaf = true;
    int index;
    // 原CF结构体，numNodes是子类中节点的数目，LS是N个节点的线性和，SS是N个节点的平方和
    int numNodes = 0;
    std::vector<double> ls, ss;

  public:
    Node() = default;
    ~Node() = default;
    NodePtr getParent() { return parent; }
    int getIndex() const { return index; }
    const std::vector<NodePtr> &getChildren() { return children; }
    void removeChild(NodePtr &child) {
      std::ranges::remove_if(children, [&](NodePtr &c) { return c == child; });
    }
    // NodePtr copy();
    bool getIsLeaf() const { return isLeaf; }
    void setIsLeaf(bool leaf) { isLeaf = leaf; }
    void setIndex(int i) { index = i; }
    void setParent(const NodePtr &p) { parent = p; }
    void addChild(const NodePtr &child) { children.push_back(child); }
    void setChildren(const std::vector<NodePtr> &c) { children = c; }
    void clearParents() { parent->setIndex(-1); }
    int getNumNodes() const { return numNodes; }
    void setNumNodes(int n) { numNodes = n; }
    const std::vector<double> &getLS() const { return ls; };
    const std::vector<double> &getSS() const { return ss; };
    void setLS(const std::vector<double> &newLS) {
      if (ls.empty()) {
        ls = newLS;
      } else {
        assert(ls.size() == newLS.size());
        ls = newLS;
      }
    }
    void setSS(const std::vector<double> &newSS) {
      if (ss.empty()) {
        ss = newSS;
      } else {
        assert(ss.size() == newSS.size());
        ss = newSS;
      }
    }
  };
};

} // namespace SESAME
#endif // SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CFTREE_HPP_
