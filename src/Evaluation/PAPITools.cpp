//
// Created by Zijie on 2023/12/20.
//

#include "Evaluation/PAPITools.hpp"
#include "papi.h"

#include <ostream>
#include <iostream>
#include <string>
#include <cassert>

namespace SESAME
{
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
        if (PAPI_add_event(eventSet, event) != PAPI_OK) exit(-1);
        switch (event)
        {
        case (L3_CACHE_MISS):
            events.push_back("L3_CACHE_MISS");
            break;
        case (TOTAL_INSTR_CNT):
            events.push_back("TOTAL_INSTR_CNT");
            break;
        default:
            break;
        }
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
    }

    /* This function is used to read data */
    void PAPITools::ReadData() {
        if (PAPI_read(eventSet, pre_value) != PAPI_OK) exit(-1);
    }

    /* This function is used to print the colletced data
     * The data is arranged in the order in which events were added */
    void PAPITools::PrintInfo() {
        std::cout << "Printing PAPI info: " << std::endl;
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
}