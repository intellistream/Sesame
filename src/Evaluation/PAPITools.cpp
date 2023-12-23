//
// Created by Zijie on 2023/12/20.
//

#include "Evaluation/PAPITools.hpp"
#include "papi.h"

#include <ostream>
#include <iostream>
#include <string>
#include <cassert>
#include <cstring>

namespace SESAME
{
    /* For unhalted cpu cycles */
    const std::string PAPITools::UNHALTED_CORE_CYCLES = "UNHALTED_CORE_CYCLES";

    /* For uops */
    const std::string PAPITools::UOPS_RETIRED = "UOPS_RETIRED";
    const std::string PAPITools::UOPS_ISSUED = "UOPS_ISSUED";

    /* For DTLB */
    const std::string PAPITools::DTLB_MISSES = "PERF_COUNT_HW_CACHE_DTLB:MISS";
    const std::string PAPITools::DTLB_LOAD_MISSES = "DTLB-LOAD-MISSES";
    const std::string PAPITools::DTLB_LOAD_PAGE_WALKS = "DTLB_LOAD_MISSES:WALK_ACTIVE";
    const std::string PAPITools::DTLB_STORE_MISSES = "DTLB-STORE-MISSES";
    const std::string PAPITools::DTLB_STORE_PAGE_WALKS = "DTLB_STORE_MISSES:WALK_ACTIVE";

    /* For ILD */
    const std::string PAPITools::ILD_STALL = "ILD_STALL";

    /* For L1 I-Cache */
    const std::string PAPITools::L1_ICACHE_LOAD_MISSES = "L1-ICACHE-LOAD-MISSES";

    /* For ITLB */
    const std::string PAPITools::ITLB_MISSES = "ITLB-LOAD-MISSES";
    const std::string PAPITools::ITLB_MISSES_WALK_CYCLES = "ITLB_MISSES:WALK_ACTIVE";

    const std::string PAPITools::IDQ_UOPS_NOT_DELIVERED_CORE = "IDQ_UOPS_NOT_DELIVERED:CORE";
    const std::string PAPITools::CPU_CLK_UNHALTED_ANY = "CPU_CLK_UNHALTED";     // The candidate machine uses hyper threading

    /* Do initialization function before using any papi object */
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

    void PAPITools::StartCountingFrontend(const char* filename, int line) {
        if(!allow_adding) {
            std::cout << "Opps, this tool is alreay occupied!" << std::endl;
            return;
        }
        AddNativeEvent(IDQ_UOPS_NOT_DELIVERED_CORE);
        AddNativeEvent(CPU_CLK_UNHALTED_ANY);
        allow_adding = false;
        StartCounting(filename, line);
    }

    void PAPITools::StopCountingFrontend() {
        if (PAPI_stop(eventSet, after_value) != PAPI_OK) exit(-1);
        /* Check cascadelakex_metrics.json to find details about calculation */
        float a = after_value[0] - pre_value[0];  // IDQ_UOPS_NOT_DELIVERED_CORE_cnt
        float b = after_value[1] - pre_value[1];  // CPU_CLK_UNHALTED_ANY_cnt
        float fe_bound = ( a / ( ( 4 ) * ( ( b / 2 ) ) ) );
        std::cout << "a: " << a << std::endl;
        std::cout << "b: " << b << std::endl;
        std::cout << "Frontend bound: " << fe_bound << std::endl;
        DestroyTool();
    }
}