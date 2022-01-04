// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

#include <Timer/TimeMeter.hpp>
#include <Utils/Logger.hpp>


void  SESAME::TimeMeter::setInterval(int interV){
  this->interval = interV;
}

void SESAME::TimeMeter::START_MEASURE() {
  clock_gettime(CLOCK_REALTIME, &start);
}
void SESAME::TimeMeter::END_MEASURE() {
  clock_gettime(CLOCK_REALTIME, &stop);
}

long SESAME::TimeMeter::MeterUSEC() {
  return (((this->stop).tv_sec * 1000000L + (this->stop).tv_nsec / 1000L)
  - ((this->start).tv_sec * 1000000L + (this->start).tv_nsec / 1000L));
}




/*
 void SESAME::TimeMeter::MEASURE(timespec Time){
  clock_gettime(CLOCK_REALTIME, &Time);
}
 * */


long SESAME::TimeMeter::MeterUSEC( timespec startAcc,  timespec endAcc){
  return (((endAcc).tv_sec * 1000000L + (endAcc).tv_nsec / 1000L)
  - ((startAcc).tv_sec * 1000000L + (startAcc).tv_nsec / 1000L));
}

//the overall start and end time of every part
void SESAME::TimeMeter::overallStartMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.start);
  clock_gettime(CLOCK_REALTIME, &timer.overallPre);
}

void SESAME::TimeMeter::overallEndMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.end);
}

// the start  of every xx s //TODO the initial is overlap with start Measure
void  SESAME::TimeMeter::overallAccMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.overallPre);
}


void SESAME::TimeMeter::OverallPreUpdate(){
  timespec now={0,0};
  clock_gettime(CLOCK_REALTIME, &now);
  if(MeterUSEC(timer.overallPre, now)/1000000L >= interval)
  {
    overallTime += MeterUSEC(timer.overallPre,now);
    clock_gettime(CLOCK_REALTIME, &timer.overallPre);
    InsertJudge = true;
   // SESAME_INFO("Update overall! now overall time is "<<overallTime/ 1000L <<"ms");
    timer.recordOverall.push_back(overallTime);
  }
  else
    InsertJudge = false;
}


long  SESAME::TimeMeter::MeterOverallUSEC(){
  long overall = MeterUSEC(timer.start,timer.end);
  setOverallTime(overall);
  timer.recordOverall.push_back(overall);
  return overall;
}

/**
 * Online Accumulate time calculate:
 * onlineAccMeasure: measure the start of Accumulate online part
 * onlineEndMeasure:  measure the end of Accumulate online part
 * MeterOnlineAccUSEC: calculate the current elapsed time of online part and push the result
 * every Interval second
 * */
//start of online part
void  SESAME::TimeMeter::onlineAccMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.online_increment_timer_pre);
}


//end of online part
void  SESAME::TimeMeter::onlineEndMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.online_timer);
  MeterOnlineAccUSEC();
}


void SESAME::TimeMeter::MeterOnlineAccUSEC(){
 onlineTime += MeterUSEC(timer.online_increment_timer_pre,timer.online_timer);
 if(InsertJudge)
 {
  // SESAME_INFO("Update ! NOW online IS <<"<<onlineTime/1000L <<"ms");
   timer.recordOnline.push_back(onlineTime);
 }
}

long SESAME::TimeMeter::getOnlineEtime(){
  return onlineTime;
}

//start of initial  part
void  SESAME::TimeMeter::initialMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.initialTimerStart);
}

//end of initial part
void  SESAME::TimeMeter::initialEndMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.initialTimer);
}

long  SESAME::TimeMeter::MeterInitialUSEC(){
  long overall = MeterUSEC(timer.initialTimerStart,timer.initialTimer);
  setInitialTime(overall);
  return overall;
}

//start of data Insert  part
void  SESAME::TimeMeter::dataInsertAccMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.dataInsertTimer_pre);
}

//end of data Insert  part
void  SESAME::TimeMeter::dataInsertEndMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.dataInsertTimer);
  MeterDataInsertAccUSEC();
}


void SESAME::TimeMeter::MeterDataInsertAccUSEC(){
  dataInsertTime += MeterUSEC(timer.dataInsertTimer_pre,timer.dataInsertTimer);
  if(InsertJudge)
  {
   // SESAME_INFO("Update ! data insert IS <<"<<dataInsertTime/1000L <<"ms");
    timer.recordInsert.push_back(dataInsertTime);
  }
}
long SESAME::TimeMeter::MeterDataInsertUSEC(){
  return dataInsertTime;
}

//start of concept drift  part
void  SESAME::TimeMeter::conceptDriftAccMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.conceptDriftTimer_pre);
}
//end of concept drift  part
void  SESAME::TimeMeter::conceptDriftEndMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.conceptDriftTimer);
  MeterConceptDriftAccUSEC();
}
void  SESAME::TimeMeter::MeterConceptDriftAccUSEC(){
  onlineClusterUpdateTime += MeterUSEC(timer.conceptDriftTimer_pre,timer.conceptDriftTimer);
  if(InsertJudge)
  {
   // SESAME_INFO("Update ! Concept drift IS <<"<<conceptDriftTime/1000L <<"ms");
   timer.recordConceptDrift.push_back(onlineClusterUpdateTime);
  }
}
long  SESAME::TimeMeter::MeterConceptDriftUSEC(){
  return onlineClusterUpdateTime;
}




