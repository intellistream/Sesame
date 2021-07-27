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
