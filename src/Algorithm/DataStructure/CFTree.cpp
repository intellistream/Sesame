//
// Created by tuidan on 2021/8/24.
//

#include <Algorithm/DataStructure/CFTree.hpp>
SESAME::CFTree::CFTree(int b, int l, double t) {
  this->maxInternalNodes = b;
  this->maxLeafNodes = l;
  this->thresholdDistance = t;
}
SESAME::CFTree::~CFTree() {

}
int SESAME::CFTree::getB() const {
  return this->maxInternalNodes;
}
int SESAME::CFTree::getL() const {
  return this->maxLeafNodes;
}
double SESAME::CFTree::getT() const {
  return this-> thresholdDistance;
}
void SESAME::CFTree::setB(int b) {
  this->maxInternalNodes = b;
}
void SESAME::CFTree::setT(double t) {
  this->thresholdDistance = t;
}
void SESAME::CFTree::setL(int l) {
  this->maxLeafNodes = l;
}
void SESAME::CFTree::initialTree(int b, int l, double t) {
  this->maxInternalNodes = b;
  this->maxLeafNodes = l;
  this->thresholdDistance = t;
}

SESAME::CFPtr SESAME::CFNode::getCF() {
  return this->curCF;
}
SESAME::NodePtr SESAME::CFNode::getParent() {
  return this->parent;
}
int SESAME::CFNode::getIndex() const {
  return this->index;
}
std::vector<SESAME::NodePtr> SESAME::CFNode::getChildren() {
  return this->children;
}

void SESAME::CFNode::setNode(CFPtr &Node) {
  this->curCF = Node;
}
void SESAME::CFNode::setParent(NodePtr &Parent) {
  this->parent = Parent->copy();
}
void SESAME::CFNode::setIndex(int Index) {
  this->index = Index;
}
void SESAME::CFNode::setChild(NodePtr &child) {
  this->children.push_back(child);
}
SESAME::CFNode::CFNode() {
  this->curCF = std::make_shared<CF>();
  this->isLeaf = true;
}
SESAME::CFNode::~CFNode() {

}
bool SESAME::CFNode::getIsLeaf() {
  return this->isLeaf;
}
void SESAME::CFNode::setIsLeaf(bool leaf) {
  this->isLeaf = leaf;
}
SESAME::NodePtr SESAME::CFNode::copy() {
  return std::make_shared<CFNode>(*this);
}
void SESAME::CFNode::setCF(CFPtr &cf) {
  this->curCF->setN(cf->getN());
  std::vector<double> ls = cf->getLS();
  std::vector<double> ss = cf->getSS();
  this->curCF->setLS(ls);
  this->curCF->setSS(ss);
}
// when need to clear the parent node, directly set the index = -1
void SESAME::CFNode::clearParents() {
  this->parent->setIndex(-1);
}
void SESAME::CFNode::removeChild(NodePtr &child) {
  for(int i = 0; i < this->children.size(); i++){
    if(this->children[i]->getIndex() == child->getIndex()) {
      this->children.erase(this->children.begin() + i);
    }
  }
}

