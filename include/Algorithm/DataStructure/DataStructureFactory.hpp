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
  static MicroClusterPtr createMicroCluster();
  static void clearMicroCluster(MicroClusterPtr microCluster);
  static SnapshotPtr createSnapshot();
  static void clearSnapshot(SnapshotPtr snapshot);
};
}
#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_DATASTRUCTUREFACTORY_HPP_
