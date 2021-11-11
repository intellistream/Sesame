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

void SESAME::TimeMeter::MEASURE(timespec Time){
  clock_gettime(CLOCK_REALTIME, &Time);
}

long SESAME::TimeMeter::MeterUSEC( timespec startAcc,  timespec endAcc){
  return (((endAcc).tv_sec * 1000000L + (endAcc).tv_nsec / 1000L)
  - ((startAcc).tv_sec * 1000000L + (startAcc).tv_nsec / 1000L));
}

//the overall start and end time of every part
void SESAME::TimeMeter::overallStartMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.start);
}
void SESAME::TimeMeter::overallEndMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.end);
}

// the start  of every xx s
void  SESAME::TimeMeter::overallAccMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.overallPre);
}

void SESAME::TimeMeter::MeterOverallAccUSEC(){
  timespec now = {0,0};
  clock_gettime(CLOCK_REALTIME, &now);
  AccumulateWithPointTimer(timer.overallPre, now, overallTime,
                            timer.recordOverall);
  //SESAME_INFO("OVERALL TIME ACC "<<MeterUSEC(timer.overallPre,now));
}

long  SESAME::TimeMeter::MeterOverallUSEC(){
  long overall = MeterUSEC(timer.start,timer.end);
  setOverallTime(overall);
  return overall;
}

//start of online part
void  SESAME::TimeMeter::onlineAccMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.online_increment_timer_pre);
}

//end of online part
void  SESAME::TimeMeter::onlineEndMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.online_timer);
}


void SESAME::TimeMeter::MeterOnlineAccUSEC(){

  AccumulateWithPointTimer(timer.online_increment_timer_pre, timer.online_timer , onlineTime,
                           timer.recordOnline);
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
}


void SESAME::TimeMeter::MeterDataInsertAccUSEC(){
  AccumulateWithPointTimer(timer.dataInsertTimer_pre, timer.dataInsertTimer, dataInsertTime,
                           timer.recordInsert);
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
}
void  SESAME::TimeMeter::MeterConceptDriftAccUSEC(){
  AccumulateWithPointTimer(timer.conceptDriftTimer_pre, timer.conceptDriftTimer, conceptDriftTime,
                           timer.recordConceptDrift);
}
long  SESAME::TimeMeter::MeterConceptDriftUSEC(){
  return conceptDriftTime;
}




//start of outlier Detection part
void  SESAME::TimeMeter::outlierDetectionAccMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.outlierDetectionTimer_pre);
}
//end of outlier Detection  part
void  SESAME::TimeMeter::outlierDetectionEndMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.outlierDetectionTimer);
}
void SESAME::TimeMeter::MeterOutlierDetectionAccUSEC(){
  AccumulateWithPointTimer( timer.outlierDetectionTimer_pre, timer.outlierDetectionTimer, outlierDetectionTime,
                           timer.recordOutlierDetection);
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
}
void SESAME::TimeMeter::MeterPruneAccUSEC(){
  AccumulatePeriodTimer(timer.pruneTimer_pre,timer.pruneTimer,
                        pruneTime,timer.pruneCnt,timer.prune);
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
}

void SESAME::TimeMeter::MeterSnapshotAccUSEC(){
  AccumulatePeriodTimer(timer.snapshotTimer_pre,timer.snapshotTimer,
                        snapshotTime,timer.snapshotCnt,timer.snapshot);
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
  conceptDriftTime = conceptDriftT ;
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

void SESAME::TimeMeter::AccumulateWithPointTimer(timespec Now, timespec end, long elapsedTime,
                                                 std::vector<long> timerVector){
  elapsedTime += MeterUSEC(Now,end);
  if(InsertJudge)
  {
    SESAME_INFO("Keep recording!");
    timerVector.push_back(elapsedTime);
  }
}

void SESAME::TimeMeter::OverallPreUpdate(){
  timespec now={0,0};
  clock_gettime(CLOCK_REALTIME, &now);



  if(MeterUSEC(timer.overallPre, now)/1000000L >= interval)
  {
    clock_gettime(CLOCK_REALTIME, &timer.overallPre);
    InsertJudge = true;
  }
  else
    InsertJudge = false;
}


void SESAME::TimeMeter::AccumulatePeriodTimer( timespec Now,timespec end,long elapsedTime,
                                              int count, std::vector<long> timerVector){
  AccumulateWithPointTimer(Now,end,elapsedTime,timerVector);
  count++;
}

/**
 * print overall time of the algorithms
 */
void SESAME::TimeMeter::breakdown_global( bool initial,bool snapshot,
                                          bool prune, bool refine) {
  otherTime = overallTime - (dataInsertTime + conceptDriftTime + outlierDetectionTime );
  SESAME_INFO("Overall time is "<< overallTime/1000L << "ms;");
  if(initial)
  {
    otherTime -= initialTime;
    SESAME_INFO("Initial time is "<< initialTime/1000L << "ms;");
  }
  if(snapshot)
  {
    otherTime -= snapshotTime;
    SESAME_INFO("snapshot time is "<< snapshotTime/1000L << "ms;");
  }

  SESAME_INFO("Data insertion time is "<< dataInsertTime << "us;");
  SESAME_INFO("Concept drift time is "<< conceptDriftTime<< "us;");
  SESAME_INFO("Outlier Detection time is "<< outlierDetectionTime << "us;");
  if(prune)
  {
    otherTime -= pruneTime;
    SESAME_INFO("prune time is "<< pruneTime << "us;");
  }

  if(refine)
  {
    otherTime -= refinementTime;
    SESAME_INFO("refinement time is "<< refinementTime/1000L << "ms;");
  }
  SESAME_INFO("other time is "<< otherTime/1000000L << "s;");
}

