#ifndef POINT_H
#define POINT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
struct representing a single point
**/
struct point {

	//dimension
	int dimension;

	//Clustering Features (weight, squareSum, linearSum)
	double weight;
	double squareSum;
	float *coordinates;

	//cost and index of the centre, the point is currently assigned to
	double curCost;
	int centreIndex;
	

	//id and class (if there is class information in the file)
	int id;
	int cl;
	
};

/**
initializes a point
**/
void initPoint(struct point *point,int dimension);

/**
deletes a point
**/
void freePoint(struct point *point);

/**
copys the data of the first point to the address of the second point
**/
void copyPoint(struct point * firstPoint,struct  point * secondPoint);

/**
copys the data of the first point to the address of the second point without initializing the second
point first (this means, that the storage for the coordinates of the second point has to be already reserved)
**/
void copyPointWithoutInit(struct point * firstPoint,struct  point * secondPoint);

#endif
