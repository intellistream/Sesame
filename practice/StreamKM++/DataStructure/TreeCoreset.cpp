/********
Copyright 2008 by Marcel R. Ackermann, Christiane Lammersen, Marcus Maertens, Christoph Raupach, 
Christian Sohler, Kamil Swierkot

Without any warranty!

*/
#include "TreeCoreset.hpp"


/**
computes the hypothetical cost if the node would be split with new centers centreA, centreB
**/
double treeNodeSplitCost(struct treeNode * node, struct point * centreA, struct point * centreB){
	//loop counter variable
	int i;
	
	//stores the cost
	double sum = 0.0;
	
	for(i=0;i<node->n;i++){
		//loop counter variable
		int l;	

		//stores the distance between p and centreA
		double distanceA = 0.0;

		for(l=0;l<node->points[i]->dimension;l++){
			//centroid coordinate of the point
			double centroidCoordinatePoint;
			if(node->points[i]->weight != 0.0){
				centroidCoordinatePoint = node->points[i]->coordinates[l] / node->points[i]->weight;
			} else {
				centroidCoordinatePoint = node->points[i]->coordinates[l];
			}
			//centroid coordinate of the centre
			double centroidCoordinateCentre;
			if(centreA->weight != 0.0){
				centroidCoordinateCentre = centreA->coordinates[l] / centreA->weight;
			} else {
				centroidCoordinateCentre = centreA->coordinates[l];
			}

			distanceA += (centroidCoordinatePoint-centroidCoordinateCentre) * 
					(centroidCoordinatePoint-centroidCoordinateCentre) ;
		}

		//stores the distance between p and centreB
		double distanceB = 0.0;

		for(l=0;l<node->points[i]->dimension;l++){
			//centroid coordinate of the point
			double centroidCoordinatePoint;
			if(node->points[i]->weight != 0.0){
				centroidCoordinatePoint = node->points[i]->coordinates[l] / node->points[i]->weight;
			} else {
				centroidCoordinatePoint = node->points[i]->coordinates[l];
			}
			//centroid coordinate of the centre
			double centroidCoordinateCentre;
			if(centreB->weight != 0.0){
				centroidCoordinateCentre = centreB->coordinates[l] / centreB->weight;
			} else {
				centroidCoordinateCentre = centreB->coordinates[l];
			}

			distanceB += (centroidCoordinatePoint-centroidCoordinateCentre) * 
					(centroidCoordinatePoint-centroidCoordinateCentre) ;
		}

		//add the cost of the closest centre to the sum
		if(distanceA < distanceB){
			sum += distanceA*node->points[i]->weight;
		} else {
			sum += distanceB*node->points[i]->weight;
		}

	}
	
	//return the total cost
	return sum;

}


/**
computes the cost of point p with the centre of treenode node
**/
double treeNodeCostOfPoint(struct treeNode * node, struct point * p){
	if(p->weight == 0.0){
		return 0.0;
	}

	//stores the distance between centre and p
	double distance = 0.0;
	
	//loop counter variable
	int l;

	for(l=0;l<p->dimension;l++){
		//centroid coordinate of the point
		double centroidCoordinatePoint;
		if(p->weight != 0.0){
			centroidCoordinatePoint = p->coordinates[l] / p->weight;
		} else {
			centroidCoordinatePoint = p->coordinates[l];
		}
		//centroid coordinate of the centre
		double centroidCoordinateCentre;
		if(node->centre->weight != 0.0){
			centroidCoordinateCentre = node->centre->coordinates[l] / node->centre->weight;
		} else {
			centroidCoordinateCentre = node->centre->coordinates[l];
		}
		distance += (centroidCoordinatePoint-centroidCoordinateCentre) * 
				(centroidCoordinatePoint-centroidCoordinateCentre) ;
				
	}
	return distance * p->weight;
}


