// Copyright (C) 2021 by the IntelliStream team (https://github.com/intellistream)

#ifndef SESAME_SRC_TIMER_CYCLEMETER_HPP_
#define SESAME_SRC_TIMER_CYCLEMETER_HPP_

#include <Timer/rdtsc.hpp>/* startTimer, stopTimer */
#include <ctime>           /* gettimeofday */
#include <cstddef>
#include <cstdint>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <Algorithm/Algorithm.hpp>

namespace SESAME {
struct T_TIMER {
#ifndef NO_TIMING
  struct timeval start, end;
  uint64_t overall_timer, partition_timer_pre, partition_timer;
  //uint64_t wait_timer_pre = 0, wait_timer = 0;
  uint64_t initialTimer_pre = 0, initialTimer = 0;

  uint64_t dataInsertTimer_pre = 0, dataInsertTimer = 0;//accumulate.
  uint64_t conceptDriftTimer_pre = 0, conceptDriftTimer = 0;//accumulate.
  uint64_t pruneTimer_pre = 0, pruneTimer = 0;//accumulate.
  uint64_t snapshotTimer_pre = 0, snapshotTimer = 0;//accumulate(Special for Clustream?)
  uint64_t refinementTimer_pre = 0, refinementTimer = 0;//offline refinement

  std::vector<uint64_t> recordR;
  std::vector<uint64_t> recordS;
  std::vector<int32_t> recordRID;
  std::vector<int32_t> recordSID;
  int match_cnt = 0;
  int joiner_cnt = 0;
  int record_gap = 1;
  int matches_in_sort = 0;
#endif
};

class TimeMeter {
 private:
  struct timespec start, stop;
 public:
  void START_MEASURE();
  void END_MEASURE();
  long MeterUSEC();//return the meter result in micro second unit.
  //All functions need to modify later, just copy from AlianceDB
  /** print out the execution time statistics of the join, used by no-refinement algorithms */
  void breakdown_global(int64_t total_results, int nthreads, double average_partition_timer, std::string txtFile, int window_size);

  /** print out the execution time statistics of the join, used by algorithms that has refinement  */
  void breakdown_global(int64_t result, int nthreads, T_TIMER *timer, long lastTS, _IO_FILE *pFile);

