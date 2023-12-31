//
// Created by Zijie on 2023/12/20.
//

#ifndef SESAME_INCLUDE_EVALUATION_PAPITOOLS_HPP_
#define SESAME_INCLUDE_EVALUATION_PAPITOOLS_HPP_

#include "papi.h"

#include <vector>
#include <string>

namespace SESAME
{
    /* This class can be used to create a PAPITools object
     * You can used the object to detect the hardware performance of the program */
    class PAPITools
    {
        private:
            std::vector<std::string> events;
            long long pre_value[64] = {0};
            long long after_value[64] = {0};
            int eventSet;
            int tma_level = -1;                              // Designate the level of tma
            int tma_level1 = -1, tma_level2 = -1, tma_level3 = -1;       // Specific which metric to count
            bool allow_adding = true;

            /* Assistant functions */
            void InitializePAPI(std::string info);  
            void ReadData();
            void PrintInfo();
            template <typename type>
            void WriteFile(const std::string& filename, std::initializer_list<type> values);

            /* TMA functions */
            void StartCountingTMALevel1(const char* filename, int line, int tma_cata);
            void StopCountingTMALevel1();
            void StartCountingTMALevel2(const char* filename, int line, int tma_cata);
            void StopCountingTMALevel2();
            void StartCountingTMALevel3(const char* filename, int line, int tma_cata);
            void StopCountingTMALevel3();

        public:
            /* The 'static const' members here are used to designate the events
             * These members include preset events and native events
             * Note that some events are unable to work together */
            static const int TOTAL_INSTR_CNT = PAPI_TOT_INS;
            static const int TOTAL_CYCLE_CNT = PAPI_TOT_CYC;
            static const int L1_DCACHE_MISS = PAPI_L1_DCM;
            static const int L2_CACHE_MISS = PAPI_L2_TCM;
            static const int L3_CACHE_MISS = PAPI_L3_TCM;

            /* Native events are listed as follow
             * Note that you shall use 'AddNativeEvent()'
             * These events can also be used in a set, accroding to TMA */
            static const std::string CPU_CLK_UNHALTED;
            static const std::string UOPS_RETIRED_RETIRE_SLOTS;
            static const std::string UOPS_ISSUED_ANY;
            static const std::string INT_MISC_RECOVERY_CYCLES;
            static const std::string CYCLE_ACTIVITY_STALLS_MEM_ANY;
            static const std::string CYCLE_ACTIVITY_STALLS_TOTAL;
            static const std::string CYCLE_ACTIVITY_STALLS_L1D_MISS;
            static const std::string CYCLE_ACTIVITY_STALLS_L2_MISS;
            static const std::string CYCLE_ACTIVITY_STALLS_L3_MISS;
            static const std::string EXE_ACTIVITY_BOUND_ON_STORES;
            static const std::string EXE_ACTIVITY_1_PORTS_UTIL;
            static const std::string EXE_ACTIVITY_2_PORTS_UTIL;
            static const std::string EXE_ACTIVITY_EXE_BOUND_0_PORTS;
            static const std::string ARITH_DIVIDER_ACTIVE;
            static const std::string IDQ_UOPS_NOT_DELIVERED_CYCLES_0_UOPS_DELIV_CORE;
            static const std::string IDQ_UOPS_NOT_DELIVERED_CORE;
            static const std::string IDQ_ALL_MITE_CYCLES_ANY_UOPS;
            static const std::string IDQ_ALL_MITE_CYCLES_4_UOPS;
            static const std::string IDQ_ALL_DSB_CYCLES_ANY_UOPS;
            static const std::string IDQ_ALL_DSB_CYCLES_4_UOPS;
            static const std::string LSD_CYCLES_ACTIVE;
            static const std::string LSD_CYCLES_4_UOPS;
            static const std::string MEM_LOAD_RETIRED_L2_HIT;
            static const std::string MEM_LOAD_RETIRED_FB_HIT;
            static const std::string MEM_LOAD_RETIRED_L1_MISS;
            static const std::string MEM_LOAD_L3_MISS_RETIRED_REMOTE_DRAM;
            static const std::string MEM_LOAD_L3_MISS_RETIRED_LOCAL_DRAM;
            static const std::string MEM_LOAD_L3_MISS_RETIRED_REMOTE_FWD;
            static const std::string MEM_LOAD_L3_MISS_RETIRED_REMOTE_HITM;
            static const std::string MEM_LOAD_RETIRED_LOCAL_PMM;
            static const std::string MEM_LOAD_L3_MISS_RETIRED_REMOTE_PMM;
            static const std::string L1D_PEND_MISS_FB_FULL_c1;

            /* The 'static const' members here are used to designate the level of TMA */
            static const int LEVEL1 = 1;
            static const int LEVEL2 = 2;
            static const int LEVEL3 = 3;

            /* The 'static const' members here are used to designate the first level of TMA metrics */
            static const int FRONTEND_BOUND = 1;
            static const int RETIRING = 2;
            static const int BAD_SPEC = 3;
            static const int BACKEND_BOUND = 4;

            /* The 'static const' members here are used to designate the second level of TMA metrics
             * Note that the events to calculate CORE_BOUND can also used to count MEMORY_BOUND are  */
            static const int FETCH_LATENCY = 1;
            static const int FETCH_BANDWIDTH = 2;
            static const int CORE_BOUND_P1 = 3;
            static const int CORE_BOUND_P2 = 4;
            static const int CORE_BOUND_P3 = 5;
            static const int MEMORY_BOUND_P1 = 3;
            static const int MEMORY_BOUND_P2 = 4;
            static const int MEMORY_BOUND_P3 = 5;

            /* The 'static const' members here are used to designate the thrid level of TMA metrics */
            static const int DIVIDER = 1;
            static const int PORTS_UTILIZATION_P1 = 2;
            static const int PORTS_UTILIZATION_P2 = 3;
            static const int L1_BOUND = 4;
            static const int L2_BOUND_P1 = 5;
            static const int L2_BOUND_P2 = 6;
            static const int L3_BOUND = 7;
            static const int DRAM_BOUND_P1 = 8;
            static const int DRAM_BOUND_P2 = 9;
            static const int DRAM_BOUND_P3 = 10;
            static const int PMM_BOUND = 11;
            static const int STORE_BOUND = 12;
            static const int MITE = 13;
            static const int DSB = 14;
            static const int LSD = 15;
            

            PAPITools(std::string info);
            void AddEvent(int event);
            void AddNativeEvent(std::string event_name);
            void StartCounting(const char* filename, int line);
            void StopCounting();
            void DestroyTool();
            void StartCountingTMA(const char* filename, int line, int level, int tma_cata);
            void StopCountingTMA();
    };
}

#endif  // SESAME_INCLUDE_EVALUATION_PAPITOOLS_HPP_
