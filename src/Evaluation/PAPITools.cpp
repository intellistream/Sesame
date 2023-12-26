//
// Created by Zijie on 2023/12/20.
//

#include "Evaluation/PAPITools.hpp"
#include "Utils/Logger.hpp"
#include "papi.h"


#include <ostream>
#include <iostream>
#include <string>
#include <cassert>
#include <cstring>

namespace SESAME
{
    const std::string PAPITools::IDQ_UOPS_NOT_DELIVERED_CORE = "IDQ_UOPS_NOT_DELIVERED:CORE";
    const std::string PAPITools::CPU_CLK_UNHALTED = "CPU_CLK_UNHALTED";     // The candidate machine uses hyper threading
    const std::string PAPITools::UOPS_RETIRED_RETIRE_SLOTS = "UOPS_RETIRED:RETIRE_SLOTS";
    const std::string PAPITools::UOPS_ISSUED_ANY = "UOPS_ISSUED:ANY"; 
    const std::string PAPITools::INT_MISC_RECOVERY_CYCLES = "INT_MISC:RECOVERY_CYCLES";
    const std::string PAPITools::IDQ_UOPS_NOT_DELIVERED_CYCLES_0_UOPS_DELIV_CORE = "IDQ_UOPS_NOT_DELIVERED:CYCLES_0_UOPS_DELIV_CORE";
    const std::string PAPITools::CYCLE_ACTIVITY_STALLS_MEM_ANY = "CYCLE_ACTIVITY:STALLS_MEM_ANY";
    const std::string PAPITools::EXE_ACTIVITY_BOUND_ON_STORES = "EXE_ACTIVITY:BOUND_ON_STORES";
    const std::string PAPITools::CYCLE_ACTIVITY_STALLS_TOTAL = "CYCLE_ACTIVITY:STALLS_TOTAL";
    const std::string PAPITools::EXE_ACTIVITY_1_PORTS_UTIL = "EXE_ACTIVITY:1_PORTS_UTIL";
    const std::string PAPITools::EXE_ACTIVITY_2_PORTS_UTIL = "EXE_ACTIVITY:2_PORTS_UTIL";

    /* Do initialization function before using any papi object */
    /* TODO:Give error information using SESAME_INFO etc. */
    PAPITools::PAPITools(std::string info) {
        InitializePAPI(info);
    }

