#ifndef STREAMINGCORESET_H
#define STREAMINGCORESET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math.h"

#include "../../DataStructure/Point.hpp"
#include "../../DataStructure/TreeCoreset.hpp"
#include "../../Utility/mt19937ar.hpp"
#include "../../OfflineClustering/kMeansUtilityFunctions.hpp"


#define TRUE 1
#define FALSE 0

/**
datastructure representing a single bucket
**/
struct Bucket {
	int cursize;
	struct point *points;
	struct point *spillover;
};

/**
datastructure for managing all O(log(n)) buckets
**/
struct Bucketmanager {
	int numberOfBuckets;
	int maxBucketsize;
	struct Bucket *buckets;
};

/**
initializes a bucketmanager for n points with bucketsize maxsize and dimension d
**/
void initManager(struct Bucketmanager * manager,int n,int d,int maxsize);

/**
inserts a single point into the bucketmanager
**/
void insertPoint(struct point * p,struct Bucketmanager * manager);

/**
It may happen that the manager is not full (since n is not always a power of 2). In this case we extract the coreset
from the manager by computing a coreset of all nonempty buckets

Case 1: the last bucket is full
=> n is a power of 2 and we return the contents of the last bucket

Case2: the last bucket is not full
=> we compute a coreset of all nonempty buckets 

this operation should only be called after the streaming process is finished
**/
struct point * getCoresetFromManager(struct Bucketmanager *manager,int d);


#endif
