//
// Created by tuidan on 2021/9/9.
//

#include <Algorithm/DataStructure/OutlierReservoir.hpp>
#include <Algorithm/DataStructure/DataStructureFactory.hpp>
#include <cfloat>
#include <cmath>


/**
 * OutlierReservoir
 */
SESAME::OutlierReservoir::OutlierReservoir() = default;
SESAME::OutlierReservoir::~OutlierReservoir() = default;
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
const std::unordered_set<SESAME::DPNodePtr> &SESAME::OutlierReservoir::GetOutliers() const {
  return outliers;
}
void SESAME::OutlierReservoir::SetOutliers(const std::unordered_set<SESAME::DPNodePtr> &outliers) {
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
void SESAME::OutlierReservoir::insert(SESAME::DPNodePtr &c) {
  c->SetDelta(DBL_MAX);
  if(c->GetDep() != nullptr){
    std::unordered_set<SESAME::DPNodePtr> successors = c->GetDep()->GetSucs();
    successors.erase(c);
  }
  c->SetDep(nullptr);
  this->outliers.insert(c);
}
SESAME::DPNodePtr SESAME::OutlierReservoir::insert(SESAME::PointPtr &p, double time) {
  double dis = 0;
  auto minDis = DBL_MAX;
  SESAME::DPNodePtr nn = nullptr;
 // SESAME::DPNodePtr temp = nullptr;
  for(auto it = this->outliers.begin(); it != this->outliers.end(); ++it){
    if(time - double(it->get()->GetLastTime()) > this->timeGap) {
      this->outliers.erase(it);
      continue;
    }
    dis = p->getDisTo(it->get()->GetCenter());
    if (dis < minDis) {
      minDis = dis;
      nn = it->get()->copy();
    }
  }
  if (nn == nullptr || minDis > r) {
    SESAME::DPNodePtr c = std::make_shared<SESAME::DPNode>(p, time);
    this->outliers.insert(c);
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
void SESAME::OutlierReservoir::remove(SESAME::DPNodePtr &nn) {
  this->outliers.erase(nn);
}
