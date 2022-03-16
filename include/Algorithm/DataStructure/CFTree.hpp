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

class ClusteringFeaturesTree
    : std::enable_shared_from_this<ClusteringFeaturesTree> {
private:
  int maxInternalNodes; // max CF number of each internal node
  int maxLeafNodes;     // max CF number of each leaf node
  double
      thresholdDistance; // threshold radius of each sub cluster in leaf nodes
  int leafMask = 0;

public:
  class Node;
  using NodePtr = std::shared_ptr<Node>;
  using TreePtr = std::shared_ptr<ClusteringFeaturesTree>;
  ClusteringFeaturesTree(const StreamClusteringParam &param);
  ~ClusteringFeaturesTree();
  int getMaxInternalNodes() const;
  int getMaxLeafNodes() const;
  double getThresholdDistance() const;
  void setMaxInternalNodes(int b);
  void setMaxLeafNodes(int l);
  void setThresholdDistance(double t);
  void insert(PointPtr point, std::vector<NodePtr> &clusterNodes);
  NodePtr root;
  std::vector<std::vector<double>>
  calcAdjacencyMatrix(const std::vector<NodePtr> &nodes);
  NodePtr closestChild(const std::vector<NodePtr> &children, PointPtr point);
  class Node {
  private:
    NodePtr parent;
    std::vector<NodePtr> children;
    bool isLeaf = true;
    int index, dim;
    // 原CF结构体，numNodes是子类中节点的数目，LS是N个节点的线性和，SS是N个节点的平方和
    int numNodes = 0;
    std::vector<double> ls, ss;

  public:
    Node(int d = 0)
        : dim(d), ls(std::vector<double>(d, 0.0)),
          ss(std::vector<double>(d, 0.0)){};
    ~Node() = default;
    NodePtr getParent() { return parent; }
    int getIndex() const { return index; }
    const std::vector<NodePtr> &getChildren() { return children; }
    void removeChild(NodePtr child) {
      std::ranges::remove_if(children, [&](auto &c) {
        return c->getIndex() == child->getIndex();
      });
    }
    // NodePtr copy();
    bool getIsLeaf() const { return isLeaf; }
    void setIsLeaf(bool leaf) { isLeaf = leaf; }
    void setIndex(int i) { index = i; }
    void setParent(NodePtr p) { parent = p; }
    void addChild(NodePtr child) { children.push_back(child); }
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
    void update(PointPtr point, bool all) {
      for (int i = 0; i < dim; ++i) {
        auto val = point->getFeatureItem(i);
        ls[i] += val;
        ss[i] += val * val;
      }
      auto p = getParent();
      if (p != nullptr && all) {
        p->update(point, all);
      }
    }
    PointPtr centroid() {
      auto c = GenericFactory::create<Point>();
      c->setIndex(-1);
      c->setClusteringCenter(-1);
      for (int i = 0; i < ls.size(); ++i)
        c->setFeatureItem(ls[i] / dim, i);
      return c;
    }
    void setCF(int d) {
      dim = d;
      ls = std::vector<double>(d, 0.0);
      ss = std::vector<double>(d, 0.0);
    }
    void setCF(NodePtr node) {
      setCF(node->getNumNodes(), node->getLS(), node->getSS());
    }
    void setCF(int n, std::vector<double> ls, std::vector<double> rs) {
      setNumNodes(n);
      setLS(ls);
      setSS(rs);
    }
    void merge(NodePtr child) {
      setNumNodes(getNumNodes() + child->getNumNodes());
      for (int i = 0; i < dim; ++i) {
        ls[i] += child->getLS()[i];
        ss[i] += child->getSS()[i];
      }
    }
    void backwardEvolution(TreePtr tree, PointPtr point,
                           std::vector<NodePtr> &clusterNodes) {
      auto curNode = std::make_shared<Node>(*this);
      if (curNode->getParent() == nullptr) { // means current node is root node
        // l <= L, create a new leaf node and insert the point into it(root
        // change)
        auto newRoot = GenericFactory::create<Node>(dim);
        newRoot->setIsLeaf(false);
        newRoot->addChild(curNode);
        curNode->setParent(newRoot);

        auto newNode = GenericFactory::create<Node>(dim);
        newNode->setIsLeaf(true);
        newNode->setParent(newRoot);
        newRoot->setLS(curNode->getLS());
        newRoot->setSS(curNode->getSS());
        newRoot->setNumNodes(curNode->getNumNodes());
        newRoot->setIndex(tree->leafMask++);
        // here we need to remove the old root and add the new one into the
        // leafnodes set update the parent node
        newRoot->addChild(newNode);
        newNode->update(point, true);
        tree->root = newRoot;
      } else {
        auto parent = curNode->getParent();
        auto newNode = GenericFactory::create<Node>(dim);
        newNode->setIsLeaf(true);
        newNode->setParent(parent);
        parent->addChild(newNode);
        newNode->update(point, false);
        if (parent->getChildren().size() < tree->getMaxLeafNodes()) {
          // whether the number of CFs(clusters) in the current leaf node is
          // lower thant threshold L l <= L, create a new leaf node and insert
          // the point into it update the parent node and all nodes on the path
          // to root node
          parent->update(point, true);
        } else {
          // l > L, parent node of the current leaf node capacity reaches the
          // threshold L, split a new parent node from the old one
          bool CurNodeIsClus = true;
          while (true) {
            NodePtr parParent;
            if (parent->getParent() == nullptr) {
              // if the parent node is the root, we need to create a new root as
              // a parParent
              parParent = GenericFactory::create<Node>(dim);
              parParent->setIsLeaf(false);
              tree->root = parParent;
              // since the parent node's nls has not been updated by the point,
              // so we directly copy the nls in parent node to the parParent one
              parParent->setCF(parent);
            } else {
              // if the parent node is not the root, we can get the parParent
              // one directly
              parParent = parent->getParent();
            }
            // we need to create a new parent node since the old one has to
            // split
            auto newParentA = GenericFactory::create<Node>(dim);
            // insert the new parent into the allNode list
            // we also need to insert the new parent node into the clusterNode
            // list if its children is a leaf node.
            if (parent->getChildren().at(0)->getIsLeaf()) {
              newParentA->setIndex(++tree->leafMask);
              clusterNodes.push_back(newParentA);
            }
            // we only create a new parent rather and keep the old parent node
            // as the split two sub-nodes so we need to refresh the old parent
            // node as a blank one and treat it as a new parent B
            parent->setIsLeaf(false);
            newParentA->setIsLeaf(false);
            parent->setParent(parParent); // link the parparent node and the new
                                          // created new parent A
            newParentA->setParent(parParent);
            parParent->addChild(newParentA);
            // clean cf of the old parent node and initialize the cf of new
            // parent A (ls and ss all have d number of 0)
            newParentA->setCF(point->getDimension());
            parent->setCF(point->getDimension());
            // split the child nodes of the old parent nodes
            auto broNodes = parent->getChildren();
            parent->setChildren(std::vector<NodePtr>());
            auto adjMatrix = tree->calcAdjacencyMatrix(
                broNodes); //  calculate the distance between
                           //  each two brother nodes
            // choose two farthest CFs as seedA and seedB
            int seedA = 0, seedB = 0;
            double maxDis = 0;
            for (int i = 0; i < broNodes.size(); i++) {
              for (int j = i; j < broNodes.size(); j++) {
                if (maxDis < adjMatrix[i][j]) {
                  seedA = i;
                  seedB = j;
                  maxDis = adjMatrix[i][j];
                }
              }
            }
            // insert the child node into the nearest seed(A / B)
            for (auto node : broNodes) {
              node->clearParents();
            }
            // insert seedA node into new parent A and link them
            newParentA->addChild(broNodes[seedA]);
            broNodes[seedA]->setParent(newParentA);
            newParentA->merge(broNodes[seedA]);
            // insert seed B node into new parent B and link them
            parent->addChild(broNodes[seedB]);
            broNodes[seedB]->setParent(parent);
            parent->merge(broNodes[seedB]);
            // if other one brother node is near seed A then split it into new
            // parent A, otherwise new parent B.
            for (int i = 0; i < broNodes.size(); i++) {
              if (i != seedA && i != seedB) {
                if (adjMatrix[i][seedA] < adjMatrix[i][seedB]) {
                  newParentA->addChild(broNodes[i]);
                  newParentA->merge(
                      broNodes[i]); // since the brother nodes list contains the
                                    // one we insert our point, so after this
                                    // function, the parent node's nls are also
                                    // updated.
                  broNodes[i]->clearParents();
                  broNodes[i]->setParent(newParentA);
                } else {
                  parent->addChild(broNodes[i]);
                  parent->merge(broNodes[i]);
                  broNodes[i]->clearParents();
                  broNodes[i]->setParent(parent);
                }
              }
            }
            // if the current node(parent) is a cluster nodes, then we need to
            // update the nls of its parent using new point. we only update the
            // parparent in the first loop.
            if (CurNodeIsClus) {
              parParent->update(point, true);
            }

            if (parParent->getChildren().size() <=
                tree->getMaxInternalNodes()) {
              // b < B, remove the old node and insert the new nodeA and nodeB
              // into the parent node.
              break;
            } else {
              // b >= B, parent node of the current interior node capacity
              // reaches the threshold B.
              curNode = curNode->getParent();
              parent = parParent;
              CurNodeIsClus = false;
            }
          }
        }
      }
    }
  };
};

} // namespace SESAME
#endif // SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CFTREE_HPP_
