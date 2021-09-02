//
// Created by tuidan on 2021/9/1.
//

#include <Algorithm/DataStructure/DPTree.hpp>
#include <cfloat>
#include <cmath>
/**
 * DPNode
 */
SESAME::DPNode::DPNode() {}

SESAME::DPNode::~DPNode() {}

int SESAME::DPNode::GetId() const {
  return id;
}
void SESAME::DPNode::SetId(int id) {
  DPNode::id = id;
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
long SESAME::DPNode::GetLastTime() const {
  return lastTime;
}
void SESAME::DPNode::SetLastTime(long last_time) {
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
long SESAME::DPNode::GetInactiveTime() const {
  return inactiveTime;
}
void SESAME::DPNode::SetInactiveTime(long inactive_time) {
  inactiveTime = inactive_time;
}
double SESAME::DPNode::GetDis() const {
  return dis;
}
void SESAME::DPNode::SetDis(double dis) {
  DPNode::dis = dis;
}

SESAME::DPNode::DPNode(SESAME::PointPtr p, int index, long time) {
  this->id = index;
  this->rho = 1;
  this->delta = FLT_MAX;
  this->dep = nullptr;
  this->center = p->copy();
  this->lastTime = time;
  this->active = false;
}


/**
 * Cluster
 */
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

void SESAME::DPNode::insert(long time) {
  this->rho++;
  this->lastTime = time;
}
void SESAME::DPNode::add(double coef, long time) {
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

/**
 * OutlierReservoir
 */
double SESAME::OutlierReservoir::GetR() const {
  return r;
}
void SESAME::OutlierReservoir::SetR(double r) {
  OutlierReservoir::r = r;
}
double SESAME::OutlierReservoir::GetTimeGap() const {
  return timeGap;
}
void SESAME::OutlierReservoir::SetTimeGap(double time_gap) {
  timeGap = time_gap;
}
long SESAME::OutlierReservoir::GetLastDelTime() const {
  return lastDelTime;
}
void SESAME::OutlierReservoir::SetLastDelTime(long last_del_time) {
  lastDelTime = last_del_time;
}
double SESAME::OutlierReservoir::GetA() const {
  return a;
}
void SESAME::OutlierReservoir::SetA(double a) {
  OutlierReservoir::a = a;
}
double SESAME::OutlierReservoir::GetLamd() const {
  return lamd;
}
void SESAME::OutlierReservoir::SetLamd(double lamd) {
  OutlierReservoir::lamd = lamd;
}
const std::vector<SESAME::DPNodePtr> &SESAME::OutlierReservoir::GetOutliers() const {
  return outliers;
}
void SESAME::OutlierReservoir::SetOutliers(const std::vector<SESAME::DPNodePtr> &outliers) {
  OutlierReservoir::outliers = outliers;
}
SESAME::OutlierReservoir::OutlierReservoir(double r, double a, double lamd) {
  this->r = r;
  this->a = a;
  this->lamd = lamd;
}
void SESAME::OutlierReservoir::setTimeGap(double timeGap) {
  this->timeGap = timeGap;
}
void SESAME::OutlierReservoir::insert(SESAME::DPNodePtr c) {
  c->SetDelta(DBL_MAX);
  if(c->GetDep() != nullptr){
    std::vector<SESAME::DPNodePtr> successors = c->GetDep()->GetSucs();
    for(int i = 0; i < successors.size(); i++) {
      if(successors.at(i)->GetId() == c->GetId()) {
        successors.erase(successors.begin() + i);
      }
    }
  }
  c->SetDep(nullptr);
  this->outliers.push_back(c);
}
SESAME::DPNodePtr SESAME::OutlierReservoir::insert(SESAME::PointPtr p, int curNodeNum, double time) {
  double dis = 0;
  double minDis = DBL_MAX;
  SESAME::DPNodePtr nn = nullptr;
  SESAME::DPNodePtr temp = nullptr;
  for(int i = 0; i < this->outliers.size(); i++){
    SESAME::DPNodePtr out = this->outliers.at(i);
    if(time - double(out->GetLastTime()) > this->timeGap) {
      this->outliers.erase(this->outliers.begin() + i);
    }
  }
  dis = p->getDisTo(temp->GetCenter());
  if (dis < minDis) {
    minDis = dis;
    nn = temp;
  }

  if (nn == nullptr || minDis > r) {
    SESAME::DPNodePtr c = std::make_shared<SESAME::DPNode>(p, curNodeNum + 1, time);
    this->outliers.push_back(c);
    return c;
  } else {
    double coef = pow(a, lamd * (time - nn->GetLastTime()));
//			System.out.println(p.startTime + " " + nn.lastTime);
//			System.out.println(minDis);
//			System.out.println(coef);
    nn->add(coef, time);
    return nn;
  }
}
void SESAME::OutlierReservoir::remove(SESAME::DPNodePtr nn) {
  for(int i = 0; i < this->outliers.size(); i++) {
    if(this->outliers.at(i)->GetId() == nn->GetId()) {
      this->outliers.erase(this->outliers.begin() + i);
    }
  }
}

/**
 * DPTree
 */
SESAME::DPTree::DPTree() {}
SESAME::DPTree::~DPTree() {}
SESAME::DPTree::DPTree(int num, double CluR) {
  this->num = num;
  this->CluR = CluR;
  this->cluLabel = 0;
}
