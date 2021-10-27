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
  uint64_t overall_timer, online_timer_pre, online_timer;

  uint64_t initialTimer_pre = 0, initialTimer = 0;

  uint64_t dataInsertTimer_pre = 0, dataInsertTimer = 0;//accumulate.
  uint64_t conceptDriftTimer_pre = 0, conceptDriftTimer = 0;//accumulate.
  uint64_t pruneTimer_pre = 0, pruneTimer = 0;//accumulate.
  uint64_t snapshotTimer_pre = 0, snapshotTimer = 0;//accumulate(Special for CluStream)


  uint64_t refinementTimer = 0;//offline refinement

  std::vector<uint64_t> recordR;
  std::vector<uint64_t> recordS;
  std::vector<int32_t> recordRID;
  std::vector<int32_t> recordSID;
  int match_cnt = 0;
  int joiner_cnt = 0;
  int record_gap = 1;

#endif
};

class TimeMeter {
 private:
  struct timespec start, stop;
 public:
  void START_MEASURE();
  void END_MEASURE();
  long MeterUSEC();//return the meter result in micro second unit.
  //TODO:: All functions need to modify later, just modified and copied from AlianceDB REPO
  /** print out the execution time statistics of stream clustering algorithms which have no-refinement algorithms */
  void breakdown_global(int64_t total_results);

  /** print out the execution time statistics of the stream clustering algorithms that has refinement  */
  void breakdown_global(int64_t result, int nthreads);

  void sortRecords(SESAME::algoType algo_name);

};

#ifndef BEGIN_MEASURE_INITIALIZE
#define BEGIN_MEASURE_INITIALIZE(timer) \
startTimer(&(timer)->initialTimer);
#endif

#ifndef END_MEASURE_INITIALIZE
#define END_MEASURE_INITIALIZE(timer) \
stopTimer(&(timer)->initialTimer);
#endif


#ifndef BEGIN_MEASURE_ONLINE
#define BEGIN_MEASURE_ONLINE(timer) \
startTimer(&(timer)->online_timer);
#endif

#ifndef END_MEASURE_ONLINE
#define END_MEASURE_ONLINE(timer) \
stopTimer(&(timer)->online_timer);
#endif

#ifndef BEGIN_MEASURE_ONLINE_ACC
#define BEGIN_MEASURE_ONLINE_ACC(timer) \
startTimer(&(timer)->online_timer_pre);
#endif

#ifndef /*END_MEASURE_ONLINE_ACC*/NO_TIMING
#define END_MEASURE_ONLINE_ACC(timer) \
accTimer(&(timer)->online_timer_pre, &(timer)->online_timer); /* ONLINE one-pass absorbing data time */
#endif



#ifndef BEGIN_MEASURE_INSERT_ACC
#define BEGIN_MEASURE_INSERT_ACC(timer) \
startTimer(&(timer)->dataInsertTimer_pre);
#endif

#ifndef /*END_MEASURE_INSERT_ACC*/NO_TIMING
#define END_MEASURE_INSERT_ACC(timer) \
accTimer(&(timer)->dataInsertTimer_pre, &(timer)->dataInsertTimer); /* data insert time */
#endif


#ifndef BEGIN_MEASURE_CONDRIFT_ACC
#define BEGIN_MEASURE_CONDRIFT_ACC(timer) \
startTimer(&(timer)->conceptDriftTimer_pre);
#endif

#ifndef /*END_MEASURE_CONDRIFT_ACC*/NO_TIMING
#define END_MEASURE_CONDRIFT_ACC(timer) \
accTimer(&(timer)->conceptDriftTimer_pre, &(timer)->conceptDriftTimer); /* Concept drift time */
#endif


#ifndef BEGIN_MEASURE_PRUNE_ACC
#define BEGIN_MEASURE_PRUNE_ACC(timer) \
startTimer(&(timer)->pruneTimer_pre);
#endif

#ifndef /*END_MEASURE_PRUNE_ACC*/NO_TIMING
#define END_MEASURE_PRUNE_ACC(timer) \
accTimer(&(timer)->pruneTimer_pre, &(timer)->pruneTimer); /* Outlier prune time */
#endif


#ifndef BEGIN_MEASURE_SNAPSHOT_ACC
#define BEGIN_MEASURE_SNAPSHOT_ACC(timer) \
startTimer(&(timer)->snapshotTimer_pre);
#endif

#ifndef /*END_MEASURE_SNAPSHOT_ACC*/NO_TIMING
#define END_MEASURE_SNAPSHOT_ACC(timer) \
accTimer(&(timer)->snapshotTimer_pre, &(timer)->snapshotTimer); /* Taking Snapshots time */
#endif


#ifndef BEGIN_MEASURE_REFINEMENT
#define BEGIN_MEASURE_REFINEMENT(timer) \
startTimer(&(timer)->refinementTimer);
#endif

#ifndef END_MEASURE_REFINEMENT
#define END_MEASURE_REFINEMENT(timer) \
stopTimer(&(timer)->refinementTimer);
#endif


#ifndef OVERALL_START_MEASURE
#define OVERALL_START_MEASURE(timer) \
gettimeofday(&(timer)->start, NULL); \
startTimer(&(timer)->overall_timer); (\
tim)er->partition_timer = 0; /* no partitioning */
#endif

#ifndef OVERALL_END_MEASURE
#define OVERALL_END_MEASURE(timer) \
stopTimer(&(timer)->overall_timer); /* overall */ \
gettimeofday(&(timer)->end, NULL);
#endif

//TODO : re-write END_PROGRESSIVE_MEASURE

}

#endif //SESAME_SRC_TIMER_CYCLEMETER_HPP_
