//
// Created by tuidan on 2021/8/24.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CFTREE_HPP_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CFTREE_HPP_

#include "Algorithm/DataStructure/FeatureVector.hpp"

#include "Algorithm/DataStructure/GenericFactory.hpp"
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
  bool isOutlier;
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
  void setIsOutlier(bool flag);
  bool getIsOutlier();
};

struct ClusteringFeatures {
  // 原CF结构体，numPoints是子类中节点的数目，LS是N个节点的线性和，SS是N个节点的平方和
  int numPoints = 0;
  std::vector<double> ls, ss;
  ClusteringFeatures(int d = 0)
      : ls(std::vector<double>(d, 0.0)), ss(std::vector<double>(d, 0.0)) {}
};

class ClusteringFeaturesTree {
private:
  const int maxInternalNodes; // max CF number of each internal node
  const int maxLeafNodes;     // max CF number of each leaf node
  const double
      thresholdDistance; // threshold radius of each sub cluster in leaf nodes
  const int dim;
  int leafMask = 0;

public:
  struct Node;
  using NodePtr = std::shared_ptr<Node>;
  ClusteringFeaturesTree(const StreamClusteringParam &param);
  ~ClusteringFeaturesTree();
  void insert(PointPtr point, std::vector<NodePtr> &outliers);
  std::vector<std::vector<double>>
  calcAdjacencyMatrix(const std::vector<NodePtr> &nodes);
  NodePtr closestNode(const std::vector<NodePtr> &children, PointPtr point);
  void backwardEvolution(NodePtr node, PointPtr point);
  NodePtr root;
  struct Node : std::enable_shared_from_this<Node> {
    NodePtr parent;
    std::vector<NodePtr> children;
    int index = 0;
    const int dim;
    ClusteringFeatures cf;

    Node(int d = 0, NodePtr p = nullptr) : dim(d), cf(d), parent(p){};
    ~Node() = default;
    void removeChild(NodePtr child) {
      std::ranges::remove_if(children,
                             [&](auto &c) { return c->index == child->index; });
    }
    bool isLeaf() const { return children.empty(); }
    void addChild(NodePtr child) {
      children.push_back(child);
      child->parent = shared_from_this();
    }
    void clearParents() { parent->index = -1; }
    template <typename T> void setCF(const T &t) {
      this->cf = ClusteringFeatures(t);
    }
    void update(PointPtr point, bool all) {
      for (int i = 0; i < dim; ++i) {
        auto val = point->getFeatureItem(i);
        cf.ls[i] += val;
        cf.ss[i] += val * val;
      }
      if (parent != nullptr && all) {
        parent->update(point, all);
      }
    }
    PointPtr centroid() {
      auto c = GenericFactory::create<Point>();
      c->setIndex(-1);
      c->setClusteringCenter(-1);
      for (int i = 0; i < cf.ls.size(); ++i)
        c->setFeatureItem(cf.ls[i] / cf.numPoints, i);
      return c;
    }
    void merge(NodePtr child) {
      cf.numPoints += child->cf.numPoints;
      for (int i = 0; i < dim; ++i) {
        cf.ls[i] += child->cf.ls[i];
        cf.ss[i] += child->cf.ss[i];
      }
    }
  };
};

class ClusteringFeaturesList {
private:
  const int maxInternalNodes; // max CF number of each internal node
  const int maxLeafNodes;     // max CF number of each leaf node
  const double
      thresholdDistance; // threshold radius of each sub cluster in leaf nodes
  const int dim;

public:
  struct Node;
  using NodePtr = std::shared_ptr<Node>;
  ClusteringFeaturesList(const StreamClusteringParam &param);
  ~ClusteringFeaturesList();
  void insert(PointPtr point, std::vector<NodePtr> &outliers);
  std::vector<std::vector<double>>
  calcAdjacencyMatrix(const std::vector<NodePtr> &nodes);
  NodePtr closestNode(const std::vector<NodePtr> &nodes, PointPtr point);
  void backwardEvolution(NodePtr node, PointPtr point);
  std::vector<NodePtr> nodes;
  struct Node : std::enable_shared_from_this<Node> {
    NodePtr parent;
    std::vector<NodePtr> children;
    int index = 0;
    const int dim;
    ClusteringFeatures cf;

    Node(int d = 0, NodePtr p = nullptr) : dim(d), cf(d), parent(p){};
    ~Node() = default;
    void removeChild(NodePtr child) {
      std::ranges::remove_if(children,
                             [&](auto &c) { return c->index == child->index; });
    }
    bool isLeaf() const { return children.empty(); }
    void addChild(NodePtr child) {
      children.push_back(child);
      child->parent = shared_from_this();
    }
    void clearParents() { parent->index = -1; }
    template <typename T> void setCF(const T &t) {
      this->cf = ClusteringFeatures(t);
    }
    void update(PointPtr point) {
      for (int i = 0; i < dim; ++i) {
        auto val = point->getFeatureItem(i);
        cf.ls[i] += val;
        cf.ss[i] += val * val;
      }
    }
    PointPtr centroid() {
      auto c = GenericFactory::create<Point>();
      c->setIndex(-1);
      c->setClusteringCenter(-1);
      for (int i = 0; i < cf.ls.size(); ++i)
        c->setFeatureItem(cf.ls[i] / cf.numPoints, i);
      return c;
    }
    void merge(NodePtr child) {
      cf.numPoints += child->cf.numPoints;
      for (int i = 0; i < dim; ++i) {
        cf.ls[i] += child->cf.ls[i];
        cf.ss[i] += child->cf.ss[i];
      }
    }
  };
};

} // namespace SESAME
#endif // SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CFTREE_HPP_
