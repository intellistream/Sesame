#ifndef POINT_HPP
#define POINT_HPP
#include "Point.hpp"

/**
initializes a point
**/
void initPoint(struct point *point,int dimension){
	point->weight = 1.0;
	point->squareSum = 0.0;
	point->dimension = dimension;
	point->coordinates = (float *)malloc(dimension*sizeof(float));
	point->id = -1;
	point->cl = -1;
	point->curCost = 0;
	point->centreIndex = -1;
	int l = 0;
	for(l=0;l<dimension;l++){
		point->coordinates[l] = 0.0;
	}
}

/**
deletes a point
**/
void freePoint(struct point *point){
	free(point->coordinates);
}

/**
copys the data of the first point to the address of the second point
**/
void copyPoint(struct point * firstPoint,struct  point * secondPoint){
	initPoint(secondPoint,firstPoint->dimension);
	secondPoint->id = firstPoint->id;
	secondPoint->weight = firstPoint->weight;
	secondPoint->cl = firstPoint->cl;
	secondPoint->squareSum = firstPoint->squareSum;
	secondPoint->curCost = firstPoint->curCost;
	secondPoint->centreIndex = firstPoint->centreIndex;
	int l;
	for(l=0;l<firstPoint->dimension;l++){
		secondPoint->coordinates[l] = firstPoint->coordinates[l];
	}
}

/**
copys the data of the first point to the address of the second point without initializing the second
point first (this means, that the storage for the coordinates of the second point has to be already reserved)
**/
void copyPointWithoutInit(struct point * firstPoint,struct  point * secondPoint){
	secondPoint->id = firstPoint->id;
	secondPoint->weight = firstPoint->weight;
	secondPoint->cl = firstPoint->cl;
	secondPoint->squareSum = firstPoint->squareSum;
	secondPoint->curCost = firstPoint->curCost;
	secondPoint->centreIndex = firstPoint->centreIndex;
	int l;
	for(l=0;l<firstPoint->dimension;l++){
		secondPoint->coordinates[l] = firstPoint->coordinates[l];
	}
}
