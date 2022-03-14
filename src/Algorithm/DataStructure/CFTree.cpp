//
// Created by tuidan on 2021/8/24.
//

#include "Algorithm/DataStructure/CFTree.hpp"
#include "Algorithm/DataStructure/GenericFactory.hpp"

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

ClusteringFeaturesTree::ClusteringFeaturesTree(
    const StreamClusteringParam &param)
    : ClusteringFeaturesTree(param.maxInternalNodes, param.maxLeafNodes,
                             param.thresholdDistance) {}

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

} // namespace SESAME
