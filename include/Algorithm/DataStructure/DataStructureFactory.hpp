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
#include <Algorithm/DataStructure/FeatureVector.hpp>
#include <Algorithm/DataStructure/WeightedAdjacencyList.hpp>
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
  static MicroClusterPtr createMicroCluster(int dimension, int id,PointPtr dataPoint,double radius);
  static void clearMicroCluster(MicroClusterPtr microCluster);
  static SnapshotPtr createSnapshot(MicroClusters & otherMicroClusters,int elapsedTime);
  static void clearSnapshot(SnapshotPtr snapshot);
  static CFTreePtr createCFTree();
  static NodePtr createNode();
  static MicroClusterPairPtr createMicroClusterPair(MicroClusterPtr microCluster1,MicroClusterPtr microCluster2);
  static void clearMicroClusterPair(MicroClusterPairPtr microClusterPair);
  static AdjustedWeightPtr createAdjustedWeight(double weight, clock_t pointTime);
  static void clearAdjustedWeight(AdjustedWeightPtr adjustedWeight);
};
}
#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_DATASTRUCTUREFACTORY_HPP_
