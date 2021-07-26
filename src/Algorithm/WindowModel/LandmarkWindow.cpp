//
// Created by Shuhao Zhang on 20/07/2021.
//

#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <Utils/Logger.hpp>
#include <Algorithm/DataStructure/CoresetTree.hpp>

void SESAME::LandmarkWindow::initBucket(int dimension, int coresetSize) {
  int i;
  for (i = 0; i < this->bucketManager.numberOfBuckets; i++) {
    this->bucketManager.buckets.emplace_back();
    this->bucketManager.buckets[i].cursize = 0;
    int i;
    for (i = 0; i < coresetSize; i++) {
      this->bucketManager.buckets[i].points[i].Initialization(-1, 1, dimension, 0);
      this->bucketManager.buckets[i].spillover[i].Initialization(-1, 1, dimension, 0);
    }
  }
}

void SESAME::LandmarkWindow::insertPoint(Point p) {
  //check if there is enough space in the first bucket
  int cursize = this->bucketManager.buckets[0].cursize;
  if (cursize >= this->bucketManager.maxBucketsize) {
    SESAME_INFO("Bucket 0 full \n");
    //start spillover process
    int curbucket = 0;
    int nextbucket = 1;

    //check if the next bucket is empty
    if (this->bucketManager.buckets[nextbucket].cursize == 0) {
      //copy the bucket
      int i;
      for (i = 0; i < this->bucketManager.maxBucketsize; i++) {
        this->bucketManager.buckets[nextbucket].points[i].copyFromPoint(this->bucketManager.buckets[curbucket].points[i]);

      }
      //bucket is now full
      this->bucketManager.buckets[nextbucket].cursize = this->bucketManager.maxBucketsize;
      //first bucket is now empty
      this->bucketManager.buckets[curbucket].cursize = 0;
      cursize = 0;
    } else {
      SESAME_INFO("Bucket " << nextbucket << "full \n");
      //copy bucket to spillover and continue
      int i;
      for (i = 0; i < this->bucketManager.maxBucketsize; i++) {
        this->bucketManager.buckets[nextbucket].spillover[i].copyFromPoint(this->bucketManager.buckets[curbucket].points[i]);

      }
      this->bucketManager.buckets[0].cursize = 0;
      cursize = 0;
      curbucket++;
      nextbucket++;
      /*
      as long as the next bucket is full output the coreset to the spillover of the next bucket
      */
      while (this->bucketManager.buckets[nextbucket].cursize == this->bucketManager.maxBucketsize) {
        SESAME_INFO("Bucket " << nextbucket << "full \n");
        SESAME::CoresetTree::unionTreeCoreset(this->bucketManager.maxBucketsize,
                                              this->bucketManager.maxBucketsize,
                                              this->bucketManager.maxBucketsize,
                                              this->bucketManager.buckets[curbucket].points,
                                              this->bucketManager.buckets[curbucket].spillover,
                                              this->bucketManager.buckets[nextbucket].spillover);
        // here we store the m constructed coreset points into spillover of the next bucket
        // bucket now empty
        this->bucketManager.buckets[curbucket].cursize = 0;
        curbucket++;
        nextbucket++;
      }
      SESAME::CoresetTree::unionTreeCoreset(this->bucketManager.maxBucketsize,
                                            this->bucketManager.maxBucketsize,
                                            this->bucketManager.maxBucketsize,
                                            this->bucketManager.buckets[curbucket].points,
                                            this->bucketManager.buckets[curbucket].spillover,
                                            this->bucketManager.buckets[nextbucket].points);
      this->bucketManager.buckets[curbucket].cursize = 0;
      this->bucketManager.buckets[nextbucket].cursize = this->bucketManager.maxBucketsize;
    }

  }
  //insert point into the first bucket
  this->bucketManager.buckets[0].points[cursize].copyFromPoint(p);
  this->bucketManager.buckets[0].cursize++;
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
Point *SESAME::LandmarkWindow::getCoresetFromManager() {
  Point *coreset;
  int i = 0;
  if (this->bucketManager.buckets[this->bucketManager.numberOfBuckets - 1].cursize
      == this->bucketManager.maxBucketsize) {
    coreset = this->bucketManager.buckets[this->bucketManager.numberOfBuckets - 1].points;

  } else {
    //find the first nonempty bucket
    for (i = 0; i < this->bucketManager.numberOfBuckets; i++) {
      if (this->bucketManager.buckets[i].cursize != 0) {
        coreset = this->bucketManager.buckets[i].points;
        break;
      }
    }
    //as long as there is a nonempty bucket compute a coreset
    int j;
    for (j = i + 1; j < this->bucketManager.numberOfBuckets; j++) {
      if (this->bucketManager.buckets[j].cursize != 0) {
        //output the coreset into the spillover of bucket j
        SESAME::CoresetTree::unionTreeCoreset(this->bucketManager.maxBucketsize,
                                              this->bucketManager.maxBucketsize,
                                              this->bucketManager.maxBucketsize,
                                              this->bucketManager.buckets[j].points,
                                              coreset,
                                              this->bucketManager.buckets[j].spillover);
        coreset = this->bucketManager.buckets[j].spillover;
      }
    }
  }
  return coreset;
}

