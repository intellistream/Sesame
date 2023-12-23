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
            bool allow_adding = true;

            void InitializePAPI(std::string info);  
            void ReadData();
            void PrintInfo();

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
             * Note that you shall use 'AddNativeEvent()' */
            static const std::string UOPS_RETIRED;
            static const std::string UOPS_ISSUED;
            static const std::string DTLB_MISSES;
            static const std::string DTLB_LOAD_MISSES;
            static const std::string DTLB_LOAD_PAGE_WALKS;
            static const std::string DTLB_STORE_MISSES;
            static const std::string DTLB_STORE_PAGE_WALKS;
            static const std::string ILD_STALL;
            static const std::string L1_ICACHE_LOAD_MISSES;
            static const std::string ITLB_MISSES;
            static const std::string ITLB_MISSES_WALK_CYCLES;
            static const std::string UNHALTED_CORE_CYCLES;

            static const std::string IDQ_UOPS_NOT_DELIVERED_CORE;
            static const std::string CPU_CLK_UNHALTED_ANY;

            PAPITools(std::string info);
            void AddEvent(int event);
            void AddNativeEvent(std::string event_name);
            void StartCounting(const char* filename, int line);
            void StopCounting();
            void DestroyTool();
            void StartCountingFrontend(const char* filename, int line);
            void StopCountingFrontend();
    };
}

#endif  // SESAME_INCLUDE_EVALUATION_PAPITOOLS_HPP_
