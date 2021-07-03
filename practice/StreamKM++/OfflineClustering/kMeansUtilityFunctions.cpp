/********
Copyright 2008 by Marcel R. Ackermann, Christiane Lammersen, Marcus Maertens, Christoph Raupach, 
Christian Sohler, Kamil Swierkot

Without any warranty!
*/
#include "kMeansUtilityFunctions.hpp"

/**
debug function to print a set of points
**/
void printPoints(int n,struct point * points){
	int i;
	for(i=0;i<n;i++){
		printf("ID %d,Dimension %d, Weight %f \n",points[i].id,points[i].dimension,points[i].weight);
	}
}


/**
computes the target function for the given pointarray points[] (of size n) with the given array of
centres centres[] (of size k)
**/
double targetFunctionValue(int k,int n,struct point centres[],struct point points[]){
	int i=0;
	double sum = 0.0;
	for(i=0;i<n;i++){
		double nearestCost = -1.0;
		int j=0;
		for(j=0;j<k;j++){
			double distance = 0.0;
			int l = 0;
			for(l=0;l<points[i].dimension;l++){
				//Centroid coordinate of the point
				double centroidCoordinatePoint;
				if(points[i].weight != 0.0){
					centroidCoordinatePoint = points[i].coordinates[l] / points[i].weight;
				} else {
					centroidCoordinatePoint = points[i].coordinates[l];
				}
				//Centroid coordinate of the centre
				double centroidCoordinateCentre;
				if(centres[j].weight != 0.0){
					centroidCoordinateCentre = centres[j].coordinates[l] / centres[j].weight;
				} else {
					centroidCoordinateCentre = centres[j].coordinates[l];
				}
				distance += (centroidCoordinatePoint-centroidCoordinateCentre) * 
						(centroidCoordinatePoint-centroidCoordinateCentre) ;
				
			}
			if(nearestCost <0 || distance < nearestCost) {
				nearestCost = distance;
			} 
		}
		sum += nearestCost*points[i].weight;
	}
	return sum;
}

/**
computes the target function for the union of the given pointarrays setA[] (of size n_1) and setB[] (of size n_2) with the given array of centres centres[] (of size k)
**/
double unionTargetFunctionValue(int k,int n_1,int n_2,struct point centres[],struct point setA[], struct point setB[]){
	int n = n_1+n_2;
	int i=0;
	double sum = 0.0;
	for(i=0;i<n;i++){
		if(i<n_1) {
			double nearestCost = -1.0;
			int j=0;
			for(j=0;j<k;j++){
				double distance = 0.0;
				int l = 0;
				for(l=0;l<setA[i].dimension;l++){
					//Centroid coordinate of the point
					double centroidCoordinatePoint;
					if(setA[i].weight != 0.0){
						centroidCoordinatePoint = setA[i].coordinates[l] / setA[i].weight;
					} else {
						centroidCoordinatePoint = setA[i].coordinates[l];
					}
					//Centroid coordinate of the centre
					double centroidCoordinateCentre;
					if(centres[j].weight != 0.0){
						centroidCoordinateCentre = centres[j].coordinates[l] / centres[j].weight;
					} else {
						centroidCoordinateCentre = centres[j].coordinates[l];
					}
					distance += (centroidCoordinatePoint-centroidCoordinateCentre) * 
							(centroidCoordinatePoint-centroidCoordinateCentre) ;
				}
				
				if(nearestCost <0 || distance < nearestCost) {
					nearestCost = distance;
				} 
			}
			sum += nearestCost * setA[i].weight;
		} else {
			double nearestCost = -1.0;
			int j=0;
			for(j=0;j<k;j++){
				double distance = 0.0;
				int l = 0;
				for(l=0;l<setB[i-n_1].dimension;l++){
					//Centroid coordinate of the point
					double centroidCoordinatePoint;
					if(setB[i-n_1].weight != 0.0){
						centroidCoordinatePoint = setB[i-n_1].coordinates[l] / setB[i-n_1].weight;
					} else {
						centroidCoordinatePoint = setB[i-n_1].coordinates[l];
					}
					//Centroid coordinate of the centre
					double centroidCoordinateCentre;
					if(centres[j].weight != 0.0){
						centroidCoordinateCentre = centres[j].coordinates[l] / centres[j].weight;
					} else {
						centroidCoordinateCentre = centres[j].coordinates[l];
					}
					distance += (centroidCoordinatePoint-centroidCoordinateCentre) * 
							(centroidCoordinatePoint-centroidCoordinateCentre) ;
				}
				
				if(nearestCost <0 || distance < nearestCost) {
					nearestCost = distance;
				} 
			}
			sum += nearestCost * setB[i-n_1].weight;

		}
	}
	return sum;


}

