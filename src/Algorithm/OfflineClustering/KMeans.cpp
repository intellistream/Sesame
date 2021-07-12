//
// Created by tuidan on 2021/7/10.
//
#include "KMeans.hpp"


/**
Randomly chooses k centres with kMeans++ distribution
**/
Point chooseRandomCentres(int k,int n,int d, Point * points, int dimension){

    //array to store the choosen centres
    Point * centres =new Point[k];

    //choose the first centre (each point has the same probability of being choosen)
    int i = 0;


    int next = 0;
    int j = 0;
    do{//only choose from the n-i points not already choosen
        next = genrand_int31()%(n-1);

        //check if the choosen point is not a dummy
    }while( points[next].getWeight() < 1);

    //set j to next unchoosen point
    j= next;
    //copy the choosen point to the array
    copyPoint(points[j], centres[i]);

    //set the current centre for all points to the choosen centre
    for(i=0;i<n;i++){
        points[i].setClusteringCenter(0);
        points[i].setCost(costOfPointToCenter(points[i], centres[0]));

    }
    //choose centre 1 to k-1 with the kMeans++ distribution
    for(i=1;i<k;i++){

        double cost = 0.0;
        for(j=0;j<n;j++){
            cost += points[j].getCost();
        }

        double random = 0;
        double sum = 0.0;
        int pos = -1;

        do{

            random = genrand_real3();
            sum = 0.0;
            pos = -1;

            for(j=0;j<n;j++){
                sum = sum + points[j].getCost();
                if(random <= sum/cost){
                    pos = j;
                    break;
                }
            }

        }while(points[pos].getWeight() < 1);
        //copy the choosen centre
        copyPoint(points[pos],centres[i]);
        //check which points are closest to the new centre
        for(j=0;j<n;j++){
            double newCost = costOfPointToCenter(points[j],centres[i]);
            if(points[j].getCost() > newCost){
                points[j].getCost() = newCost;
                points[j].getClusteringCenter() = i;
            }
        }

    }

    printf("random centres: \n");
    for(i=0;i<k;i++){
        printf("%d: (",i);
        int l = 0;
        for(l=0;l<dimension;l++){
            printf("%f,",centres[i].getFeatureItem(l) / centres[i].getWeight());
        }
        printf(")\n");
    }

    return centres;
}

/**
kMeans++ algorithm for n points of dimension d with k centres
**/
struct point * lloydPlusPlus(int k,int n,int d, Point * points,double *resultCost, int dimension){
    printf("starting kMeans++\n");
    //choose random centres
    Point *centres = chooseRandomCentres(k,n,d,points[0]);
    double cost = targetFunctionValue(k,n,centres[0],points[0]);
    double newCost = cost;


    Point * massCentres = new Point[k];
    double numberOfPoints[k];

    do{
        cost = newCost;
        //reset centres of mass
        int i = 0;
        for(i=0;i<k;i++){

            initPoint(massCentres[i],d);
            numberOfPoints[i] = 0.0;
        }
        //compute centres of mass
        for(i=0;i<n;i++){
            int centre = determineClusterCentreKMeans(k,points[i],centres[0]);
            int l = 0;
            for(l=0;l<dimension;l++){
                if(points[i].weight != 0.0)
                    massCentres[centre].getFeatureItem(l) += points[i].getFeatureItem(l);
            }
            numberOfPoints[centre] += points[i].getWeight();

        }

        //move centres
        for(i=0;i<k;i++){
            int l = 0;
            for(l=0;l<dimension;l++){
                centres[i].getFeatureItem(l) = massCentres[i].getFeatureItem(l);
                centres[i].getWeight() = numberOfPoints[i];
            }
        }

        //calculate costs
        newCost = targetFunctionValue(k,n,centres[0],points[0]);
        printf("old cost:%f, new cost:%f \n",cost,newCost);
    }while(newCost < THRESHOLD * cost);

    printf("Centres: \n");
    int i=0;
    for(i=0;i<k;i++){
        printf("(");
        int l = 0;
        for(l=0;l<dimension;l++){
            printf("%f,",centres[i].getFeatureItem(l) / centres[i].getWeight());
        }
        printf(")\n");
    }
    *resultCost = newCost;
    printf("kMeans++ finished\n");
    return centres;
}

/**
computes the target function for the given pointarray points[] (of size n) with the given array of
centres centres[] (of size k)
**/
double targetFunctionValue(int k,int n, Point * centres, Point * points, int dimension){
    int i=0;
    double sum = 0.0;
    for(i=0;i<n;i++){
        double nearestCost = -1.0;
        int j=0;
        for(j=0;j<k;j++){
            double distance = 0.0;
            int l = 0;
            for(l=0;l<dimension;l++){
                //Centroid coordinate of the point
                double centroidCoordinatePoint;
                if(points[i].getWeight() != 0.0){
                    centroidCoordinatePoint = points[i].getFeatureItem(l) / points[i].getWeight();
                } else {
                    centroidCoordinatePoint = points[i].getFeatureItem(l);
                }
                //Centroid coordinate of the centre
                double centroidCoordinateCentre;
                if(centres[j].getWeight() != 0.0){
                    centroidCoordinateCentre = centres[j].getFeatureItem(l) / centres[j].getWeight();
                } else {
                    centroidCoordinateCentre = centres[j].getFeatureItem(l);
                }
                distance += (centroidCoordinatePoint-centroidCoordinateCentre) *
                            (centroidCoordinatePoint-centroidCoordinateCentre) ;

            }
            if(nearestCost <0 || distance < nearestCost) {
                nearestCost = distance;
            }
        }
        sum += nearestCost*points[i].getWeight();
    }
    return sum;
}


/**
Computes the index of the centre nearest to p with the given array of centres centres[] (of size k)
**/
int determineClusterCentreKMeans(int k, Point p, Point * centre, int dimension){
    int centre = 0;
    double nearestCost = -1.0;
    int j=0;
    for(j=0;j<k;j++){
        double distance = 0.0;
        int l = 0;
        for(l=0;l<dimension;l++){
            //Centroid coordinate of the point
            double centroidCoordinatePoint;
            if(p.get != 0.0){
                centroidCoordinatePoint = p.getFeatureItem(l) / p.getWeight();
            } else {
                centroidCoordinatePoint = p.getFeatureItem(l);
            }
            //Centroid coordinate of the centre
            double centroidCoordinateCentre;
            if(centre[j].getFeatureItem(l) != 0.0){
                centroidCoordinateCentre = centres[j].coordinates[l] / centre[j].getWeight();
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
double costOfPointToCenter(Point p, Point centre, int dimension){
    if(p.getWeight() == 0.0){
        return 0.0;
    }

    //stores the distance between p and centre
    double distance = 0.0;

    //loop counter
    int l;

    for(l=0;l<dimension;l++){
        //Centroid coordinate of the point
        double centroidCoordinatePoint;
        if(p->weight != 0.0){
            centroidCoordinatePoint = p.getFeatureItem(l) / p.getWeight();
        } else {
            centroidCoordinatePoint = p.getFeatureItem(l);
        }
        //Centroid coordinate of the centre
        double centroidCoordinateCentre;
        if(centre.getWeight() != 0.0){
            centroidCoordinateCentre = centre.getFeatureItem(l) / centre.getWeight();
        } else {
            centroidCoordinateCentre = centre.getFeatureItem(l);
        }
        distance += (centroidCoordinatePoint-centroidCoordinateCentre) *
                    (centroidCoordinatePoint-centroidCoordinateCentre) ;

    }
    return distance * p.getWeight();
}

