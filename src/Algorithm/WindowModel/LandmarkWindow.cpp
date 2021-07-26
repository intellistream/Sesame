//
// Created by Shuhao Zhang on 20/07/2021.
//

#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <Utils/Logger.hpp>
#include <Algorithm/DataStructure/CoresetTree.hpp>

void SESAME::LandmarkWindow::initBucket(SESAME::LandmarkWindow::Bucket *bucket, int d, int maxsize) {
  bucket->cursize = 0;
  bucket->points = new Point[maxsize];
  bucket->spillover = new Point[maxsize];
  int i;
  for (i = 0; i < maxsize; i++) {
    bucket->points[i].Initialization(-1, 1, d, 0);
    bucket->spillover[i].Initialization(-1, 1, d, 0);
  }
}

void SESAME::LandmarkWindow::initManager(Bucketmanager *manager, int n, int d, int maxsize) {
  manager->numberOfBuckets = ceil(log((double) n / (double) maxsize) / log(2)) + 2;
  manager->maxBucketsize = maxsize;
  manager->buckets = (struct Bucket *) malloc(manager->numberOfBuckets * sizeof(struct Bucket));
  int i;
  for (i = 0; i < manager->numberOfBuckets; i++) {
    initBucket(&(manager->buckets[i]), d, maxsize);
  }
  SESAME_INFO("Created manager with" << manager->numberOfBuckets << "buckets of dimension" << d << "\n");
}

void SESAME::LandmarkWindow::insertPoint(Point p, SESAME::LandmarkWindow::Bucketmanager *manager) {
  //check if there is enough space in the first bucket
  int cursize = manager->buckets[0].cursize;
  if (cursize >= manager->maxBucketsize) {
    SESAME_INFO("Bucket 0 full \n");
    //start spillover process
    int curbucket = 0;
    int nextbucket = 1;

    //check if the next bucket is empty
    if (manager->buckets[nextbucket].cursize == 0) {
      //copy the bucket
      int i;
      for (i = 0; i < manager->maxBucketsize; i++) {
        manager->buckets[nextbucket].points[i].copyFromPoint(manager->buckets[curbucket].points[i]);

      }
      //bucket is now full
      manager->buckets[nextbucket].cursize = manager->maxBucketsize;
      //first bucket is now empty
      manager->buckets[curbucket].cursize = 0;
      cursize = 0;
    } else {
      SESAME_INFO("Bucket " << nextbucket << "full \n");
      //copy bucket to spillover and continue
      int i;
      for (i = 0; i < manager->maxBucketsize; i++) {
        manager->buckets[nextbucket].spillover[i].copyFromPoint(manager->buckets[curbucket].points[i]);

      }
      manager->buckets[0].cursize = 0;
      cursize = 0;
      curbucket++;
      nextbucket++;
      /*
      as long as the next bucket is full output the coreset to the spillover of the next bucket
      */
      while (manager->buckets[nextbucket].cursize == manager->maxBucketsize) {
        SESAME_INFO("Bucket " << nextbucket << "full \n");
        SESAME::CoresetTree::unionTreeCoreset(manager->maxBucketsize, manager->maxBucketsize,
                                              manager->maxBucketsize,
                                              manager->buckets[curbucket].points, manager->buckets[curbucket].spillover,
                                              manager->buckets[nextbucket].spillover);
        // here we store the m constructed coreset points into spillover of the next bucket
        // bucket now empty
        manager->buckets[curbucket].cursize = 0;
        curbucket++;
        nextbucket++;
      }
      SESAME::CoresetTree::unionTreeCoreset(manager->maxBucketsize, manager->maxBucketsize,
                                            manager->maxBucketsize,
                                            manager->buckets[curbucket].points, manager->buckets[curbucket].spillover,
                                            manager->buckets[nextbucket].points);
      manager->buckets[curbucket].cursize = 0;
      manager->buckets[nextbucket].cursize = manager->maxBucketsize;
    }

  }
  //insert point into the first bucket
  manager->buckets[0].points[cursize].copyFromPoint(p);
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
Point *SESAME::LandmarkWindow::getCoresetFromManager(SESAME::LandmarkWindow::Bucketmanager *manager) {
  Point *coreset;
  int i = 0;
  if (manager->buckets[manager->numberOfBuckets - 1].cursize == manager->maxBucketsize) {
    coreset = manager->buckets[manager->numberOfBuckets - 1].points;

  } else {
    //find the first nonempty bucket
    for (i = 0; i < manager->numberOfBuckets; i++) {
      if (manager->buckets[i].cursize != 0) {
        coreset = manager->buckets[i].points;
        break;
      }
    }
    //as long as there is a nonempty bucket compute a coreset
    int j;
    for (j = i + 1; j < manager->numberOfBuckets; j++) {
      if (manager->buckets[j].cursize != 0) {
        //output the coreset into the spillover of bucket j
        SESAME::CoresetTree::unionTreeCoreset(manager->maxBucketsize, manager->maxBucketsize,
                                              manager->maxBucketsize,
                                              manager->buckets[j].points, coreset,
                                              manager->buckets[j].spillover);
        coreset = manager->buckets[j].spillover;
      }
    }
  }
  return coreset;
}
