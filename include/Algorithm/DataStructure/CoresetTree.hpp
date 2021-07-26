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

class CoresetTree {

 public:

/**
Constructs a coreset of size k from the union of setA and setB
**/
  static void unionTreeCoreset(int k,
                               int n_1,
                               int n_2,
                               std::vector<Point> setA,
                               std::vector<Point> setB,
                               std::vector<Point> centres);
  static void freeTree(TreeNode *root);
  static bool treeFinished(TreeNode *root);
  static bool isLeaf(TreeNode *node);
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
  static void constructRoot(TreeNode &root,
                            std::vector<Point> setA,
                            std::vector<Point> setB,
                            int n_1,
                            int n_2,
                            Point centre,
                            int centreIndex);
  /**
   * Computes the target function value of the n points of the treenode. Differs from the function "targetFunctionValue" in three things:
   * 1. only the centre of the treenode is used as a centre
   * 2. works on arrays of pointers instead on arrays of points
   * 3. stores the cost in the treenode
   * @param node
   */
  static void treeNodeTargetFunctionValue(TreeNode &node);

  /**
  selects a leaf node (using the kMeans++ distribution)
  **/
  static TreeNode *selectNode(TreeNode &root);
  static Point chooseCentre(TreeNode &node);
  static double treeNodeCostOfPoint(TreeNode &node, Point p);
  static double treeNodeSplitCost(TreeNode *node, Point CenterA, Point CenterB);
  static void split(TreeNode *parent, Point newCentre, int newCentreIndex);
  static Point determineClosestCentre(Point p, Point centreA, Point centreB);
};

}

#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CORESETTREE_HPP_