  void sortRecords(SESAME::algoType algo_name, int exp_id, long lastTS, unsigned long inputs, int64_t matches);

};
#ifndef JOIN_COUNT
#define JOIN_COUNT(timer) \
timer->joiner_cnt++;
#endif

#ifndef BEGIN_MEASURE_BUILD
#define BEGIN_MEASURE_BUILD(timer) \
startTimer(&(timer)->buildtimer);
#endif

#ifndef END_MEASURE_BUILD
#define END_MEASURE_BUILD(timer) \
stopTimer(&(timer)->buildtimer);
#endif

#ifndef BEGIN_MEASURE_BUILD_ACC
#define BEGIN_MEASURE_BUILD_ACC(timer) \
startTimer(&(timer)->buildtimer_pre);
#endif

#ifndef END_MEASURE_BUILD_ACC
#define END_MEASURE_BUILD_ACC(timer) \
accTimer(&(timer)->buildtimer_pre, &(timer)->buildtimer); /* build time */
#endif

#ifndef BEGIN_MEASURE_SORT_ACC
#define BEGIN_MEASURE_SORT_ACC(timer) \
startTimer(&(timer)->sorttimer_pre);
#endif

#ifndef END_MEASURE_SORT_ACC
#define END_MEASURE_SORT_ACC(timer) \
accTimer(&(timer)->sorttimer_pre, &(timer)->sorttimer); /* sort time */
#endif

#ifndef BEGIN_MEASURE_MERGE_ACC
#define BEGIN_MEASURE_MERGE_ACC(timer) \
startTimer(&(timer)->mergetimer_pre);
#endif

#ifndef END_MEASURE_MERGE_ACC
#define END_MEASURE_MERGE_ACC(timer) \
accTimer(&(timer)->mergetimer_pre, &(timer)->mergetimer); /* merge time */
#endif

#ifndef BEGIN_MEASURE_JOIN_MERGE_ACC
#define BEGIN_MEASURE_JOIN_MERGE_ACC(timer) \
startTimer(&(timer)->join_mergetimer_pre);
#endif

#ifndef END_MEASURE_JOIN_MERGE_ACC
#define END_MEASURE_JOIN_MERGE_ACC(timer) \
accTimer(&(timer)->join_mergetimer_pre, &(timer)->join_mergetimer); /* merge time */
#endif

#ifndef BEGIN_MEASURE_JOIN_ACC
#define BEGIN_MEASURE_JOIN_ACC(timer) \
startTimer(&(timer)->join_timer_pre);
#endif

#ifndef END_MEASURE_JOIN_ACC
#define END_MEASURE_JOIN_ACC(timer) \
accTimer(&(timer)->join_timer_pre, &(timer)->join_timer );  /* join time */
#endif

#ifndef BEGIN_MEASURE_WAIT_ACC
#define BEGIN_MEASURE_WAIT_ACC(timer) \
startTimer(&(timer)->wait_timer_pre); /* wait time */
#endif

#ifndef END_MEASURE_WAIT_ACC
#define END_MEASURE_WAIT_ACC(timer) \
accTimer(&(timer)->wait_timer_pre, &(timer)->wait_timer); /* wait time */
#endif

#ifndef SET_WAIT_ACC
#define SET_WAIT_ACC(timer, delay) \
timer->wait_timer=delay; /* wait time */
#endif



/////

#ifndef /*BEGIN_MEASURE_DEBUILD_ACC*/NO_TIMING
#define BEGIN_MEASURE_DEBUILD_ACC(timer) \
startTimer(&(timer)->debuildtimer_pre); /* clean time */
#endif
#ifndef /*END_MEASURE_DEBUILD_ACC*/NO_TIMING
#define END_MEASURE_DEBUILD_ACC(timer) \
accTimer(&(timer)->debuildtimer_pre, &(timer)->debuildtimer); /* clean time */
#endif
#ifndef BEGIN_MEASURE_PARTITION_ACC
#define BEGIN_MEASURE_PARTITION_ACC(timer) \
startTimer(&(timer)->partition_timer_pre);
#endif

#ifndef END_MEASURE_PARTITION_ACC
#define END_MEASURE_PARTITION_ACC(timer) \
accTimer(&(timer)->partition_timer_pre, &(timer)->partition_timer); /* partition time */
#endif

#ifndef BEGIN_MEASURE_PARTITION
#define BEGIN_MEASURE_PARTITION(timer) \
startTimer(&(timer)->partition_timer);
#endif

#ifndef END_MEASURE_PARTITION
#define END_MEASURE_PARTITION(timer) \
stopTimer(&(timer)->partition_timer);
#endif

#ifndef BEGIN_GARBAGE
#define BEGIN_GARBAGE(timer) \
startTimer(&(timer)->garbage_time);
#endif

#ifndef END_GARBAGE
#define END_GARBAGE(timer) \
stopTimer(&(timer)->garbage_time);
#endif


#ifndef START_MEASUREMENT
#define START_MEASUREMENT(timer) \
gettimeofday(&(timer)->start, NULL); \
startTimer(&(timer)->overall_timer); (\
tim)er->partition_timer = 0; /* no partitioning */
#endif

#ifndef END_MEASUREMENT
#define END_MEASUREMENT(timer) \
stopTimer(&(timer)->overall_timer); /* overall */ \
gettimeofday(&(timer)->end, NULL);
#endif

#ifndef END_PROGRESSIVE_MEASURE
#define END_PROGRESSIVE_MEASURE(payloadID, timer, IStupleR)      \
timer->match_cnt++;                                     \
if((timer)->match_cnt == (timer)->record_gap){              \
if(IStupleR){                                        \
auto ts = curtick();                              \
DEBUGMSG("TUPLE R [payload ID:%d] is joined at %lu\n", payloadID, ts)(\
tim)er->recordRID.push_back(payloadID);           (\
tim)er->recordR.push_back(ts);                    \
}else{                                               \
auto ts = curtick();                              \
DEBUGMSG("TUPLE S [payload ID:%d] is joined at %lu\n", payloadID, ts)(\
tim)er->recordSID.push_back(payloadID);           (\
tim)er->recordS.push_back(ts);                    \
}                                                (\
tim)er->match_cnt=0;                                 \
}
#endif



}

#endif //SESAME_SRC_TIMER_CYCLEMETER_HPP_
