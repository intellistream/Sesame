// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

//
// Created by Shuhao Zhang on 19/07/2021.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CORESETTREE_HPP_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CORESETTREE_HPP_
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <Algorithm/DataStructure/Point.hpp>
#include <Algorithm/DataStructure/TreeNode.hpp>
#include <vector>

namespace SESAME {
class CoresetTree;
typedef std::shared_ptr<CoresetTree> CoresetTreePtr;

class CoresetTree {

 public:

  /**
 * initalizes root as a treenode with the union of setA and setB as pointset and centre as centre
 * @param root
 * @param setA
 * @param setB
 * @param n_1
 * @param n_2
 * @param centre
 * @param centreIndex
 */
  void constructRoot(TreeNodePtr root,
                     std::vector<PointPtr> &setA,
                     std::vector<PointPtr> &setB,
                     int n_1,
                     int n_2,
                     PointPtr centre,
                     int centreIndex);

/**
Constructs a coreset of size k from the union of setA and setB
**/
  void unionTreeCoreset(int k,
                        int n_1,
                        int n_2,
                        std::vector<PointPtr> &setA,
                        std::vector<PointPtr> &setB,
                        std::vector<PointPtr> &centres);
  void freeTree(TreeNodePtr root);
  bool treeFinished(TreeNodePtr root);
  bool isLeaf(TreeNodePtr node);

  /**
   * Computes the target function value of the n points of the treenode. Differs from the function "targetFunctionValue" in three things:
   * 1. only the centre of the treenode is used as a centre
   * 2. works on arrays of pointers instead on arrays of points
   * 3. stores the cost in the treenode
   * @param node
   */
  void treeNodeTargetFunctionValue(TreeNodePtr node);

  /**
  selects a leaf node (using the kMeans++ distribution)
  **/
  TreeNodePtr selectNode(TreeNodePtr root);
  PointPtr chooseCentre(TreeNodePtr node);
  double treeNodeCostOfPoint(TreeNodePtr node, PointPtr p);
  double treeNodeSplitCost(TreeNodePtr node, PointPtr CenterA, PointPtr CenterB);
  void split(TreeNodePtr parent, PointPtr newCentre, int newCentreIndex);
  PointPtr determineClosestCentre(PointPtr point, PointPtr centreA, PointPtr centreB);
};

}

#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CORESETTREE_HPP_
