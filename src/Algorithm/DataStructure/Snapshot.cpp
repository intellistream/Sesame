//
// Created by 1124a on 2021/8/8.
//

#include <Algorithm/DataStructure/Snapshot.hpp>
SESAME::MicroClustersPtr SESAME::Snapshot::findSnapshot(QueueOrderSnapshotPtr orderSnapShots,
                                                        int landmarkTime,int currentElapsedTime ,unsigned int currentOrder)
{
  unsigned int i=0;
  MicroClustersPtr nearestSnapshot;
  int minDistance=currentElapsedTime;
  int dist;
  int tempMinDistance=0;

  while (i<=currentOrder)
  {
    for(int a=0;a<orderSnapShots[i]->size();a++)
    {
      int elapsedTimeSnapshot=orderSnapShots[i]->Element(a).elapsedTime;
      dist=abs((int)(elapsedTimeSnapshot-landmarkTime));

      if(minDistance>dist || (minDistance==dist&&tempMinDistance<elapsedTimeSnapshot))
      {
        minDistance=dist;
        tempMinDistance=elapsedTimeSnapshot;
        nearestSnapshot=orderSnapShots[i]->Element(a).microClusters;
      }
    }
    i++;
  }
  return nearestSnapshot;

}
SESAME::MicroClustersPtr SESAME::Snapshot::substractSnapshot(MicroClustersPtr snapshotCurrent,
                                                             MicroClustersPtr snapshotLandmark,unsigned int clusterNumber)
{


  MicroClustersPtr substractedSnapshot;
  for(unsigned int i=0;i<clusterNumber;i++)
  {
    //If the micro cluster raised from merging
    if(snapshotCurrent[i]->id.size()>1)
    {
      for(unsigned int j=0;j<clusterNumber;j++)
      {
        if(snapshotLandmark[j]->id.size()>1)
        {
          bool ifMerge=snapshotCurrent[i]->judgeMerge(snapshotLandmark[j]);
          if(ifMerge)
            substractedSnapshot[i]=snapshotCurrent[i]->substractClusterVector(snapshotLandmark[j]);
        }

        else
        {
          int clusterIdLandmark=snapshotLandmark[j]->id[0];
          auto iter=std::find(snapshotCurrent[i]->id.begin(),
                              snapshotCurrent[i]->id.end(),clusterIdLandmark);
          if(iter==snapshotCurrent[i]->id.end())
            substractedSnapshot[i]=snapshotCurrent[i]->substractClusterVector(snapshotLandmark[j]);
        }
      }
    }
      // The micro cluster raised from creating new ones
    else
    {
      for(unsigned int j=0;j<clusterNumber;j++)
      {
        int clusterIdLandmark=snapshotLandmark[j]->id[0];
        auto iter=std::find(snapshotCurrent[i]->id.begin(),
                            snapshotCurrent[i]->id.end(),clusterIdLandmark);
        if(iter==snapshotCurrent[i]->id.end())
        {
          substractedSnapshot[i]=snapshotCurrent[i]->substractClusterVector(snapshotLandmark[j]);
        }
      }
    }
  }
  return substractedSnapshot;

}

