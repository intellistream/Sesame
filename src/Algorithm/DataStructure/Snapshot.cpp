//
// Created by 1124a on 2021/8/8.
//

#include <Algorithm/DataStructure/Snapshot.hpp>
#include <Utils/Logger.hpp>
#include <iterator>
#include <Algorithm/DataStructure/DataStructureFactory.hpp>

SESAME::Snapshot SESAME::Snapshot::findSnapshot(QueueOrderSnapshot orderSnapShots,
                                                int landmarkTime, int currentElapsedTime , unsigned int currentOrder)
{
  unsigned int i=0;
  Snapshot nearestSnapshot;
  nearestSnapshot.elapsedTime=-1;

  int minDistance=currentElapsedTime;
  int dist;
  int tempMinDistance=0;
  int elapsedTimeSnapshot;
 // SESAME_INFO("currentOrder"<< currentOrder<<", Elapsed time: "<<currentElapsedTime<<"Landmark is"<<landmarkTime);
  while (i<=currentOrder)
  {
    for(int a=0;a<orderSnapShots[i].size();a++)
    {
       elapsedTimeSnapshot=orderSnapShots[i][a].elapsedTime;
       dist=abs((int)(elapsedTimeSnapshot-landmarkTime));

      if(minDistance>dist|| (minDistance==dist&&tempMinDistance<elapsedTimeSnapshot) )//
      { //SESAME_INFO(" this one ET is "<< elapsedTimeSnapshot<<",last mindistance is "<<tempMinDistance);
        minDistance=dist;
        tempMinDistance=elapsedTimeSnapshot;
        nearestSnapshot.microClusters=std::move(orderSnapShots[i][a].microClusters);
        nearestSnapshot.elapsedTime=orderSnapShots[i][a].elapsedTime;

      }
    }
    i++;
  }
  SESAME_INFO("close landmark "<< tempMinDistance);
  return nearestSnapshot;

}
SESAME::Snapshot SESAME::Snapshot::substractSnapshot(Snapshot &snapshotCurrent,
                                                        Snapshot &snapshotLandmark,unsigned int clusterNumber)
{
  SESAME_INFO("Start substract "<<snapshotCurrent.elapsedTime<<" LANDMARK ET "<<snapshotLandmark.elapsedTime);
  if(snapshotCurrent.elapsedTime!=snapshotLandmark.elapsedTime)
  {
      for(unsigned int i=0;i<clusterNumber;i++)
      {   //If the micro cluster raised from merging
          if(snapshotCurrent.microClusters[i].id.size()>1)
          {
           for(unsigned int j=0;j<clusterNumber;j++)
           {
              if(snapshotLandmark.microClusters[j].id.size()>1)
              {
                if(snapshotCurrent.microClusters[i].judgeMerge(snapshotLandmark.microClusters[j]))
                {
                  snapshotCurrent.microClusters[i].substractClusterVector(snapshotLandmark.microClusters[j]);
                }
              }
              else
              {
                int clusterIdLandmark;
                clusterIdLandmark = snapshotLandmark.microClusters[j].id[0];
                if(std::find(snapshotCurrent.microClusters[i].id.begin(),
                             snapshotCurrent.microClusters[i].id.end(),clusterIdLandmark)!=snapshotCurrent.microClusters[i].id.end())
                snapshotCurrent.microClusters[i].substractClusterVector(snapshotLandmark.microClusters[j]);
              }
           }
          }
          // The micro cluster raised from creating new ones or the original ones
          else
          {
            for(unsigned int j=0;j<clusterNumber;j++)
            {
              if(snapshotLandmark.microClusters[j].id.size()==1)
              {
              int clusterIdLandmark=snapshotLandmark.microClusters[j].id[0];
              if(snapshotCurrent.microClusters[i].id[0]==clusterIdLandmark)
                  snapshotCurrent.microClusters[i].substractClusterVector(snapshotLandmark.microClusters[j]);
              }
            }
          }
      }
  }
  return snapshotCurrent;

}

