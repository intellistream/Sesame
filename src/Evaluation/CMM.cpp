//
// Created by tuidan on 2021/10/11.
//

#include "Evaluation/CMM.hpp"
#include "Utils/Logger.hpp"
#include "Utils/UtilityFunctions.hpp"

#include <cassert>
#include <cmath>
#include <numeric>

/**
 * @Description: CMMPoint
 */
SESAME::CMMPoint::CMMPoint(int id, long startTime, long time,
                           std::vector<double> &vec, double a, double lambda,
                           int truth) {
  this->id = id;
  this->startTime = startTime;
  this->dim = (int)vec.size();
  this->vec = vec;
  this->weight = pow(a, lambda * (time - startTime));
  this->truth = truth;
}

double SESAME::CMMPoint::getDisTo(CMMPointPtr &p) {
  double dis = 0;
  double temp = 0;
  for (int i = 0; i < dim; i++) {
    temp = p->vec[i] - vec[i];
    dis += temp * temp;
  }
  return sqrt(dis);
}

double SESAME::CMMPoint::knnDis(int k, SESAME::CMMCluster &c) {
  std::vector<CMMPointPtr> list = c.points;
  int size = c.points.size();
  std::vector<double> diss(size);
  for (int i = 0; i < size; i++) {
    CMMPointPtr p = list.at(i);
    if (list.at(i)->id != this->id) {
      diss[i] = getDisTo(list.at(i));
    }
  }
  sort(diss.begin(), diss.end());
  double sum = 0;
  int num = 0;
  for (int i = 0; i < k && i < size; i++) {
    sum += diss[i];
    num++;
  }
  return sum / num;
}

/**
 * @Description: CMMCluster
 */
SESAME::CMMCluster::CMMCluster() {}

void SESAME::CMMCluster::add(SESAME::CMMPointPtr &p) {
  this->points.push_back(p);
}

void SESAME::CMMCluster::getConn() {
  int size = (int)points.size();
  std::vector<double> knhPDis(size);
  double sum = 0;
  for (int i = 0; i < size; i++) {
    CMMPointPtr p = points.at(i);
    knhPDis[i] = p->knnDis(CMM_KNN, *this);
    // the paper says that the value of k has only marginal influence on CMM
    // effectiveness. so here we set k = 10
    sum += knhPDis[i];
  }
  knhDis = sum / size;
  for (int i = 0; i < size; i++) {
    if (type == SESAME::GTCluster) {
      if (knhPDis[i] < knhDis) {
        points.at(i)->conCL = 1;
      } else {
        if (points.size() == 1) {
          points.at(i)->conCL = 1;
        } else {
          points.at(i)->conCL = knhDis / knhPDis[i];
        }
      }
    } else {
      if (knhPDis[i] < knhDis) {
        points.at(i)->con = 1;
      } else {
        points.at(i)->con = knhDis / knhPDis[i];
      }
    }
  }
}