//start of outlier Detection part
void  SESAME::TimeMeter::outlierDetectionAccMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.outlierDetectionTimer_pre);
}
//end of outlier Detection  part
void  SESAME::TimeMeter::outlierDetectionEndMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.outlierDetectionTimer);
  MeterOutlierDetectionAccUSEC();
}
void SESAME::TimeMeter::MeterOutlierDetectionAccUSEC(){
  outlierDetectionTime += MeterUSEC(timer.outlierDetectionTimer_pre,timer.outlierDetectionTimer);
  if(InsertJudge)
  {
   // SESAME_INFO("Update ! Outlier detection IS <<"<<outlierDetectionTime/1000L <<"ms");
    timer.recordOutlierDetection.push_back(outlierDetectionTime);
  }
}




long SESAME::TimeMeter::MeterOutlierDetectionUSEC(){
  return outlierDetectionTime;
}


//start of prune part
void  SESAME::TimeMeter::pruneAccMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.pruneTimer_pre);
}

//end of prune  part
void  SESAME::TimeMeter::pruneEndMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.pruneTimer);
  MeterPruneAccUSEC();
}
void SESAME::TimeMeter::MeterPruneAccUSEC(){
  pruneTime += MeterUSEC(timer.pruneTimer_pre,timer.pruneTimer);
  if(InsertJudge)
  {
   // SESAME_INFO("Update! prune Time IS <<"<<pruneTime/1000L <<"ms");
    timer.prune.push_back(pruneTime);
  }
  timer.pruneCnt++;
}
long SESAME::TimeMeter::MeterPruneUSEC(){
  return pruneTime;
}


//start of snapshot  part
void  SESAME::TimeMeter::snapshotAccMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.snapshotTimer_pre);
}
//end of snapshot  part
void  SESAME::TimeMeter::snapshotEndMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.snapshotTimer);
  MeterSnapshotAccUSEC();
}

void SESAME::TimeMeter::MeterSnapshotAccUSEC(){
  //if(InsertJudge)
  //  SESAME_INFO("Snapshot ES time is ");
  snapshotTime += MeterUSEC(timer.snapshotTimer_pre,timer.snapshotTimer);
  if(InsertJudge)
  {
 //   SESAME_INFO("Update! snapshot Time IS <<"<<snapshotTime/1000L <<"ms");
    timer.snapshot.push_back(snapshotTime);
  }
  timer.snapshotCnt++;

}
long SESAME::TimeMeter::MeterSnapshotUSEC(){
  return snapshotTime;
}

//start of refinement  part
void  SESAME::TimeMeter::refinementAccMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.refinementStart);
}
//end of refinement  part
void  SESAME::TimeMeter::refinementEndMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.refinementTimer);
}
long  SESAME::TimeMeter::MeterRefinementUSEC(){
  long refinement = MeterUSEC(timer.refinementStart,timer.refinementTimer);
  setRefinementTime(refinement);
  return refinement;
}

void SESAME::TimeMeter::setOverallTime(long overallT){
  overallTime = overallT;
}
void SESAME::TimeMeter::setOnlineTime(long onlineT){
  onlineTime = onlineT;
}
void SESAME::TimeMeter::setDataInsertTime(long dataInsertT){
  dataInsertTime = dataInsertT;
}
void SESAME::TimeMeter::setConceptDriftTime(long conceptDriftT){
  onlineClusterUpdateTime = conceptDriftT ;
}
void SESAME::TimeMeter::setOutlierDetectionTime(long outlierDetectionT){
  outlierDetectionTime = outlierDetectionT;
}

void SESAME::TimeMeter::setInitialTime(long initialT){
  initialTime = initialT;
}
// if possible
void SESAME::TimeMeter::setRefinementTime(long refinementT){
  refinementTime= refinementT;
}
// if possible
void SESAME::TimeMeter::setSnapshotTime(long snapshotT){
  snapshotTime = snapshotT;
}
void SESAME::TimeMeter::setPruneTime(long pruneT){
  pruneTime = pruneT;
}

void SESAME::TimeMeter::AccumulateWithPointTimer(timespec start, timespec end, long elapsedTime,
                                                 std::vector<long> timerVector){
  //SESAME_INFO("Keep recording! NOW ES IS <<"<<elapsedTime);
  elapsedTime += MeterUSEC(start,end);
  if(InsertJudge)
  {
   // SESAME_INFO("Update recording! NOW ES IS <<"<<elapsedTime);
    timerVector.push_back(elapsedTime);
  }
}




/**
 * print overall time of the algorithms
 */
void SESAME::TimeMeter::breakdown_global( bool initial,bool snapshot,
                                          bool prune, bool refine) {
  otherTime = overallTime - (dataInsertTime + onlineClusterUpdateTime + outlierDetectionTime );
  SESAME_DEBUG("Overall time is "<< overallTime/1000L << "ms;" );//timer.recordOverall[timer.recordOverall.size()-1]
  if(initial)
  {
    otherTime -= initialTime;
    SESAME_DEBUG("Initial time is "<< initialTime/1000L << "ms;");
  }
  if(snapshot)
  {
    otherTime -= snapshotTime;
    SESAME_DEBUG("snapshot time is "<< snapshotTime/1000L << "ms;");
  }

  SESAME_DEBUG("Data insertion time is "<< dataInsertTime/1000L << "ms;");
  SESAME_DEBUG("online cluster update time is "<< onlineClusterUpdateTime/1000L<< "ms;");
  SESAME_DEBUG("Outlier Detection time is "<< outlierDetectionTime/1000L << "ms;");
  if(prune)
  {
    otherTime -= pruneTime;
    SESAME_DEBUG("prune time is "<< pruneTime/1000L << "ms;");
  }

  if(refine)
  {
    otherTime -= refinementTime;
    SESAME_DEBUG("refinement time is "<< refinementTime/1000L << "ms;");
  }
  SESAME_DEBUG("other time is "<< otherTime/1000L << "ms;");
}