    void PAPITools::InitializePAPI(std::string info) {
        if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT) exit(-1);
        if (PAPI_thread_init(pthread_self) != PAPI_OK) exit(1); 
        eventSet = PAPI_NULL;  
        if (PAPI_create_eventset(&eventSet) != PAPI_OK) exit(1);
        std::cout << "-----Finished Intializing " << info << "-----" << std::endl;
    }

    /* Add an event to the tool by using 'static int' provides by the PAPITools Class
     * Note that some events may not be able to work together */
    void PAPITools::AddEvent(int event) {
        if(!allow_adding)  {
            std::cout << "Opps, event addition is not allow!" << std::endl;
            return;
        }
        if (PAPI_add_event(eventSet, event) != PAPI_OK) exit(-1);
        switch (event)
        {
        case (TOTAL_INSTR_CNT):
            events.push_back("TOTAL_INSTR_CNT");
            break;
        case (TOTAL_CYCLE_CNT):
            events.push_back("TOTAL_CYCLE_CNT");
            break;
        case (L1_DCACHE_MISS):
            events.push_back("L1_DCACHE_MISS");
            break;
        case (L2_CACHE_MISS):
            events.push_back("L2_CACHE_MISS");
            break;
        case (L3_CACHE_MISS):
            events.push_back("L3_CACHE_MISS");
            break;
        default:
            break;
        }
    }

    void PAPITools::AddNativeEvent(std::string event_name) {
        int native;
        if(!allow_adding)  {
            std::cout << "Opps, event addition is not allow!" << std::endl;
            return;
        }
		if (PAPI_event_name_to_code(event_name.c_str(), &native) != PAPI_OK) exit(-1);
        if (PAPI_add_event(eventSet, native) != PAPI_OK) exit(-1);
        events.push_back(event_name);
    }

    /* Call this funtion to start collecting statistics for registered events
     * Calling this function will save the current data in function pre_value
     * The values are arranged in the array in the order in which events were added */
    void PAPITools::StartCounting(const char* filename, int line) {
        std::cout << "-----PAPI Starts Running in file " << filename << " at line " << line << "-----" << std::endl;
        if (PAPI_start(eventSet) != PAPI_OK) exit(-1);
        ReadData();
    }

    /* Call this funtion to stop collecting statistics for registered events
     * Calling this function will save the current data in function after_value 
     * The values are arranged in the array in the order in which events were added */
    void PAPITools::StopCounting() {
        if (PAPI_stop(eventSet, after_value) != PAPI_OK) exit(-1);
        PrintInfo();
        DestroyTool();
    }

    /* This function is used to read data */
    void PAPITools::ReadData() {
        if (PAPI_read(eventSet, pre_value) != PAPI_OK) exit(-1);
    }

    /* This function is used to print the colletced data
     * The data is arranged in the order in which events were added */
    void PAPITools::PrintInfo() {
        std::cout << "-----PAPI Starts Printing Info-----" << std::endl;
        for(int i = 0;i < events.size();i++) {
            std::cout << "Event " << events[i] << ":" << after_value[i] - pre_value[i] << std::endl;
        }
    }

    /* When you finished using a papi tool object, call this function */
    void PAPITools::DestroyTool() {
        if (PAPI_cleanup_eventset(eventSet) != PAPI_OK) exit(-1);
        if (PAPI_destroy_eventset(&eventSet) != PAPI_OK) exit(-1);
        PAPI_shutdown();
        std::cout << "-----PAPI Stops Running and Resets-----" << std::endl;
    }

    /* Use this funciton to start counting TMA metric quickly */
    void PAPITools::StartCountingTMA(const char* filename, int line, int level, int tma_cata) {
        tma_level = level;
        switch (level) {
            case LEVEL1:
                StartCountingTMALevel1(filename, line, tma_cata);
                break;
            case LEVEL2:
                StartCountingTMALevel2(filename, line, tma_cata);
                break;
            case LEVEL3:
                break;
            default:
                break;
        }
    }

    /* Stop TMA calculation */
    void PAPITools::StopCountingTMA() {
        switch (tma_level) {
            case LEVEL1:
                StopCountingTMALevel1();
                break;
            case LEVEL2:
                StopCountingTMALevel2();
                break;
            case LEVEL3:
                break;
            default:
                break;
        }
    }

    void PAPITools::StartCountingTMALevel1(const char* filename, int line, int tma_cata) {
        if(!allow_adding) {
            std::cout << "Opps, this tool is alreay occupied!" << std::endl;
            return;
        }
        tma_level1 = tma_cata;
        switch (tma_cata) {
            case FRONTEND_BOUND:
                AddNativeEvent(IDQ_UOPS_NOT_DELIVERED_CORE);
                AddNativeEvent(CPU_CLK_UNHALTED);
                break;
            case BACKEND_BOUND:
                AddNativeEvent(IDQ_UOPS_NOT_DELIVERED_CORE);
                AddNativeEvent(CPU_CLK_UNHALTED);
                AddNativeEvent(UOPS_ISSUED_ANY);
                AddNativeEvent(INT_MISC_RECOVERY_CYCLES);
                break;
            case RETIRING:
                AddNativeEvent(UOPS_RETIRED_RETIRE_SLOTS);
                AddNativeEvent(CPU_CLK_UNHALTED);
                break;
            case BAD_SPEC:
                AddNativeEvent(UOPS_ISSUED_ANY);
                AddNativeEvent(UOPS_RETIRED_RETIRE_SLOTS);
                AddNativeEvent(INT_MISC_RECOVERY_CYCLES);
                AddNativeEvent(CPU_CLK_UNHALTED);
                break;
            default:
                break;
        }
        allow_adding = false;
        StartCounting(filename, line);
    }

    /* This function is used to count metrics in the first level of TMA */
    void PAPITools::StopCountingTMALevel1() {
        if (PAPI_stop(eventSet, after_value) != PAPI_OK) exit(-1);
        /* Check cascadelakex_metrics.json to find details about calculation */
        if (tma_level1 == FRONTEND_BOUND) {
            float a = after_value[0] - pre_value[0];  // IDQ_UOPS_NOT_DELIVERED_CORE_cnt
            float b = after_value[1] - pre_value[1];  // CPU_CLK_UNHALTED_cnt
            float fe_bound = ( a / ( ( 4 * b ) ) );
            std::cout << "Frontend bound: " << fe_bound << std::endl;
        } else if(tma_level1 == BACKEND_BOUND){
            float a = after_value[0] - pre_value[0];  // UOPS_ISSUED_ANY_cnt
            float b = after_value[1] - pre_value[1];  // UOPS_RETIRED_RETIRE_SLOTS_cnt
            float c = after_value[2] - pre_value[2];  // INT_MISC_RECOVERY_CYCLES_cnt
            float d = after_value[3] - pre_value[3];  // CPU_CLK_UNHALTED_cnt
            float bad_spec = (1 - ( a / ( 4 *  b ) ) - ( c + ( 4 * d ) ) / ( ( 4 * b ) ) );
            std::cout << "Backend bound: " << bad_spec << std::endl;
        } else if (tma_level1 == RETIRING) {
            float a = after_value[0] - pre_value[0];  // UOPS_RETIRED_RETIRE_SLOTS_cnt
            float b = after_value[1] - pre_value[1];  // CPU_CLK_UNHALTED_cnt
            float retiring = ( a / ( ( 4 * b ) ) );
            std::cout << "Retiring: " << retiring << std::endl;
        } else if (tma_level1 == BAD_SPEC) {
            float a = after_value[0] - pre_value[0];  // UOPS_ISSUED_ANY_cnt
            float b = after_value[1] - pre_value[1];  // UOPS_RETIRED_RETIRE_SLOTS_cnt
            float c = after_value[2] - pre_value[2];  // INT_MISC_RECOVERY_CYCLES_cnt
            float d = after_value[3] - pre_value[3];  // CPU_CLK_UNHALTED_cnt
            float bad_spec = ( a - ( b ) + ( 4 * c ) ) / ( 4 * d );
            std::cout << "Bad speculation: " << bad_spec << std::endl;
        }
        DestroyTool();
    }

    /* This function is used to count metrics in the second level of TMA
     * Not all the metrics are support in this func, but it is extendable */
    void PAPITools::StartCountingTMALevel2(const char* filename, int line, int tma_cata) {
        if(!allow_adding) {
            std::cout << "Opps, this tool is alreay occupied!" << std::endl;
            return;
        }
        tma_level2 = tma_cata;
        switch (tma_level2) {
            case FETCH_LATENCY:
                AddNativeEvent(PAPITools::IDQ_UOPS_NOT_DELIVERED_CYCLES_0_UOPS_DELIV_CORE);
                AddNativeEvent(PAPITools::CPU_CLK_UNHALTED);
                break;
            case FETCH_BANDWIDTH:
                AddNativeEvent(PAPITools::IDQ_UOPS_NOT_DELIVERED_CORE);
                AddNativeEvent(PAPITools::CPU_CLK_UNHALTED);
                AddNativeEvent(PAPITools::IDQ_UOPS_NOT_DELIVERED_CYCLES_0_UOPS_DELIV_CORE);
                break;
            case CORE_BOUND_P1:
                /* The sub events of CORE_BOUND cannot be measured together */
                AddNativeEvent(PAPITools::IDQ_UOPS_NOT_DELIVERED_CORE); 
                AddNativeEvent(PAPITools::CPU_CLK_UNHALTED);        
                AddNativeEvent(PAPITools::UOPS_ISSUED_ANY);              
                AddNativeEvent(PAPITools::INT_MISC_RECOVERY_CYCLES);    
                break;
            case CORE_BOUND_P2:
                AddNativeEvent(PAPITools::CYCLE_ACTIVITY_STALLS_MEM_ANY);
                AddNativeEvent(PAPITools::CYCLE_ACTIVITY_STALLS_TOTAL);  
                AddNativeEvent(PAPITools::UOPS_RETIRED_RETIRE_SLOTS);    
                break;
            case CORE_BOUND_P3:
                AddNativeEvent(PAPITools::EXE_ACTIVITY_BOUND_ON_STORES);
                AddNativeEvent(PAPITools::EXE_ACTIVITY_1_PORTS_UTIL);
                AddNativeEvent(PAPITools::EXE_ACTIVITY_2_PORTS_UTIL);
            default:
                break;
        }
        allow_adding = false;
        StartCounting(filename, line);
    }

    void PAPITools::StopCountingTMALevel2() {
        if (PAPI_stop(eventSet, after_value) != PAPI_OK) exit(-1);
        /* Check cascadelakex_metrics.json to find details about calculation */
        if (tma_level2 == FETCH_LATENCY) {
            float a = after_value[0] - pre_value[0];  // IDQ_UOPS_NOT_DELIVERED_CYCLES_0_UOPS_DELIV_CORE_cnt
            float b = after_value[1] - pre_value[1];  // CPU_CLK_UNHALTED_cnt
            float fetch_latency = ( ( 4 ) * a / ( 4 * b ) );
            std::cout << "Fetch latency: " << fetch_latency << std::endl;
        } else if (tma_level2 == FETCH_BANDWIDTH) {
            float a = after_value[0] - pre_value[0];  // IDQ_UOPS_NOT_DELIVERED_CORE_cnt
            float b = after_value[1] - pre_value[1];  // CPU_CLK_UNHALTED_cnt
            float c = after_value[2] - pre_value[2];  // IDQ_UOPS_NOT_DELIVERED_CYCLES_0_UOPS_DELIV_CORE_cnt
            float fetch_bandwidth = ( ( a / ( 4 * b ) ) - ( ( 4 ) * c / ( 4 * b ) ) );
            std::cout << "Fetch bandwidth: " << fetch_bandwidth << std::endl;
        } else if (tma_level2 == CORE_BOUND_P1) {
            std::cout << "IDQ_UOPS_NOT_DELIVERED_CORE_cnt: " << after_value[0] - pre_value[0] << std::endl;
            std::cout << "CPU_CLK_UNHALTED_cnt: " << after_value[1] - pre_value[1] << std::endl;
            std::cout << "UOPS_ISSUED_ANY_cnt: " << after_value[2] - pre_value[2] << std::endl;
            std::cout << "INT_MISC_RECOVERY_CYCLES_cnt: " << after_value[3] - pre_value[3] << std::endl;
        } else if (tma_level2 == CORE_BOUND_P2) {
            std::cout << "CYCLE_ACTIVITY_STALLS_MEM_ANY_cnt" << after_value[0] - pre_value[0] << std::endl;
            std::cout << "CYCLE_ACTIVITY_STALLS_TOTAL_cnt" << after_value[1] - pre_value[1] << std::endl;
            std::cout << "UOPS_RETIRED_RETIRE_SLOTS_cnt" << after_value[2] - pre_value[2] << std::endl;
        } else if (tma_level2 == CORE_BOUND_P3) {
            std::cout << "EXE_ACTIVITY_BOUND_ON_STORES_cnt" << after_value[0] - pre_value[0] << std::endl;
            std::cout << "EXE_ACTIVITY_1_PORTS_UTIL_cnt" << after_value[1] - pre_value[1] << std::endl;
            std::cout << "EXE_ACTIVITY_2_PORTS_UTIL_cnt" << after_value[2] - pre_value[2] << std::endl;
        }
        DestroyTool();
    }
}