SESAME::CMMDriver::CMMDriver(int dim, double a, double lambda) {
  this->dim = dim;
  this->a = a;
  this->lambda = lambda;
  // this->k = k;
}
void SESAME::CMMDriver::load(const std::vector<PointPtr> &inputs,
                             const std::vector<PointPtr> &predicts, int dim,
                             double time) {
  // time ? weight ?
  // convert to the predicted clustering center index
  for (int i = 0; i < inputs.size(); i++) {
    std::vector<double> features;
    for (int j = 0; j < dim; j++) {
      features.push_back(inputs.at(i)->getFeatureItem(j));
    }
    int cl = inputs.at(i)->getClusteringCenter();
    assert(cl != -1);
    SESAME::CMMPointPtr p = std::make_shared<CMMPoint>(
        predicts.at(i)->getIndex(), (long)predicts.at(i)->getIndex(), time,
        features, a, lambda, cl);
    if (CL.count(cl)) {
      CL[cl]->add(p);
    } else {
      CMMClusterPtr c = std::make_shared<CMMCluster>();
      c->groundTruth = cl;
      c->type = SESAME::GTCluster;
      c->add(p);
      CL.insert(std::pair<int, CMMClusterPtr>(cl, c));
      CLlist.push_back(c);
    }
    int ci = predicts.at(i)->getClusteringCenter();
    if(C.count(ci)){
      C[ci]->add(p);
    } else {
      CMMClusterPtr c = std::make_shared<CMMCluster>();
      ;
      c->groundTruth = ci;
      c->type = SESAME::Cluster;
      c->add(p);
      C.insert(std::pair<int, CMMClusterPtr>(ci, c));
      Clist.push_back(c);
    }
  }
}
void SESAME::CMMDriver::voteMap() {
  /*
    here CMM uses the mapping strategy rather than the vote, and in the paper
    the author also says that Mapping clusters based on majority voting cannot
    recognize emerging or disappearing clusters.
  */
  int csize = (int)Clist.size();
  for (int i = 0; i < csize; i++) {
    CMMClusterPtr c = Clist.at(i);
    std::unordered_map<int, double> map;
    std::vector<CMMPointPtr> list = c->points;
    int psize = (int)list.size();
    for (int j = 0; j < psize; j++) {
      CMMPointPtr p = list.at(j);
      int truth = p->truth;
      if (map.count(truth)) {
        map.insert(std::pair<int, double>(truth, map.at(truth) + p->weight));
      } else {
        map.insert(std::pair<int, double>(truth, p->weight));
      }
    }
    int label = 1;
    double max = 0;
    for (auto &m : map) {
      if (m.second > max) {
        max = m.second;
        label = m.first;
        assert(label != -1);
      }
    }
    c->groundTruth = label;
  }
}

void SESAME::CMMDriver::getFaultSet() {
  int csize = (int)Clist.size();
  for (int i = 0; i < csize; i++) {
    CMMClusterPtr c = Clist.at(i);
    int truth = c->groundTruth;
    std::vector<CMMPointPtr> list = c->points;
    int psize = list.size();
    for (int j = 0; j < psize; j++) {
      CMMPointPtr p = list.at(j);
      if (p->truth != truth) {
        faultSet.push_back(p);
        if (!faultClu.contains(c)) {
          faultClu.insert(c);
          faultClu.insert(CL.at(p->truth));
        }
      }
    }
  }
}

double SESAME::CMMDriver::compCMM() {
  getFaultSet();
  if (faultSet.empty()) {
    return 1;
  }
  compCon();
  double totalPen = 0;
  double totalCon = 0;
  int faultPsize = (int)faultSet.size();
  for (int i = 0; i < faultPsize; i++) {
    CMMPointPtr p = faultSet.at(i);
    totalPen += p->weight * p->conCL * (1 - p->con);
    totalCon += p->weight * p->conCL;
  }
  if (totalCon == 0) {
    return 0;
  } else
    return 1 - totalPen / totalCon;
}

void SESAME::CMMDriver::compCon() {
  for (auto &c : faultClu) {
    c->getConn();
  }
}

double SESAME::CMMDriver::computeWeight(double deltaTime) {
  const double belta = 2;
  const double lamda = 1;
  return pow(belta, lamda * (deltaTime));
}
/*here we assume that the data comes every 10 seconds
 * */
double SESAME::CMM::CMMCost(int dim, const std::vector<PointPtr> &inputs,
                            const std::vector<PointPtr> &predicts) {
  std::vector<double> CMMValues;
  CMMValues.push_back(0);
  int start = 0;
  for (int i = 25; i < inputs.size(); i += 25) {
    // segment the stream data into horizons(windows) according to threshold
    CMMDriver cmm(dim, CMM_A, CMM_LAMDA);
    std::vector<PointPtr> seg;
    for (; start < i; start++) {
      seg.push_back(inputs.at(start));
    }
    cmm.load(seg, predicts, dim, i * 10);
    cmm.voteMap();
    double cmmValue = cmm.compCMM();
    //      cmm.voteMap();  // TODO: change voteMap according to the paper
    CMMValues.push_back(cmmValue);
    //      std::cout << "cmm: " << cmmValue << std::endl;
  }
  double sum = std::accumulate(std::begin(CMMValues), std::end(CMMValues), 0.0);
  return sum / (CMMValues.size() - 1);
}
