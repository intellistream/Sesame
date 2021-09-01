//
// Created by tuidan on 2021/9/1.
//

#include <Algorithm/DataStructure/DPTree.hpp>
#include <cfloat>
SESAME::DPNode::DPNode() {}

SESAME::DPNode::~DPNode() {}

SESAME::DPNode::DPNode(SESAME::PointPtr p, long startTime) {
  this->cid = id++;
  this->rho = 1;
  this->delta = FLT_MAX;
  this->dep = nullptr;
  this->center = p->copy();
  this->lastTime = startTime;
  this->active = false;
}
void SESAME::DPNode::insert(SESAME::PointPtr p) {

}
void SESAME::DPNode::add(SESAME::PointPtr p, double coef) {

}
void SESAME::DPNode::addSuccessor(SESAME::DPNodePtr &node) {
  this->sucs.push_back(node);
}
void SESAME::DPNode::removeSuccessor(SESAME::DPNodePtr &node) {

}
bool SESAME::DPNode::hasSuccessor() {
  return false;
}

