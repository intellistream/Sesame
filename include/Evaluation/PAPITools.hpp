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
            int tma_level1 = -1;
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
             * Note that you shall use 'AddNativeEvent()'
             * These events can also be used in a set, accroding to TMA */
            static const std::string IDQ_UOPS_NOT_DELIVERED_CORE;
            static const std::string CPU_CLK_UNHALTED;
            static const std::string UOPS_RETIRED_RETIRE_SLOTS;
            static const std::string UOPS_ISSUED_ANY;
            static const std::string INT_MISC_RECOVERY_CYCLES;

            // The 'static const' members here are used to designate the first level of TMA metrics
            static const int FRONTEND_BOUND = 1;
            static const int RETIRING = 2;
            static const int BAD_SPEC = 3;

            PAPITools(std::string info);
            void AddEvent(int event);
            void AddNativeEvent(std::string event_name);
            void StartCounting(const char* filename, int line);
            void StopCounting();
            void DestroyTool();
            void StartCountingTMALevel1(const char* filename, int line, int tma_cata);
            void StopCountingTMALevel1();
    };
}

#endif  // SESAME_INCLUDE_EVALUATION_PAPITOOLS_HPP_
