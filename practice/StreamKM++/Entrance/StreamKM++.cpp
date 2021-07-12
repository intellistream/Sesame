//
// Created by tuidan on 2021/6/30.
//

/********
Copyright 2008 by Marcel R. Ackermann, Christiane Lammersen, Marcus Maertens, Christoph Raupach,
Christian Sohler, Kamil Swierkot

Without any warranty!

To compile:

Use the Makefile by typing 'make' in a terminal or
type:
 g++ DatasetFunctions.c kMeansUtilityFunctions.c LloydPlusPlus.c Main.c mt19937ar.c Point.c StreamingCoreset.c TreeCoreset.c -O2 -o stream


To execute:

./stream filename n k d c outputfile seed

filename  must contain one point per line in format:

	1 x1 x2 ... xd

where x1 to xd are the coordinates and each point will be treated as a point in real space with the L2 Norm.

n is the number of points in the data set.

k is the number of clusters to compute.

d is the dimensionality of the points;

c specifies the size of the coreset.

outputfile contains the calculated centroids of the k clusters in format:

	w x1 x2 ... xd
where w is the weight if the centroid.

seed must be an integer and it will be used to seed the random number generator.
**/


/**
 *
 *
 * Changelog:
 *  2008/04/25
 * - added changelog
 * - changed input format and output format to Liadan format
 *
 * */




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../DataStructure/Point.hpp"
#include "../Utility/mt19937ar.hpp"
#include "../OfflineClustering/kMeansUtilityFunctions.hpp"
#include "../Utility/DatasetFunctions.hpp"
#include "../OfflineClustering/LloydPlusPlus.hpp"
#include "../WindowModel/LandmarkWindow/StreamingCoreset.hpp"


#define TRUE 1
#define FALSE 0

int main(int argc, char * argv[]) {

    FILE *in;
    struct point *points;
    int length;
    int dimension;
    int numberOfCentres;
    int coresetsize;
    char * filename;
    char * outputfile;
    int seed;
    if(argc != 8){
        printf("streaming.out filename n k d coresetsize outputfile seed\n");
        return 0;
    } else {
        filename = argv[1];
        length = atoi(argv[2]);
        numberOfCentres = atoi(argv[3]);
        dimension = atoi(argv[4]);
        coresetsize = atoi(argv[5]);
        outputfile = argv[6];
        seed = atoi(argv[7]);
    }

    //initalize random generator with current time
    init_genrand(seed);
    printf("Seed: %d \n",seed);

    in = fopen(filename,"r");


    //compute a coreset with the streaming algorithm
    struct Bucketmanager * manager = (struct Bucketmanager *)malloc(sizeof(struct Bucketmanager));
    initManager(manager,length,dimension,coresetsize);

    //insert the points one by one
    int i;
    for(i=0;i<length;i++){
        struct point *p = (struct point *)malloc(sizeof(struct point));
        initPoint(p,dimension);
        /*
        getNextPointGereon(in,p,dimension);
         - changed to Liadan format
        */
        getNextPointLiadan(in,p,dimension);
        p->id = i;



        insertPoint(p,manager);
        freePoint(p);
        free(p);
    }
    fclose(in);

    struct point *streamingCoreset = getCoresetFromManager(manager,dimension);



    //compute 5 clusterings of the coreset with kMeans++ and take the best
    double minCost = 0.0;
    double curCost = 0.0;
    struct point * centresStreamingCoreset = lloydPlusPlus(numberOfCentres,coresetsize,dimension,streamingCoreset,&minCost);
    curCost = minCost;

    for(i=1;i<5;i++){
        struct point * tmpCentresStreamingCoreset = lloydPlusPlus(numberOfCentres,coresetsize,dimension,streamingCoreset,&curCost);
        if(curCost < minCost) {
            minCost = curCost;
            centresStreamingCoreset = tmpCentresStreamingCoreset;
        }
    }

    //output the centres
    FILE *out = fopen(outputfile,"w");
    /*for(i=0;i<numberOfCentres;i++){
        int l;
        for(l=0;l<dimension-1;l++){
            fprintf(out,"%f,",centresStreamingCoreset[i].coordinates[l] / centresStreamingCoreset[i].weight);
        }
        fprintf(out,"%f",centresStreamingCoreset[i].coordinates[dimension-1] / centresStreamingCoreset[i].weight);
        fprintf(out,"\n");
    }
    -- Changed to Liadan format --
    */
    saveCentersLiadanFormat( out ,numberOfCentres, centresStreamingCoreset,dimension);

    fclose(out);




    return 0;
}
