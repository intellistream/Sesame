//
// Created by 1124a on 2021/8/8.
//


#ifndef SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_SNAPSHOT_HPP_
#define SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_SNAPSHOT_HPP_
#include <algorithm>
#include <cmath>
#include <cassert>
#include <limits>
#include <ctime>
#include <cstdio>
#include<Algorithm/DataStructure/MicroCluster.hpp>
#include<Algorithm/DataStructure/ArrayQueue.hpp>
#include <iostream>

namespace SESAME {
class Snapshot;
typedef std::shared_ptr<Snapshot> SnapshotPtr;

typedef std::vector <MicroCluster>  MicroClusters;
typedef std::vector<SnapshotPtr> QueueSnapshotPtr;
typedef std::vector<QueueSnapshotPtr> QueueOrderSnapshotPtr;

class Snapshot{
 public:
  int elapsedTime;
  MicroClusters  microClusters;

  /**
  QueueSnapshotPtr: Data Structure representing order ith snapshots list
  QueueOrderSnapshotPtr: Data Structure representing orders
  **/

  static SnapshotPtr findSnapshot(QueueOrderSnapshotPtr orderSnapShots,
                                       int landmarkTime ,int currentElapsedTime ,unsigned int currentOrder);

  static SnapshotPtr substractSnapshot(SnapshotPtr snapshotCurrent,
                                       SnapshotPtr snapshotLandmark,unsigned int clusterNumber);

};
}
#endif //SESAME_INCLUDE_ALGORITHM_DATASTRUCTURE_SNAPSHOT_HPP_