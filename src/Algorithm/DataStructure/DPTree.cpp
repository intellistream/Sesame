//
// Created by tuidan on 2021/9/1.
//

#include <Algorithm/DataStructure/DPTree.hpp>
#include <cfloat>
SESAME::DPNode::DPNode() {}

SESAME::DPNode::~DPNode() {}

SESAME::DPNode::DPNode(SESAME::PointPtr p, long time) {
  this->cid = id++;
  this->rho = 1;
  this->delta = FLT_MAX;
  this->dep = nullptr;
  this->center = p->copy();
  this->lastTime = time;
  this->active = false;
}
void SESAME::DPNode::insert(SESAME::PointPtr p,long time) {
  this->rho++;
  this->lastTime = time;
}
void SESAME::DPNode::add(SESAME::PointPtr p, double coef, long time) {
  this->rho = coef * this->rho + 1;
}
void SESAME::DPNode::addSuccessor(SESAME::DPNodePtr &node) {
  this->sucs.push_back(node);
}
void SESAME::DPNode::removeSuccessor(SESAME::DPNodePtr &node) {
  for(int i = 0; i < this->sucs.size(); i++){
    if(this->sucs.at(i)->getcid() == node->getcid()) {
      this->sucs.erase(this->sucs.begin() + i);
    }
  }
}
bool SESAME::DPNode::hasSuccessor() {
  return !sucs.empty();
}

SESAME::DPTree::DPTree() {}
SESAME::DPTree::~DPTree() {}
SESAME::DPTree::DPTree(int num, double CluR) {
  this->num = num;
  this->CluR = CluR;
  this->cluLabel = 0;
}
