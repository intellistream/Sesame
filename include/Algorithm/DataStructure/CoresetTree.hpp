//
// Created by Shuhao Zhang on 19/07/2021.
//

#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CORESETTREE_HPP_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CORESETTREE_HPP_
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <Algorithm/DataStructure/Point.hpp>

namespace SESAME {

class CoresetTree {
/**
DataStructure representing a node within a tree
**/

  struct treeNode {
    //number of points in this node
    int n;

    //array with pointers on points
    Point *points;

    //pointer on the centre of the treenode
    Point centre;

    //pointer on the left childnode
    struct treeNode *lc;

    //pointer on the right childnode
    struct treeNode *rc;

    //pointer on the parent node
    struct treeNode *parent;

    //cost of the treenode
    double cost;
  };
 public:

/**
Constructs a coreset of size k from the union of setA and setB
**/
  static void unionTreeCoreset(int k, int n_1, int n_2, Point *setA, Point *setB, Point *centres);
  static void freeTree(treeNode *root);
  static bool treeFinished(treeNode *root);
  static bool isLeaf(treeNode *node);
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
  static void constructRoot(treeNode *root, Point *setA, Point *setB, int n_1, int n_2, Point centre, int centreIndex);
  /**
   * Computes the target function value of the n points of the treenode. Differs from the function "targetFunctionValue" in three things:
   * 1. only the centre of the treenode is used as a centre
   * 2. works on arrays of pointers instead on arrays of points
   * 3. stores the cost in the treenode
   * @param node
   */
  static void treeNodeTargetFunctionValue(treeNode *node);

  /**
  selects a leaf node (using the kMeans++ distribution)
  **/
  static treeNode *selectNode(treeNode *root);
  static Point chooseCentre(treeNode *node);
  static double treeNodeCostOfPoint(treeNode *node, Point p);
  static double treeNodeSplitCost(treeNode *node, Point CenterA, Point CenterB);
  static void split(treeNode *parent, Point newCentre, int newCentreIndex);
  static Point determineClosestCentre(Point p, Point centreA, Point centreB);
};

}

#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_CORESETTREE_HPP_