/**
Computes the target function value of the n points of the treenode. Differs from the function "targetFunctionValue" in three things:

1. only the centre of the treenode is used as a centre

2. works on arrays of pointers instead on arrays of points 

3. stores the cost in the treenode
**/
void treeNodeTargetFunctionValue(struct treeNode * node){
	//loop counter variable
	int i;
	
	//stores the cost
	double sum = 0.0;

	for(i=0;i<node->n;i++){
		//stores the distance
		double distance = 0.0;

		//loop counter variable
		int l;

		for(l=0;l<node->points[i]->dimension;l++){
			//centroid coordinate of the point
			double centroidCoordinatePoint;
			if(node->points[i]->weight != 0.0){
				centroidCoordinatePoint = node->points[i]->coordinates[l] / node->points[i]->weight;
			} else {
				centroidCoordinatePoint = node->points[i]->coordinates[l];
			}
			//centroid coordinate of the centre
			double centroidCoordinateCentre;
			if(node->centre->weight != 0.0){
				centroidCoordinateCentre = node->centre->coordinates[l] / node->centre->weight;
			} else {
				centroidCoordinateCentre = node->centre->coordinates[l];
			}
			distance += (centroidCoordinatePoint-centroidCoordinateCentre) * 
					(centroidCoordinatePoint-centroidCoordinateCentre) ;
				
		}

		sum += distance*node->points[i]->weight;	
	}
	
	
	node->cost = sum;
}

/**
initalizes root as a treenode with the union of setA and setB as pointset and centre as centre
**/
void constructRoot(struct treeNode * root, struct point * setA, struct point * setB,int n_1,int n_2, struct point * centre, int centreIndex){
	//loop counter variable
	int i;	

	//the root has no parent and no child nodes in the beginning
	root->parent 	= NULL;
	root->lc     	= NULL;
	root->rc     	= NULL; 

	//array with points to the points
	root->points = (struct point **)malloc(sizeof(struct point *)*(n_1+n_2));
	root->n = n_1 + n_2;
	
	for(i=0;i<root->n;i++){
		if(i < n_1){
			root->points[i] = &setA[i];
			root->points[i]->centreIndex = centreIndex;
		} else {
			root->points[i] = &setB[i-n_1];
			root->points[i]->centreIndex = centreIndex;
		}
	}

	//set the centre
	root->centre = centre;

	//calculate costs
	treeNodeTargetFunctionValue(root);
}

/**
tests if a node is a leaf
**/
int isLeaf(struct treeNode * node){

	if(node->lc == NULL && node->rc == NULL){
		return TRUE;
	} else {
		return FALSE;
	}

}

/**
selects a leaf node (using the kMeans++ distribution)
**/
struct treeNode * selectNode(struct treeNode * root){
	
	//random number between 0 and 1
	double random = genrand_real3();
	
	while(!isLeaf(root)){
		if(root->lc->cost == 0 && root->rc->cost == 0){
			if(root->lc->n == 0){
				root = root->rc;
			} else if(root->rc->n == 0){
				root = root->lc;
			}else if(random < 0.5){
				random = genrand_real3();
				root = root->lc;
			} else {		
				random = genrand_real3();		
				root = root->rc;
			}
		} else {

			if(random < root->lc->cost/root->cost){
		
				root = root->lc;
			} else {		

				root = root->rc;
			}
		}
	}

	return root;
}

/**
selects a new centre from the treenode (using the kMeans++ distribution)
**/
struct point * chooseCentre(struct treeNode * node){

	//How many times should we try to choose a centre ??
	int times = 3;
		
	//stores the nodecost if node is split with the best centre
	double minCost = node->cost;
	struct point * bestCentre = NULL;
	
	//loop counter variable
	int i;
	int j;
	
	for(j=0;j<times;j++){
		//sum of the relativ cost of the points
		double sum = 0.0;
		//random number between 0 and 1
		double random = genrand_real3();
		
		for(i=0;i<node->n;i++){
		
			sum += treeNodeCostOfPoint(node,node->points[i]) / node->cost;
			if(sum >= random){
				if(node->points[i]->weight == 0.0){
					printf("ERROR: CHOOSEN DUMMY NODE THOUGH OTHER AVAILABLE \n");
					return NULL;
				}
				double curCost = treeNodeSplitCost(node,node->centre,node->points[i]);
				if(curCost < minCost){
					bestCentre = node->points[i];
					minCost = curCost;
				}
				break;
			}
		}
	}
	if(bestCentre == NULL){
		return node->points[0];
	} else {
		return bestCentre;
	}
}

