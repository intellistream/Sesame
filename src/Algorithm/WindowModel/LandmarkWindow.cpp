//
// Created by Shuhao Zhang on 20/07/2021.
//

#include <Algorithm/WindowModel/LandmarkWindow.hpp>
#include <Utils/Logger.hpp>

/**
 * TODO: seems not meaningful? @Wangxin
 * @Description: create a blank window, initial the point in the window.
 * @param: dimension
 * @param: windowSize
 */
void SESAME::LandmarkWindow::initWindow() {
  int i;
  for (i = 0; i < this->windowManager.numberOfWindow; i++) {
    Window blankWindow;
    blankWindow.cursize = 0;
    this->windowManager.windows.push_back(blankWindow);
  }
}

/**
 * TODO: extract the incremental computation progress from the whole function. @Wangxin
 * @Description: insert every data point into the time window. If both of the current original and spillover windows are
 * full, construct a coreset tree using all data and store the computed results tree nodes into the next original window
 * @Param: Point: the single point data
 * @Return: void
 */
void SESAME::LandmarkWindow::insertPoint(PointPtr point) {
  //check if there is enough space in the first window
  int cursize = this->windowManager.windows[0].cursize;
  if (cursize >= this->windowManager.maxWindowSize) {
    SESAME_INFO("Window 0 is full");
    //start spillover process
    int curWindow = 0;
    int nextWindow = 1;

    // check if the next window is empty
    if (this->windowManager.windows[nextWindow].cursize == 0) {
      SESAME_INFO("Window " << nextWindow << " is not full, move window " << curWindow << " points to window " << nextWindow);

      // if empty, copy the window
      int i;
      for (i = 0; i < this->windowManager.maxWindowSize; i++) {
        this->windowManager.windows[nextWindow].points.push_back(this->windowManager.windows[curWindow].points[i]->copy());
      }
      // window is now full
      this->windowManager.windows[nextWindow].cursize = this->windowManager.maxWindowSize;
      // first window is now set empty
      this->windowManager.windows[curWindow].cursize = 0;
      cursize = 0;
    } else {

      // if next window is full
      SESAME_INFO("Window " << nextWindow << " is full, move window " << curWindow << " to spillover " << nextWindow);
      //copy the points in the current window to the next spillover and continue
      int i;
      for (i = 0; i < this->windowManager.maxWindowSize; i++) {
        this->windowManager.windows[nextWindow].spillover.push_back(this->windowManager.windows[curWindow].points[i]->copy());
      }
      this->windowManager.windows[0].cursize = 0;
      cursize = 0;
      curWindow++;
      nextWindow++;

      // as long as the next window is full output the coreset to the next spillover, using points in
      // the next window and spillover
      while (this->windowManager.windows[nextWindow].cursize == this->windowManager.maxWindowSize) {
        SESAME_INFO("Window " << nextWindow << " is full, Continue! construct the coreset using points in window and spillover "<< curWindow << " and store it in the spillover " << nextWindow);
        this->tree->unionTreeCoreset(this->windowManager.maxWindowSize,
                                     this->windowManager.maxWindowSize,
                                     this->windowManager.maxWindowSize,
                                     this->windowManager.windows[curWindow].points,
                                     this->windowManager.windows[curWindow].spillover,
                                     this->windowManager.windows[nextWindow].spillover);
        // here we store the m constructed coreset points into the next spillover
        // current window now empty
        this->windowManager.windows[curWindow].cursize = 0;
        curWindow++;
        nextWindow++;
      }
      SESAME_INFO("Window " << nextWindow << " is not full, End! construct the coreset using points in window and spillover "<< curWindow << " and store the it in the last spillover");
      // if the next window is empty, just do the same operation and store the constructed coreset into
      // the next spillover, now the next window is empty but next spillover is full
      this->tree->unionTreeCoreset(this->windowManager.maxWindowSize,
                                   this->windowManager.maxWindowSize,
                                   this->windowManager.maxWindowSize,
                                   this->windowManager.windows[curWindow].points,
                                   this->windowManager.windows[curWindow].spillover,
                                   this->windowManager.windows[nextWindow].points);
      this->windowManager.windows[curWindow].cursize = 0;
      this->windowManager.windows[nextWindow].cursize = this->windowManager.maxWindowSize;
    }

  }
  // if the first window is not full, just insert point into it
  this->windowManager.windows[0].points.push_back(point->copy());//   .copy(point);
  this->windowManager.windows[0].cursize++;
}
/**
It may happen that the manager is not full (since n is not always a power of 2). In this case we extract the coreset
from the manager by computing a coreset of all nonempty windows

Case 1: the last bucket is full
=> n is a power of 2 and we return the contents of the last bucket

Case2: the last bucket is not full
=> we compute a coreset of all nonempty windows

this operation should only be called after the streaming process is finished
**/
std::vector<SESAME::PointPtr> SESAME::LandmarkWindow::getCoresetFromManager(std::vector<PointPtr> & coreset) {
  int i = 0;
  if (this->windowManager.windows[this->windowManager.numberOfWindow - 1].cursize
      == this->windowManager.maxWindowSize) {
    coreset = this->windowManager.windows[this->windowManager.numberOfWindow - 1].points;

  } else if(this->windowManager.windows[this->windowManager.numberOfWindow - 1].cursize == 0){
    coreset = this->windowManager.windows[this->windowManager.numberOfWindow - 2].points;
  } else {
    //find the first nonempty bucket

    for (i = 0; i < this->windowManager.numberOfWindow; i++) {
      if (this->windowManager.windows[i].cursize != 0) {
        coreset = this->windowManager.windows[i].points;
        break;
      }
    }
    //as long as there is a nonempty bucket compute a coreset
    int j;
    for (j = i + 1; j < this->windowManager.numberOfWindow; j++) {
      if (this->windowManager.windows[j].cursize != 0) {
        //output the coreset into the spillover of bucket j
        this->tree->unionTreeCoreset(this->windowManager.maxWindowSize,
                                     this->windowManager.maxWindowSize,
                                     this->windowManager.maxWindowSize,
                                     this->windowManager.windows[j].points,
                                     coreset,
                                     this->windowManager.windows[j].spillover);
        coreset = this->windowManager.windows[j].spillover;
      }
    }
  }
  return coreset;
}




