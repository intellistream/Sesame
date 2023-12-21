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

            void InitializePAPI(std::string info);  
            void ReadData();
            void PrintInfo();

        public:
            /* The 'static const' members here are used to designate the events
             * Note that some events are unable to work together */
            static const int L3_CACHE_MISS = PAPI_L3_TCM;
            static const int TOTAL_INSTR_CNT = PAPI_TOT_INS;

            PAPITools(std::string info);
            void AddEvent(int event);
            void StartCounting(const char* filename, int line);
            void StopCounting();
            void DestroyTool();
    };
}

#endif  // SESAME_INCLUDE_EVALUATION_PAPITOOLS_HPP_
