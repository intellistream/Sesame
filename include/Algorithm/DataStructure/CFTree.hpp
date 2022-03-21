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
#include <cmath>
#include <iostream>
#include <limits>
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

template <typename T> concept NodeConcept = requires(T t) {
  t->Centroid();
  t->cf.numPoints;
  t->index;
  t->Update(GenericFactory::create<Point>());
};

template <NodeConcept T>
std::vector<std::vector<double>> CalcAdjMatrix(const std::vector<T> &nodes) {
  int n = nodes.size();
  std::vector<std::vector<double>> adjMatrix(n, std::vector<double>(n, 0.0));
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      auto centroid1 = nodes[i]->Centroid(), centroid2 = nodes[j]->Centroid();
      auto distance = centroid1->distance(centroid2);
      adjMatrix[i][j] = distance, adjMatrix[j][i] = distance;
    }
  }
  return adjMatrix;
}

template <NodeConcept T>
std::pair<T, double> CalcClosestNode(const std::vector<T> &nodes,
                                     PointPtr point) {
  double minDist = std::numeric_limits<double>::max();
  T node = nullptr;
  for (auto child : nodes) {
    auto centroid = child->Centroid();
    auto distance = centroid->distance(point);
    if (distance < minDist) {
      minDist = distance;
      node = child;
    }
  }
  return std::make_pair(node, minDist);
}

template <NodeConcept T> double CalcClusterDist(T a, T b) {
  double dist = 0.0;
  auto ca = a->Centroid(), cb = b->Centroid();
  for (int i = 0; i < ca->getDimension(); ++i) {
    auto val = ca->getFeatureItem(i) - cb->getFeatureItem(i);
    dist += val * val;
  }
  return sqrt(dist);
}

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
  void Insert(PointPtr point);
  void Insert(NodePtr node);
  const std::vector<NodePtr> &clusters();

private:
  template <typename T> void backwardEvolution(NodePtr node, T point);
  NodePtr root_;
  std::vector<NodePtr> clusters_;

public:
  struct Node : std::enable_shared_from_this<Node> {
    NodePtr parent;
    std::vector<NodePtr> children;
    int index = 0;
    const int dim;
    ClusteringFeatures cf;

    Node(int d = 0, NodePtr p = nullptr) : dim(d), cf(d), parent(p){};
    Node(PointPtr p) : Node(p->getDimension()) { Update(p); }
    ~Node() = default;
    void RemoveChild(NodePtr child) {
      std::ranges::remove_if(children,
                             [&](auto &c) { return c->index == child->index; });
    }
    bool IsLeaf() const { return children.empty(); }
    void AddChild(NodePtr child) {
      children.push_back(child);
      child->parent = shared_from_this();
    }
    void ClearParents() {
      if (parent != nullptr)
        parent->index = -1;
    }
    void Update(PointPtr point) {
      cf.numPoints++;
      for (int i = 0; i < dim; ++i) {
        auto val = point->getFeatureItem(i);
        cf.ls[i] += val;
        cf.ss[i] += val * val;
      }
    }
    void Update(NodePtr node) {
      cf.numPoints += node->cf.numPoints;
      for (int i = 0; i < dim; ++i) {
        cf.ls[i] += node->cf.ls[i];
        cf.ss[i] += node->cf.ss[i] * node->cf.ss[i];
      }
    }
    template <typename T> void Update(T point, bool all) {
      Update(point);
      if (parent != nullptr && all) {
        parent->Update(point, all);
      }
    }
    PointPtr Centroid() {
      auto c = GenericFactory::create<Point>();
      c->setIndex(-1);
      c->setClusteringCenter(-1);
      for (int i = 0; i < cf.ls.size(); ++i)
        c->setFeatureItem(cf.ls[i] / cf.numPoints, i);
      return c;
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
  void Insert(PointPtr point);
  void Insert(NodePtr node);
  const std::vector<NodePtr> &clusters();

private:
  std::vector<NodePtr> clusters_;

public:
  struct Node : std::enable_shared_from_this<Node> {
    int index = 0;
    const int dim;
    ClusteringFeatures cf;

    Node(int d = 0) : dim(d), cf(d){};
    Node(PointPtr p) : Node(p->getDimension()) { Update(p); }
    ~Node() = default;
    void Update(PointPtr point) {
      cf.numPoints++;
      for (int i = 0; i < dim; ++i) {
        auto val = point->getFeatureItem(i);
        cf.ls[i] += val;
        cf.ss[i] += val * val;
      }
    }
    void Update(NodePtr node) {
      cf.numPoints += node->cf.numPoints;
      for (int i = 0; i < dim; ++i) {
        cf.ls[i] += node->cf.ls[i];
        cf.ss[i] += node->cf.ss[i] * node->cf.ss[i];
      }
    }
    PointPtr Centroid() {
      auto c = GenericFactory::create<Point>();
      c->setIndex(-1);
      c->setClusteringCenter(-1);
      for (int i = 0; i < cf.ls.size(); ++i)
        c->setFeatureItem(cf.ls[i] / cf.numPoints, i);
      return c;
    }
  };
};

} // namespace SESAME
#endif // SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CFTREE_HPP_
