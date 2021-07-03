/********
Copyright 2008 by Marcel R. Ackermann, Christiane Lammersen, Marcus Maertens, Christoph Raupach, 
Christian Sohler, Kamil Swierkot

Without any warranty!
*/
#include "LloydPlusPlus.hpp"


#define TRUE 1
#define FALSE 0

/**
Randomly chooses k centres with kMeans++ distribution
**/
struct point * chooseRandomCentres(int k,int n,int d,struct point points[]){

	//array to store the choosen centres
	struct point *centres =(struct point *)malloc(k*sizeof(struct point));

	//choose the first centre (each point has the same probability of being choosen)
	int i = 0;
	

	int next = 0;
	int j = 0;
	do{//only choose from the n-i points not already choosen
		next = genrand_int31()%(n-1);
		
		//check if the choosen point is not a dummy
	}while( points[next].weight < 1);
	
	//set j to next unchoosen point
	j= next;
	//copy the choosen point to the array
	copyPoint(&points[j], &centres[i]);
		
	//set the current centre for all points to the choosen centre
	for(i=0;i<n;i++){
		points[i].centreIndex = 0;
		points[i].curCost = costOfPointToCenter(&points[i],&centres[0]);
	
	}
	//choose centre 1 to k-1 with the kMeans++ distribution
	for(i=1;i<k;i++){

		double cost = 0.0;
		for(j=0;j<n;j++){
			cost += points[j].curCost;
		}
		
		double random = 0;
		double sum = 0.0;
		int pos = -1;
		
		do{

			random = genrand_real3();
			sum = 0.0;
			pos = -1;

		for(j=0;j<n;j++){
			sum = sum + points[j].curCost;
			if(random <= sum/cost){
				pos = j;
				break;
			}	
		}	
		
		}while(points[pos].weight < 1);	
		//copy the choosen centre
		copyPoint(&points[pos],&centres[i]);
		//check which points are closest to the new centre
		for(j=0;j<n;j++){
			double newCost = costOfPointToCenter(&points[j],&centres[i]);
			if(points[j].curCost > newCost){
				points[j].curCost = newCost;
				points[j].centreIndex = i;
			}
		}
		
	}
	
	printf("random centres: \n");
	for(i=0;i<k;i++){
		printf("%d: (",i);
		int l = 0;
		for(l=0;l<centres[i].dimension;l++){
			printf("%f,",centres[i].coordinates[l] / centres[i].weight);
		}
		printf(")\n");
	}

	return centres;
}

/**
kMeans++ algorithm for n points of dimension d with k centres
**/
struct point * lloydPlusPlus(int k,int n,int d,struct point points[],double *resultCost){
	printf("starting kMeans++\n");
	//choose random centres
	struct point *centres = chooseRandomCentres(k,n,d,&points[0]);
	double cost = targetFunctionValue(k,n,&centres[0],&points[0]);
	double newCost = cost;
	

	struct point massCentres[k];
	double numberOfPoints[k];

	do{
		cost = newCost;
		//reset centres of mass
		int i = 0;
		for(i=0;i<k;i++){
			
			initPoint(&massCentres[i],d);
			numberOfPoints[i] = 0.0;
		}
		//compute centres of mass
		for(i=0;i<n;i++){
			int centre = determineClusterCentreKMeans(k,points[i],&centres[0]);
			int l = 0;
			for(l=0;l<massCentres[centre].dimension;l++){
				if(points[i].weight != 0.0)
					massCentres[centre].coordinates[l] += points[i].coordinates[l];
			}
			numberOfPoints[centre] += points[i].weight;
		
		}
		
		//move centres
		for(i=0;i<k;i++){
			int l = 0;
			for(l=0;l<centres[i].dimension;l++){
				centres[i].coordinates[l] = massCentres[i].coordinates[l];
				centres[i].weight = numberOfPoints[i];
			}
		}
		

		//calculate costs
		newCost = targetFunctionValue(k,n,&centres[0],&points[0]);
		printf("old cost:%f, new cost:%f \n",cost,newCost);
	}while(newCost < THRESHOLD * cost);

	printf("Centres: \n");
	int i=0;
	for(i=0;i<k;i++){
		printf("(");
		int l = 0;
		for(l=0;l<centres[i].dimension;l++){
			printf("%f,",centres[i].coordinates[l] / centres[i].weight);
		}
		printf(")\n");
	}
	*resultCost = newCost;
	printf("kMeans++ finished\n");
	return centres;
}