/**
 * @Description: init the pyramidal window, pass the value of startTime and
 * reset shared_ptr Arrayqueue of snapshots
 * timeInterval: the time interval of online pyramidal window frame
 * @Return: void
 */
void SESAME::LandmarkWindow::initPyramidalWindow(unsigned int timeInterval)
{
  for(int i=0;i<200;i++)
  {
    orderSnapShots.push_back(std::make_shared<ArrayQueue<Snapshot>>(timeInterval+1));
  }
  this->pyramidalWindow.currentOrder=0;
}

/**
    * @Description: this function is pyramidal window for Clustream,
    * which takes and stores the snapshots of micro clusters
    * @Param: microClusters: micro clusters' snapshot need to be stored
    * startTime: start time of algorithms
    * @Return: void
    */
//TODO Still need to debug
void SESAME::LandmarkWindow::pyramidalWindowProcess(clock_t startTime,SESAME::MicroClustersPtr microClusters){
  int i=-1;
 // unsigned int i=0;
  clock_t now= clock();
  int elapsedTime=(int)((now,startTime)/CLOCKS_PER_SEC);
  if(elapsedTime>0)
  this->pyramidalWindow.currentOrder= (int)(log(elapsedTime)/log(this->pyramidalWindow.timeInterval));
  //NOTE: snapshot when elapsed time =0 always add to the front of latest T order
  while(++i>=0)//++i>=0
  {
    if(this->pyramidalWindow.currentOrder>=i && elapsedTime%(int)(pow(this->pyramidalWindow.timeInterval,i))==0)
    {
      if(elapsedTime%(int)(pow(this->pyramidalWindow.timeInterval,i+1))!=0)
      {
        std::cout<<i<<std::endl;
        storeSnapshot(i,microClusters,elapsedTime);
      }


    }
    else
      break;
    if(i==10)
      break;
  }
}
/**
      * @Description: this function stores snapshots into the pyramidal window data structure
      * @Param: currentOrder: the ith order  snapshots stored into
      * microClusters: micro clusters' snapshot need to be stored
      * elapsedTime: the current elapsed time
      * @Return: void
      */

void SESAME::LandmarkWindow::storeSnapshot(unsigned  int currentOrder,MicroClustersPtr microClusters, int elapsedTime)
{
  unsigned int size=orderSnapShots[currentOrder]->size();
  Snapshot snapshot;
  snapshot.elapsedTime=elapsedTime;
  snapshot.microClusters=std::move(microClusters);
  if(size==this->pyramidalWindow.timeInterval+1)
  {
    orderSnapShots[currentOrder]->pop();
  }
  orderSnapShots[currentOrder]->add(snapshot);
}
void SESAME::LandmarkWindow::clearPyramidalWindow()
{
  std::vector <SESAME::QueueSnapshotPtr>().swap(this->orderSnapShots);
}