/**
returns the next centre
**/
struct point * determineClosestCentre(struct point * p, struct point * centreA, struct point * centreB){
	
	//loop counter variable
	int l;	

	//stores the distance between p and centreA
	double distanceA = 0.0;

	for(l=0;l<p->dimension;l++){
		//centroid coordinate of the point
		double centroidCoordinatePoint;
		if(p->weight != 0.0){
			centroidCoordinatePoint = p->coordinates[l] / p->weight;
		} else {
			centroidCoordinatePoint = p->coordinates[l];
		}
		//centroid coordinate of the centre
		double centroidCoordinateCentre;
		if(centreA->weight != 0.0){
			centroidCoordinateCentre = centreA->coordinates[l] / centreA->weight;
		} else {
			centroidCoordinateCentre = centreA->coordinates[l];
		}

		distanceA += (centroidCoordinatePoint-centroidCoordinateCentre) * 
				(centroidCoordinatePoint-centroidCoordinateCentre) ;
	}

	//stores the distance between p and centreB
	double distanceB = 0.0;

	for(l=0;l<p->dimension;l++){
		//centroid coordinate of the point
		double centroidCoordinatePoint;
		if(p->weight != 0.0){
			centroidCoordinatePoint = p->coordinates[l] / p->weight;
		} else {
			centroidCoordinatePoint = p->coordinates[l];
		}
		//centroid coordinate of the centre
		double centroidCoordinateCentre;
		if(centreB->weight != 0.0){
			centroidCoordinateCentre = centreB->coordinates[l] / centreB->weight;
		} else {
			centroidCoordinateCentre = centreB->coordinates[l];
		}

		distanceB += (centroidCoordinatePoint-centroidCoordinateCentre) * 
				(centroidCoordinatePoint-centroidCoordinateCentre) ;
	}

	//return the nearest centre
	if(distanceA < distanceB){
		return centreA;
	} else {
		return centreB;
	}
}

/**
splits the parent node and creates two child nodes (one with the old centre and one with the new one)
**/
void split(struct treeNode * parent,struct point * newCentre,int newCentreIndex){
	
	//loop counter variable
	int i;


	//1. Counts how many points belong to the new and how many points belong to the old centre
	int nOld = 0;
	int nNew = 0;
	for(i=0;i<parent->n;i++){
		struct point * centre = determineClosestCentre(parent->points[i],parent->centre,newCentre);
		if(centre == newCentre){
			nNew++;
		} else {
			nOld++;
		} 
	}

	//2. initalizes the arrays for the pointer
	
	//array for pointer on the points belonging to the old centre
	struct point ** oldPoints = (struct point **)malloc(nOld * sizeof(struct point *));

	//array for pointer on the points belonging to the new centre
	struct point ** newPoints = (struct point **)malloc(nNew * sizeof(struct point *));

	int indexOld = 0;
	int indexNew = 0;

	for(i=0;i<parent->n;i++){
		struct point * centre = determineClosestCentre(parent->points[i],parent->centre,newCentre);
		if(centre == newCentre){
			newPoints[indexNew] = parent->points[i];
			newPoints[indexNew]->centreIndex = newCentreIndex;
			indexNew++;
		} else if(centre == parent->centre){
			oldPoints[indexOld] = parent->points[i];
			indexOld++;
		} else {
			printf("ERROR !!! NO CENTER NEAREST !! \n");
		}
	}

	//left child: old centre
	struct treeNode * lc = (struct treeNode *)malloc(sizeof(struct treeNode));
	lc->centre = parent->centre;
	lc->points = oldPoints;
	lc->n = nOld;

	lc->lc = NULL;
	lc->rc = NULL;
	lc->parent = parent;

	treeNodeTargetFunctionValue(lc);
	
	//right child: new centre
	struct treeNode * rc = (struct treeNode *)malloc(sizeof(struct treeNode));
	rc->centre = newCentre;
	rc->points = newPoints;
	rc->n = nNew;

	rc->lc = NULL;
	rc->rc = NULL;
	rc->parent = parent;

	treeNodeTargetFunctionValue(rc);

	//set childs of the parent node
	parent->lc = lc;
	parent->rc = rc;

	//propagate the cost changes to the parent nodes
	while(parent != NULL){
		parent->cost = parent->lc->cost + parent->rc->cost;
		parent = parent->parent;
	}

}