/**
Computes the cost of point p with the given array of centres centres[] (of size k)
**/
double costOfPoint(int k,struct point centres[],struct point p){
	double nearestCost = -1.0;
	int j=0;
	for(j=0;j<k;j++){
		double distance = 0.0;
		int l = 0;
		for(l=0;l<p.dimension;l++){
			//Centroid coordinate of the point
			double centroidCoordinatePoint;
			if(p.weight != 0.0){
				centroidCoordinatePoint = p.coordinates[l] / p.weight;
			} else {
				centroidCoordinatePoint = p.coordinates[l];
			}
			//Centroid coordinate of the centre
			double centroidCoordinateCentre;
			if(centres[j].weight != 0.0){
				centroidCoordinateCentre = centres[j].coordinates[l] / centres[j].weight;
			} else {
				centroidCoordinateCentre = centres[j].coordinates[l];
			}
			distance += (centroidCoordinatePoint-centroidCoordinateCentre) * 
					(centroidCoordinatePoint-centroidCoordinateCentre) ;
		}
		
		if(nearestCost <0 || distance < nearestCost) {
			nearestCost = distance;
		} 
	}
	return p.weight*nearestCost;
}


/**
Computes the index of the centre nearest to p with the given array of centres centres[] (of size k)
**/
int determineClusterCentreKMeans(int k,struct point p,struct point centres[]){
	int centre = 0;
	double nearestCost = -1.0;
	int j=0;
	for(j=0;j<k;j++){
		double distance = 0.0;
		int l = 0;
		for(l=0;l<p.dimension;l++){
			//Centroid coordinate of the point
			double centroidCoordinatePoint;
			if(p.weight != 0.0){
				centroidCoordinatePoint = p.coordinates[l] / p.weight;
			} else {
				centroidCoordinatePoint = p.coordinates[l];
			}
			//Centroid coordinate of the centre
			double centroidCoordinateCentre;
			if(centres[j].weight != 0.0){
				centroidCoordinateCentre = centres[j].coordinates[l] / centres[j].weight;
			} else {
				centroidCoordinateCentre = centres[j].coordinates[l];
			}
			distance += (centroidCoordinatePoint-centroidCoordinateCentre) * 
					(centroidCoordinatePoint-centroidCoordinateCentre) ;
		}
		
		if(nearestCost <0 || distance < nearestCost) {
			nearestCost = distance;
			centre = j;
		} 
	}
	return centre;	
}


/**
Computes the cost of point p with centre centre
**/
double costOfPointToCenter(struct point * p,struct point * centre){
	if(p->weight == 0.0){
		return 0.0;
	}

	//stores the distance between p and centre
	double distance = 0.0;

	//loop counter
	int l;

	for(l=0;l<p->dimension;l++){
		//Centroid coordinate of the point
		double centroidCoordinatePoint;
		if(p->weight != 0.0){
			centroidCoordinatePoint = p->coordinates[l] / p->weight;
		} else {
			centroidCoordinatePoint = p->coordinates[l];
		}
		//Centroid coordinate of the centre
		double centroidCoordinateCentre;
		if(centre->weight != 0.0){
			centroidCoordinateCentre = centre->coordinates[l] / centre->weight;
		} else {
			centroidCoordinateCentre = centre->coordinates[l];
		}
		distance += (centroidCoordinatePoint-centroidCoordinateCentre) * 
				(centroidCoordinatePoint-centroidCoordinateCentre) ;
				
	}
	return distance * p->weight;
}

