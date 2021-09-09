//
// Created by tuidan on 2021/9/9.
//

#include <Algorithm/DataStructure/Cache.hpp>
#include <cfloat>
#include <cmath>
#include <algorithm>
bool cmp(SESAME::DPNodePtr c1,SESAME::DPNodePtr c2){
  return c1->GetRho() > c2->GetRho() ? -1 : 1;
}
SESAME::Cache::Cache(int num, double a, double lamd, double r) {
  std::vector<DPNodePtr> blankNode(10000);
  this->buffer = blankNode;
  this->num = num;
  this->pnum = 0;
  this->size = 0;
  this->a = a;
  this->lamd = lamd;
  this->r = r;
}
SESAME::Cache::~Cache() = default;
SESAME::Cache::Cache() = default;
SESAME::DPNodePtr SESAME::Cache::add(SESAME::PointPtr p, double startTime) {
  this->pnum++;
  double dis = FLT_MAX;
  double minDis = FLT_MAX;
  SESAME::DPNodePtr nn;
  for (int i = 0; i < size; i++) {
    dis = p->getDisTo(buffer[i]->GetCenter());
    if (dis < minDis) {
      minDis = dis;
      nn = buffer[i];
    }
  }
  if (minDis <= r) {
    double coef = pow(a, lamd * double (startTime - nn->GetLastTime()));
    nn->add(coef, startTime);
    return nn;
  } else {
    SESAME::DPNodePtr c = std::make_shared<DPNode>(p, startTime);
    buffer[size] = c;
    size++;
    return c;
  }
}
bool SESAME::Cache::isFull() {
  return pnum == num;
}
void SESAME::Cache::compDeltaRho(double time) {
  std::vector<DPNodePtr> blankNode(size);
  clus = blankNode;
  for (int i = 0; i < size; i++) {
    buffer[i]->SetRho((float) (pow(a, lamd * (time - buffer[i]->GetLastTime())) * buffer[i]->GetRho()));
    clus[i] = buffer[i];
  }
  sort(clus.begin(), clus.end(), cmp);

  double dis = 0;
  clus[0]->SetDelta(0);
  for (int i = 1; i < size; i++) {
    DPNodePtr cc = clus[i];
    auto minDis = DBL_MAX;
    for (int j = i - 1; j >= 0; j--) {
      dis = cc->GetCenter()->getDisTo(clus[j]->GetCenter());
      if (minDis > dis) {
        minDis = dis;
        cc->SetDep(clus[j]);
      }
    }
    cc->SetDelta(minDis);
    if (clus[0]->GetDelta() < minDis) {
      clus[0]->SetDelta(minDis);
    }
  }
}
