// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 19/07/2021.
//

#include <Algorithm/DataStructure/DataStructureFactory.hpp>

SESAME::TreeNodePtr SESAME::DataStructureFactory::createTreeNode() {
  return std::make_shared<TreeNode>();
}
void SESAME::DataStructureFactory::clearTreeNode(SESAME::TreeNodePtr treeNode) {
  treeNode.reset();
}
SESAME::PointPtr SESAME::DataStructureFactory::createPoint() {
  return std::make_shared<Point>();
}

SESAME::PointPtr SESAME::DataStructureFactory::createPoint(int index, double weight, int dimension, double cost) {
  return std::make_shared<Point>(index, weight, dimension, cost);
}

void SESAME::DataStructureFactory::clearPoint(SESAME::PointPtr point) {
  point.reset();
}
SESAME::CoresetTreePtr SESAME::DataStructureFactory::createCoresetTree() {
  return std::make_shared<CoresetTree>();
}
void SESAME::DataStructureFactory::clearCoresetTree(SESAME::CoresetTreePtr tree) {
  tree.reset();
}

SESAME::MicroClusterPtr SESAME::DataStructureFactory::createMicroCluster(int id, int dimension){
  return std::make_shared<SESAME::MicroCluster>( id, dimension);
}

void SESAME::DataStructureFactory::clearMicroCluster(SESAME::MicroClusterPtr microCluster){
  microCluster.reset();
}

SESAME::SnapshotPtr SESAME::DataStructureFactory::createSnapshot(SESAME::MicroClusters & otherMicroClusters,int elapsedTime){
  return std::make_shared<SESAME::Snapshot>(otherMicroClusters,elapsedTime);
}

void SESAME::DataStructureFactory::clearSnapshot(SESAME::SnapshotPtr Snapshot){
  Snapshot.reset();
}
SESAME::CFTreePtr SESAME::DataStructureFactory::createCFTree() {
  return std::make_shared<SESAME::CFTree>(0,0,0);
}
SESAME::NodePtr SESAME::DataStructureFactory::createNode() {
  return std::make_shared<SESAME::CFNode>();
}
SESAME::DPNodePtr SESAME::DataStructureFactory::createDPNode() {
  return std::make_shared<SESAME::DPNode>();
}
SESAME::DPNodePtr SESAME::DataStructureFactory::createDPNode(SESAME::PointPtr p, double time) {
  return std::make_shared<SESAME::DPNode>(p, time);
}
SESAME::OutPtr SESAME::DataStructureFactory::createOutlierReservoir() {
  return std::make_shared<SESAME::OutlierReservoir>();
}
SESAME::CachePtr SESAME::DataStructureFactory::creatCache() {
  return std::make_shared<SESAME::Cache>();
}
SESAME::OutPtr SESAME::DataStructureFactory::createOutlierReservoir(double r, double a, double lamd) {
  return std::make_shared<SESAME::OutlierReservoir>(r, a, lamd);
}
SESAME::CachePtr SESAME::DataStructureFactory::creatCache(int num, double a, double lamd, double r) {
  return std::make_shared<SESAME::Cache>(num, a, lamd, r);
}

