/********
Copyright 2008 by Marcel R. Ackermann, Christiane Lammersen, Marcus Maertens, Christoph Raupach, 
Christian Sohler, Kamil Swierkot

Without any warranty!

*/
#include "StreamingCoreset.hpp"

/**
initializes a bucket
**/
void initBucket(struct Bucket * bucket,int d, int maxsize){
	bucket->cursize = 0;
	bucket->points = (struct point *)malloc(maxsize * sizeof(struct point));
	bucket->spillover = (struct point *)malloc(maxsize * sizeof(struct point));
	int i;
	for(i=0;i<maxsize;i++){
		initPoint(&(bucket->points[i]),d);
		initPoint(&(bucket->spillover[i]),d);
	}
}

/**
initializes a bucketmanager for n points with bucketsize maxsize and dimension d
**/
void initManager(struct Bucketmanager * manager,int n,int d,int maxsize){
	manager->numberOfBuckets = ceil(log((double)n/(double)maxsize) / log(2) )+2;
	manager->maxBucketsize = maxsize;
	manager->buckets = (struct Bucket *)malloc(manager->numberOfBuckets * sizeof(struct Bucket));
	int i;
	for(i=0;i<manager->numberOfBuckets;i++){
		initBucket(&(manager->buckets[i]),d,maxsize);
	}
	printf("Created manager with %d buckets of dimension %d \n",manager->numberOfBuckets,d);
}

/**
inserts a single point into the bucketmanager
**/
void insertPoint(struct point *p,struct Bucketmanager *manager){
	
	//check if there is enough space in the first bucket
	int cursize = manager->buckets[0].cursize;	
	if(cursize >= manager->maxBucketsize) {
		printf("Bucket 0 full \n");
		//start spillover process
		int curbucket  = 0;
		int nextbucket = 1;

		//check if the next bucket is empty
		if(manager->buckets[nextbucket].cursize == 0){
			//copy the bucket
			int i;
			for(i=0;i<manager->maxBucketsize;i++){
				copyPointWithoutInit( &(manager->buckets[curbucket].points[i]),
					&(manager->buckets[nextbucket].points[i]));

			}
			//bucket is now full
			 manager->buckets[nextbucket].cursize = manager->maxBucketsize;
			//first bucket is now empty
			manager->buckets[curbucket].cursize = 0;
			cursize = 0;
		} else {
			printf("Bucket %d full \n",nextbucket);
			//copy bucket to spillover and continue
			int i;
			for(i=0;i<manager->maxBucketsize;i++){

				copyPointWithoutInit( &(manager->buckets[curbucket].points[i]),
					&(manager->buckets[nextbucket].spillover[i]));

			}
			manager->buckets[0].cursize=0;
			cursize = 0;
			curbucket++;
			nextbucket++;
			/*
			as long as the next bucket is full output the coreset to the spillover of the next bucket
			*/
			while(manager->buckets[nextbucket].cursize == manager->maxBucketsize){
				printf("Bucket %d full \n",nextbucket);
				unionTreeCoreset(manager->maxBucketsize,manager->maxBucketsize,
					manager->maxBucketsize,p->dimension, 
					manager->buckets[curbucket].points,manager->buckets[curbucket].spillover,
					manager->buckets[nextbucket].spillover);
				// here we store the m constructed coreset points into spillover of the next bucket
				// bucket now empty
				manager->buckets[curbucket].cursize = 0;
				curbucket++;
				nextbucket++;
			}
			unionTreeCoreset(manager->maxBucketsize,manager->maxBucketsize,
					manager->maxBucketsize,p->dimension, 
					manager->buckets[curbucket].points,manager->buckets[curbucket].spillover,
					manager->buckets[nextbucket].points);
			manager->buckets[curbucket].cursize = 0;
			manager->buckets[nextbucket].cursize = manager->maxBucketsize;
		}
		
	}
	//insert point into the first bucket
	copyPointWithoutInit(p,&(manager->buckets[0].points[cursize]));
	manager->buckets[0].cursize++;
}

/**
It may happen that the manager is not full (since n is not always a power of 2). In this case we extract the coreset
from the manager by computing a coreset of all nonempty buckets

Case 1: the last bucket is full
=> n is a power of 2 and we return the contents of the last bucket

Case2: the last bucket is not full
=> we compute a coreset of all nonempty buckets 

this operation should only be called after the streaming process is finished
**/
struct point * getCoresetFromManager(struct Bucketmanager *manager,int d){
	struct point *coreset;
	int i = 0;
	if(manager->buckets[manager->numberOfBuckets-1].cursize == manager->maxBucketsize){
		coreset = manager->buckets[manager->numberOfBuckets-1].points;

	} else {
		//find the first nonempty bucket
		for(i=0;i<manager->numberOfBuckets;i++){
			if(manager->buckets[i].cursize != 0){
				coreset = manager->buckets[i].points;
				break;
			}
		}		
		//as long as there is a nonempty bucket compute a coreset
		int j;
		for(j=i+1;j<manager->numberOfBuckets;j++){
			if(manager->buckets[j].cursize != 0){
				//output the coreset into the spillover of bucket j
				unionTreeCoreset(manager->maxBucketsize,manager->maxBucketsize,
					manager->maxBucketsize,d, 
					manager->buckets[j].points,coreset,
					manager->buckets[j].spillover);
				coreset = manager->buckets[j].spillover;			
			}
		}
	}
	return coreset;
}
