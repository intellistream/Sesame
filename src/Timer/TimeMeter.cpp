// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

#include <Timer/TimeMeter.hpp>
#include <Utils/Logger.hpp>
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
void   SESAME::TimeMeter::overallEndMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.end);
}
// the start  of every xx s
void  SESAME::TimeMeter::overallAccMeasure(){
  clock_gettime(CLOCK_REALTIME, &timer.overallPre);
}

void SESAME::TimeMeter::MeterOverallAccUSEC(int interval){
  AccumulateWithPointTimer(timer.overallPre, timer.end, overallTime,
                           interval, timer.recordOverall);
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


void SESAME::TimeMeter::MeterOnlineAccUSEC(int interval){
  AccumulateWithPointTimer(timer.online_increment_timer_pre, timer.online_timer , onlineTime,
                           interval,
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


void SESAME::TimeMeter::MeterDataInsertAccUSEC(int interval){
  AccumulateWithPointTimer(timer.dataInsertTimer_pre, timer.dataInsertTimer, dataInsertTime,
                           interval,
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
void  SESAME::TimeMeter::MeterConceptDriftAccUSEC(int interval){
  AccumulateWithPointTimer(timer.conceptDriftTimer_pre, timer.conceptDriftTimer, conceptDriftTime,
                           interval,
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
void SESAME::TimeMeter::MeterOutlierDetectionAccUSEC(int interval){
  AccumulateWithPointTimer(timer.outlierDetectionTimer_pre, timer.outlierDetectionTimer, outlierDetectionTime,
                           interval,
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
void SESAME::TimeMeter::MeterPruneAccUSEC(int interval){
  AccumulatePeriodTimer(timer.pruneTimer_pre,timer.pruneTimer,
                        pruneTime,
                        interval,timer.pruneCnt,timer.prune);
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

void SESAME::TimeMeter::MeterSnapshotAccUSEC(int interval){
  AccumulatePeriodTimer(timer.snapshotTimer_pre,timer.snapshotTimer,
                        snapshotTime,
                        interval,timer.snapshotCnt,timer.snapshot);
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
                                                 int interval, std::vector<long> timerVector){
  elapsedTime += MeterUSEC(Now,end);
  if(MeterUSEC(timer.overallPre, Now) >= interval *1000000L)
  {
    timerVector.push_back(elapsedTime);
  }

}
void SESAME::TimeMeter::AccumulatePeriodTimer(timespec Now,timespec end,long elapsedTime, int interval,
                                              int count, std::vector<long> timerVector){
  AccumulateWithPointTimer(Now,end,elapsedTime,interval,timerVector);
  count++;
}

/**
 * print overall time of the algorithms
 */
void SESAME::TimeMeter::breakdown_global( bool initial,bool snapshot,
                                          bool prune, bool refine ) {
  otherTime = overallTime - (dataInsertTime + conceptDriftTime + outlierDetectionTime );
  SESAME_INFO("Overall time is "<< overallTime << "us;");
  if(initial)
  {
    otherTime -= initialTime;
    SESAME_INFO("Initial time is "<< initialTime << "us;");
  }
  if(snapshot)
  {
    otherTime -= snapshotTime;
    SESAME_INFO("snapshot time is "<< snapshotTime << "us;");
  }

  SESAME_INFO("Data insertion time is "<< dataInsertTime << "us;");
  SESAME_INFO("Concept drift time is "<< conceptDriftTime << "us;");
  SESAME_INFO("Outlier Detection time is "<< outlierDetectionTime << "us;");
  if(prune)
  {
    otherTime -= pruneTime;
    SESAME_INFO("prune time is "<< pruneTime << "us;");
  }

  if(refine)
  {
    otherTime -= refinementTime;
    SESAME_INFO("refinement time is "<< refinementTime << "us;");
  }
  SESAME_INFO("other time is "<< otherTime << "us;");
}

