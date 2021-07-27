//
// Created by Shuhao Zhang on 19/07/2021.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_DATASTRUCTUREFACTORY_HPP_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_DATASTRUCTUREFACTORY_HPP_
#include <Algorithm/DataStructure/Point.hpp>
#include <Algorithm/DataStructure/TreeNode.hpp>
#include <Algorithm/DataStructure/CoresetTree.hpp>

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
};
}
#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_DATASTRUCTUREFACTORY_HPP_
