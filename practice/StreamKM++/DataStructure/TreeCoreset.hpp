#ifndef TREECORESET_H
#define TREECORESET_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Point.hpp"
#include "../Utility/mt19937ar.hpp"
#include "../OfflineClustering/kMeansUtilityFunctions.hpp"

#define TRUE 1
#define FALSE 0


/**
datastructure representing a node within a tree
**/
struct treeNode {
	//number of points in this node
	int n;
	
	//array with pointers on points
	struct point ** points;

	//pointer on the centre of the treenode
	struct point * centre;

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
void unionTreeCoreset(int k,int n_1,int n_2,int d, struct point * setA,struct point * setB,struct point * centres);

#endif
