//
// Created by tuidan on 2021/9/9.
//

#include <Algorithm/DataStructure/DPNode.hpp>
#include <cfloat>
#include <cmath>
/**
 * DPNode
 */
static int id = 0;
SESAME::DPNode::DPNode() {
}

SESAME::DPNode::~DPNode() {}

int SESAME::DPNode::GetId() const {
  return cid;
}
void SESAME::DPNode::SetId(int id) {
  DPNode:: cid = id;
}
int SESAME::DPNode::GetCId() const {
  return Cid;
}
void SESAME::DPNode::SetCId(int Cid) {
  DPNode:: Cid = Cid;
}
int SESAME::DPNode::GetNum() const {
  return num;
}
void SESAME::DPNode::SetNum(int num) {
  DPNode::num = num;
}
double SESAME::DPNode::GetRho() const {
  return rho;
}
void SESAME::DPNode::SetRho(double rho) {
  DPNode::rho = rho;
}
double SESAME::DPNode::GetDelta() const {
  return delta;
}
void SESAME::DPNode::SetDelta(double delta) {
  DPNode::delta = delta;
}
const SESAME::DPNodePtr &SESAME::DPNode::GetDep() const {
  return dep;
}
void SESAME::DPNode::SetDep(const SESAME::DPNodePtr &dep) {
  DPNode::dep = dep;
}
const SESAME::PointPtr &SESAME::DPNode::GetCenter() const {
  return center;
}
void SESAME::DPNode::SetCenter(const SESAME::PointPtr &center) {
  DPNode::center = center;
}
double SESAME::DPNode::GetLastTime() const {
  return lastTime;
}
void SESAME::DPNode::SetLastTime(double last_time) {
  lastTime = last_time;
}
bool SESAME::DPNode::IsActive() const {
  return active;
}
void SESAME::DPNode::SetActive(bool active) {
  DPNode::active = active;
}
const std::vector<SESAME::DPNodePtr> &SESAME::DPNode::GetSucs() const {
  return sucs;
}
void SESAME::DPNode::SetSucs(const std::vector<SESAME::DPNodePtr> &sucs) {
  DPNode::sucs = sucs;
}
const SESAME::ClusterPtr &SESAME::DPNode::GetCluster() const {
  return cluster;
}
void SESAME::DPNode::SetCluster(const SESAME::ClusterPtr &cluster) {
  DPNode::cluster = cluster;
}
double SESAME::DPNode::GetInactiveTime() const {
  return inactiveTime;
}
void SESAME::DPNode::SetInactiveTime(double inactive_time) {
  inactiveTime = inactive_time;
}
double SESAME::DPNode::GetDis() const {
  return dis;
}
void SESAME::DPNode::SetDis(double dis) {
  DPNode::dis = dis;
}

SESAME::DPNode::DPNode(SESAME::PointPtr p, double time) {
  this->cid = id++;
  this->rho = 1;
  this->delta = FLT_MAX;
  this->dep = nullptr;
  this->center = p->copy();
  this->lastTime = time;
  this->active = false;
  this->Cid = 0;
  this->inactiveTime = 0;
  this->dis = 0;
}


/**
 * Cluster
 */
int SESAME::Cluster::GetLabel() const {
  return label;
}
void SESAME::Cluster::SetLabel(int label) {
  Cluster::label = label;
}
const std::vector<SESAME::DPNodePtr> &SESAME::Cluster::GetCells() const {
  return cells;
}
void SESAME::Cluster::SetCells(const std::vector<DPNodePtr> &cells) {
  Cluster::cells = cells;
}
SESAME::Cluster::Cluster(int label) {
  this->label = label;
}
void SESAME::Cluster::add(DPNodePtr &node) {
  this->cells.push_back(node);
}
void SESAME::Cluster::remove(DPNodePtr &node) {
  for(int i = 0; i < this->cells.size(); i++) {
    if(this->cells.at(i)->GetId() == node->GetId()) {
      this->cells.erase(this->cells.begin() + i);
    }
  }
}

void SESAME::DPNode::insert(double time) {
  this->rho++;
  this->lastTime = time;
}
void SESAME::DPNode::add(double coef, double time) {
  this->rho = coef * this->rho + 1;
  this->lastTime = time;
}
void SESAME::DPNode::addSuccessor(SESAME::DPNodePtr &node) {
  this->sucs.push_back(node);
}
void SESAME::DPNode::removeSuccessor(SESAME::DPNodePtr &node) {
  for(int i = 0; i < this->sucs.size(); i++){
    if(this->sucs.at(i)->GetId() == node->GetId()) {
      this->sucs.erase(this->sucs.begin() + i);
    }
  }
}
bool SESAME::DPNode::hasSuccessor() {
  return !sucs.empty();
}
double SESAME::DPNode::getDisTo(SESAME::DPNodePtr &node) {
  double distance = 0;
  for(int i = 0; i < this->center->getDimension(); i++){
    distance = distance + pow(node->center->getFeatureItem(i) - this->center->getFeatureItem(i), 2);
  }
  return sqrt(distance);
}
SESAME::DPNodePtr SESAME::DPNode::copy() {
  return std::make_shared<DPNode>(*this);
}