/**
Checks if the storage is completly freed
**/
int treeFinished(struct treeNode * root){
	if(root->parent == NULL && root->lc == NULL && root->rc == NULL){
		return TRUE;
	} else {
		return FALSE;
	}

}

/**
frees a tree of its storage
**/
void freeTree(struct treeNode * root){

	while(!treeFinished(root)){
		if(root->lc == NULL && root->rc == NULL){
			root = root->parent;
		} else if(root->lc == NULL && root->rc != NULL){
			//Schau ob rc ein Blatt ist
			if(isLeaf(root->rc)){
				//Gebe rechtes Kind frei
				free(root->rc->points);
				free(root->rc);
				root->rc = NULL;
			} else {
				//Fahre mit rechtem Kind fort
				root = root->rc;
			}
		} else if(root->lc != NULL) {
			if(isLeaf(root->lc)){
				free(root->lc->points);
				free(root->lc);
				root->lc = NULL;
			} else {
				root = root->lc;
			}
		}
	}
	free(root->points);
	free(root);

}

/**
Constructs a coreset of size k from the union of setA and setB
**/
void unionTreeCoreset(int k,int n_1,int n_2,int d, struct point * setA,struct point * setB,struct point * centres) {
	printf("Computing coreset...\n");
	//total number of points
	int n = n_1+n_2;

	//choose the first centre (each point has the same probability of being choosen)
	
	//stores, how many centres have been choosen yet
	int choosenPoints = 0; 
	
	//only choose from the n-i points not already choosen
	int j = genrand_int31()%(n-choosenPoints);

	//copy the choosen point
	if(j < n_1){
		copyPointWithoutInit(&setA[j],&centres[choosenPoints]);
	} else {
		j = j - n_1;
		copyPointWithoutInit(&setB[j],&centres[choosenPoints]);
	}
	struct treeNode * root = (struct treeNode *)malloc(sizeof(struct treeNode));
	constructRoot(root,setA,setB,n_1,n_2,&centres[choosenPoints],choosenPoints);
	choosenPoints = 1;
	
	//choose the remaining points
	while(choosenPoints < k){
		if(root->cost > 0.0){
			struct treeNode * leaf = selectNode(root);
			struct point * centre = chooseCentre(leaf);
			split(leaf,centre,choosenPoints);
			copyPointWithoutInit(centre,&centres[choosenPoints]);
		} else {
			//create a dummy point
			copyPointWithoutInit(root->centre,&centres[choosenPoints]);
			int l;
			for(l=0;l<root->centre->dimension;l++){
				centres[choosenPoints].coordinates[l] = -1 * 1000000;
			}
			centres[choosenPoints].id = -1;
			centres[choosenPoints].weight = 0.0;
			centres[choosenPoints].squareSum = 0.0;
			
		}
		
		choosenPoints++;
	}

	//free the tree
	freeTree(root);

	//recalculate clustering features
	int i;
	for(i=0;i<n;i++){
			
		if(i < n_1) {
			
			int index = setA[i].centreIndex;
			if(centres[index].id != setA[i].id){
				centres[index].weight += setA[i].weight;
				centres[index].squareSum += setA[i].squareSum;
				int l;
				for(l=0;l<centres[index].dimension;l++){
					if(setA[i].weight != 0.0){
						centres[index].coordinates[l] += setA[i].coordinates[l];
					}
				}
			}
		} else {
			
			int index = setB[i-n_1].centreIndex;
			if(centres[index].id != setB[i-n_1].id){
				centres[index].weight += setB[i-n_1].weight;
				centres[index].squareSum += setB[i-n_1].squareSum;
				int l;
				for(l=0;l<centres[index].dimension;l++){
					if(setB[i-n_1].weight != 0.0){
						centres[index].coordinates[l] += setB[i-n_1].coordinates[l];
					}
				}
			}
		}
	}
}
