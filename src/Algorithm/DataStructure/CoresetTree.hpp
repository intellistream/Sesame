//
// Created by tuidan on 2021/7/10.
//

#ifndef ONLINEMLBENCHMARK_CORESETTREE_HPP_
#define ONLINEMLBENCHMARK_CORESETTREE_HPP_

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "../../Benchmark/DataLoader/Point.hpp"
#include "../../Utility/RandomGenerator.hpp"
#include "../OfflineClustering/KMeans.hpp"
#include "../../Utility/Macro.hpp"

/**
DataStructure representing a node within a tree
**/

struct treeNode {
    //number of points in this node
    int n;

    //array with pointers on points
    Point * points;

    //pointer on the centre of the treenode
    Point centre;

    //pointer on the left childnode
    struct treeNode * lc;

    //pointer on the right childnode
    struct treeNode * rc;

    //pointer on the parent node
    struct treeNode * parent;

    //cost of the treenode
    double cost;
};


/**
Constructs a coreset of size k from the union of setA and setB
**/
void unionTreeCoreset(int k,int n_1,int n_2,int d,  Point * setA, Point * setB, Point * centres, int dimensionx);


#endif //ONLINEMLBENCHMARK_CORESETTREE_H
