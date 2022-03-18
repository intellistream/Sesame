//
// Created by tuidan on 2021/8/24.
//

#include "Algorithm/DataStructure/CFTree.hpp"
#include "Algorithm/DataStructure/GenericFactory.hpp"


#include <limits>
#include <vector>

namespace SESAME {

CFTree::CFTree(const StreamClusteringParam &param)
    : maxInternalNodes(param.maxInternalNodes),
      maxLeafNodes(param.maxLeafNodes),
      thresholdDistance(param.thresholdDistance) {}

CFTree::CFTree(int b = 0, int l = 0, double t = 0.0)
    : maxInternalNodes(b), maxLeafNodes(l), thresholdDistance(t) {}

CFTree::~CFTree() {}
int CFTree::getB() const { return this->maxInternalNodes; }
int CFTree::getL() const { return this->maxLeafNodes; }
double CFTree::getT() const { return this->thresholdDistance; }
void CFTree::setB(int b) { this->maxInternalNodes = b; }
void CFTree::setT(double t) { this->thresholdDistance = t; }
void CFTree::setL(int l) { this->maxLeafNodes = l; }

CFPtr CFNode::getCF() { return this->curCF; }
NodePtr CFNode::getParent() { return this->parent; }
int CFNode::getIndex() const { return this->index; }
std::vector<NodePtr> CFNode::getChildren() { return this->children; }

void CFNode::setNode(CFPtr &Node) { this->curCF = Node; }
void CFNode::setParent(NodePtr &Parent) { this->parent = Parent->copy(); }
void CFNode::setIndex(int Index) { this->index = Index; }
void CFNode::setChild(NodePtr &child) { this->children.push_back(child); }
void CFNode::setChildren(std::vector<NodePtr> children) {
  this->children = children;
}
CFNode::CFNode() {
  this->curCF = std::make_shared<CF>();
  this->isLeaf = true;
}
CFNode::~CFNode() {}
bool CFNode::getIsLeaf() { return this->isLeaf; }
void CFNode::setIsLeaf(bool leaf) { this->isLeaf = leaf; }
NodePtr CFNode::copy() { return std::make_shared<CFNode>(*this); }
void CFNode::setCF(CFPtr &cf) {
  this->curCF->setN(cf->getN());
  std::vector<double> ls = cf->getLS();
  std::vector<double> ss = cf->getSS();
  this->curCF->setLS(ls);
  this->curCF->setSS(ss);
}
// when need to clear the parent node, directly set the index = -1
void CFNode::clearParents() { this->parent->setIndex(-1); }
void CFNode::removeChild(NodePtr &child) {
  for (int i = 0; i < this->children.size(); i++) {
    if (this->children[i]->getIndex() == child->getIndex()) {
      this->children.erase(this->children.begin() + i);
    }
  }
}
bool SESAME::CFNode::getIsOutlier() {
  return this->isOutlier;
}
void SESAME::CFNode::setIsOutlier(bool flag) {
  this->isOutlier = flag;
}


ClusteringFeaturesTree::ClusteringFeaturesTree(int maxInternalNodes = 0,
                                               int maxLeafNodes = 0,
                                               double thresholdDistance = 0.0)
    : maxInternalNodes(maxInternalNodes), maxLeafNodes(maxLeafNodes),
      thresholdDistance(thresholdDistance),
      root(GenericFactory::create<Node>()) {}

ClusteringFeaturesTree::~ClusteringFeaturesTree() {}
int ClusteringFeaturesTree::getMaxInternalNodes() const {
  return this->maxInternalNodes;
}
int ClusteringFeaturesTree::getMaxLeafNodes() const {
  return this->maxLeafNodes;
}
double ClusteringFeaturesTree::getThresholdDistance() const {
  return this->thresholdDistance;
}
void ClusteringFeaturesTree::setMaxInternalNodes(int b) {
  this->maxInternalNodes = b;
}
void ClusteringFeaturesTree::setThresholdDistance(double t) {
  this->thresholdDistance = t;
}
void ClusteringFeaturesTree::setMaxLeafNodes(int l) { this->maxLeafNodes = l; }


ClusteringFeaturesTree::ClusteringFeaturesTree(
    const StreamClusteringParam &param)
    : maxInternalNodes(param.maxInternalNodes),
      maxLeafNodes(param.maxLeafNodes),
      thresholdDistance(param.thresholdDistance),
      root(GenericFactory::create<Node>(param.dimension)) {}

void ClusteringFeaturesTree::insert(
    PointPtr point,
    std::vector<ClusteringFeaturesTree::NodePtr> &clusterNodes,
    std::vector<ClusteringFeaturesTree::NodePtr> &outliers) {
  auto curNode = root;
  if (curNode->getNumNodes() == 0) {
    // timerMeter.dataInsertAccMeasure();
    curNode->update(point, true);
    // timerMeter.dataInsertEndMeasure();
  } else {
    while (1) {
      auto childrenNode = curNode->getChildren();
      if (curNode->getIsLeaf()) {
        // timerMeter.clusterUpdateAccMeasure();
        // timerMeter.dataInsertAccMeasure();
        auto centroid = curNode->centroid();
        // timerMeter.dataInsertEndMeasure();
        if (point->radius(centroid) <=
            getThresholdDistance()) { // concept drift detection
          // whether the new radius is lower than threshold T
          // timerMeter.dataInsertAccMeasure();
          curNode->update(point, true);
          // TODO
          
          // timerMeter.dataInsertEndMeasure();

          // means this point could get included in this cluster
          // SESAME_DEBUG("No concept drift occurs(t <= T), insert tha point
          // into the leaf node...");
          break;
          // Normally insert the data point into the tree leafNode without
          // concept drift
        } else {
          // concept drift adaption
          // SESAME_DEBUG("Concept drift occurs(t > T), the current leaf node
          // capacity reaches the threshold T");
          // timerMeter.clusterUpdateAccMeasure();
          curNode->backwardEvolution(shared_from_this(), point, clusterNodes);
          // timerMeter.clusterUpdateEndMeasure();
          break;
        }

      } else {
        // timerMeter.dataInsertAccMeasure();
        curNode = closestChild(childrenNode, point);
        // timerMeter.dataInsertEndMeasure();
      }
    }
  }
}

std::vector<std::vector<double>> ClusteringFeaturesTree::calcAdjacencyMatrix(
    const std::vector<ClusteringFeaturesTree::NodePtr> &nodes) {
  int n = nodes.size();
  std::vector<std::vector<double>> adjacencyMatrix(n,
                                                   std::vector<double>(n, 0.0));
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      auto centroid1 = nodes[i]->centroid();
      auto centroid2 = nodes[j]->centroid();
      auto distance = centroid1->distance(centroid2);
      adjacencyMatrix[i][j] = distance;
      adjacencyMatrix[j][i] = distance;
    }
  }
  return adjacencyMatrix;
}

ClusteringFeaturesTree::NodePtr ClusteringFeaturesTree::closestChild(
    const std::vector<ClusteringFeaturesTree::NodePtr> &children,
    PointPtr point) {
  double minDistance = std::numeric_limits<double>::max();
  NodePtr closestChild = nullptr;
  for (auto child : children) {
    auto centroid = child->centroid();
    auto distance = centroid->distance(point);
    if (distance < minDistance) {
      minDistance = distance;
      closestChild = child;
    }
  }
  return closestChild;
}

} // namespace SESAME
