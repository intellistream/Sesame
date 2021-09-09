//
// Created by tuidan on 2021/9/1.
//

#include <Algorithm/DataStructure/DPTree.hpp>
#include <cfloat>
#include <cmath>


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
void SESAME::DPTree::insert(SESAME::DPNodePtr cc, double minRho, SESAME::PointPtr p, int opt) {
  cc->SetActive(true);
  Clus[size] = cc;
  size++;

  if (opt == 0) {
    adjustNoOpt(size - 1, minRho, p);
  }
  if (opt == 1) {
    adjustOpt1(size - 1, minRho, p);
  }

  if (opt == 2) {
    adjust(size - 1, minRho, p);
  }

  if (opt == -1) {
    adjustNoDelta(size - 1, minRho, p);
  }

  if (size == num) {
    std::cout << "lack of DPTree nodes";
  }}
void SESAME::DPTree::init(std::vector<SESAME::DPNodePtr> clus,
                          int size,
                          double minRho,
                          double minDelta,
                          SESAME::OutPtr outs,
                          std::vector<ClusterPtr> clusters) {
  this->minDelta = minDelta;
  Clus[0] = clus[0];
  SESAME::ClusterPtr cluster = std::make_shared<Cluster>(cluLabel++);;
  cluster->add(Clus[0]);
  clusters.push_back(cluster);
  int i = 1;

  for (; i < size && clus[i]->GetRho() >= minRho; i++) {
    Clus[i] = clus[i];
    std::vector<DPNodePtr> sucs = Clus[i]->GetDep()->GetSucs();
    sucs.push_back(Clus[i]);
    if (Clus[i]->GetDelta() > minDelta) {
      SESAME::ClusterPtr c = std::make_shared<Cluster>(cluLabel++);;
      c->add(Clus[i]);
      clusters.push_back(c);
    } else {
      Clus[i]->GetDep()->GetCluster()->add(Clus[i]);
    }

  }
  double maxDelta = 0;
  for (int j = 1; j < size && j < i; j++) {
    // System.out.println(j);
    // System.out.println(size);
    // System.out.println(Clus[j]);
    if (maxDelta < Clus[j]->GetDelta()) {
      maxDelta = Clus[j]->GetDelta();
    }
  }
  Clus[0]->SetDelta(maxDelta);
  this->size = i;
  for (; i < size; i++) {
    outs->insert(clus[i]);
  }

}
