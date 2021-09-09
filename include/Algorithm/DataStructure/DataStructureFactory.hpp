// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 19/07/2021.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_DATASTRUCTUREFACTORY_HPP_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_DATASTRUCTUREFACTORY_HPP_
#include <Algorithm/DataStructure/Point.hpp>
#include <Algorithm/DataStructure/TreeNode.hpp>
#include <Algorithm/DataStructure/CoresetTree.hpp>
#include <Algorithm/DataStructure/MicroCluster.hpp>
#include <Algorithm/DataStructure/Snapshot.hpp>
#include <Algorithm/DataStructure/CFTree.hpp>
#include <Algorithm/DataStructure/DPTree.hpp>
#include <Algorithm/DataStructure/DPNode.hpp>
#include <Algorithm/DataStructure/OutlierReservoir.hpp>
#include <Algorithm/DataStructure/Cache.hpp>
#include <Algorithm/DataStructure/FeatureVector.hpp>

namespace SESAME {
class DataStructureFactory {

 public:

  static PointPtr createPoint(int index, double weight, int dimension, double cost);
  static PointPtr createPoint();
  static void clearPoint(PointPtr point);

  static TreeNodePtr createTreeNode();
  static void clearTreeNode(TreeNodePtr treeNode);
  static CoresetTreePtr createCoresetTree();
  static void clearCoresetTree(CoresetTreePtr tree);
  static MicroClusterPtr createMicroCluster(int dimension, int id);
  static void clearMicroCluster(MicroClusterPtr microCluster);
  static SnapshotPtr createSnapshot(MicroClusters & otherMicroClusters,int elapsedTime);
  static void clearSnapshot(SnapshotPtr snapshot);
  static CFTreePtr createCFTree();
  static NodePtr createNode();
  static NodePtr createDPTree();
  static DPNodePtr createDPNode();
  static DPNodePtr createDPNode(SESAME::PointPtr p, double time);
  static CachePtr creatCache();
  static CachePtr creatCache(int num, double a, double lamd, double r);
  static OutPtr createOutlierReservoir();
  static OutPtr createOutlierReservoir(double r, double a, double lamd);

};
}
#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_DATASTRUCTUREFACTORY_HPP_
