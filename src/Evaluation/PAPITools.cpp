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
#include <algorithm>
#include <fstream>

namespace SESAME
{
    const std::string PAPITools::IDQ_UOPS_NOT_DELIVERED_CORE = "IDQ_UOPS_NOT_DELIVERED:CORE";
    const std::string PAPITools::CPU_CLK_UNHALTED = "CPU_CLK_UNHALTED";     // The candidate machine uses hyper threading
    const std::string PAPITools::UOPS_RETIRED_RETIRE_SLOTS = "UOPS_RETIRED:RETIRE_SLOTS";
    const std::string PAPITools::UOPS_ISSUED_ANY = "UOPS_ISSUED:ANY"; 
    const std::string PAPITools::INT_MISC_RECOVERY_CYCLES = "INT_MISC:RECOVERY_CYCLES";
    const std::string PAPITools::IDQ_UOPS_NOT_DELIVERED_CYCLES_0_UOPS_DELIV_CORE = "IDQ_UOPS_NOT_DELIVERED:CYCLES_0_UOPS_DELIV_CORE";
    const std::string PAPITools::CYCLE_ACTIVITY_CYCLES_MEM_ANY = "CYCLE_ACTIVITY:CYCLES_MEM_ANY";
    const std::string PAPITools::CYCLE_ACTIVITY_STALLS_MEM_ANY = "CYCLE_ACTIVITY:STALLS_MEM_ANY";
    const std::string PAPITools::CYCLE_ACTIVITY_STALLS_TOTAL = "CYCLE_ACTIVITY:STALLS_TOTAL";
    const std::string PAPITools::CYCLE_ACTIVITY_STALLS_L1D_MISS = "CYCLE_ACTIVITY:STALLS_L1D_MISS";
    const std::string PAPITools::CYCLE_ACTIVITY_STALLS_L2_MISS = "CYCLE_ACTIVITY:STALLS_L2_MISS";
    const std::string PAPITools::CYCLE_ACTIVITY_STALLS_L3_MISS = "CYCLE_ACTIVITY:STALLS_L3_MISS";
    const std::string PAPITools::EXE_ACTIVITY_EXE_BOUND_0_PORTS = "EXE_ACTIVITY:EXE_BOUND_0_PORTS";
    const std::string PAPITools::EXE_ACTIVITY_BOUND_ON_STORES = "EXE_ACTIVITY:BOUND_ON_STORES";
    const std::string PAPITools::EXE_ACTIVITY_1_PORTS_UTIL = "EXE_ACTIVITY:1_PORTS_UTIL";
    const std::string PAPITools::EXE_ACTIVITY_2_PORTS_UTIL = "EXE_ACTIVITY:2_PORTS_UTIL";
    const std::string PAPITools::ARITH_DIVIDER_ACTIVE = "ARITH:DIVIDER_ACTIVE";
    const std::string PAPITools::IDQ_ALL_MITE_CYCLES_ANY_UOPS = "IDQ:ALL_MITE_CYCLES_ANY_UOPS";
    const std::string PAPITools::IDQ_ALL_MITE_CYCLES_4_UOPS = "IDQ:ALL_MITE_CYCLES_4_UOPS";
    const std::string PAPITools::IDQ_ALL_DSB_CYCLES_ANY_UOPS = "IDQ:ALL_DSB_CYCLES_ANY_UOPS";
    const std::string PAPITools::IDQ_ALL_DSB_CYCLES_4_UOPS = "IDQ:ALL_DSB_CYCLES_4_UOPS";
    const std::string PAPITools::LSD_CYCLES_ACTIVE = "LSD:CYCLES_ACTIVE";
    const std::string PAPITools::LSD_CYCLES_4_UOPS = "LSD:CYCLES_4_UOPS";
    const std::string PAPITools::MEM_LOAD_RETIRED_L2_HIT = "MEM_LOAD_RETIRED:L2_HIT";
    const std::string PAPITools::MEM_LOAD_RETIRED_FB_HIT = "MEM_LOAD_RETIRED:FB_HIT";
    const std::string PAPITools::MEM_LOAD_RETIRED_L1_MISS = "MEM_LOAD_RETIRED:L1_MISS";
    const std::string PAPITools::L1D_PEND_MISS_FB_FULL_c1 = "L1D_PEND_MISS:FB_FULL:c=1";
    const std::string PAPITools::L1D_PEND_MISS_PENDING = "L1D_PEND_MISS:PENDING";
    const std::string PAPITools::MEM_LOAD_L3_MISS_RETIRED_REMOTE_DRAM = "MEM_LOAD_L3_MISS_RETIRED:REMOTE_DRAM";
    const std::string PAPITools::MEM_LOAD_L3_MISS_RETIRED_LOCAL_DRAM = "MEM_LOAD_L3_MISS_RETIRED:LOCAL_DRAM";
    const std::string PAPITools::MEM_LOAD_L3_MISS_RETIRED_REMOTE_FWD = "MEM_LOAD_L3_MISS_RETIRED:REMOTE_FWD";
    const std::string PAPITools::MEM_LOAD_L3_MISS_RETIRED_REMOTE_HITM = "MEM_LOAD_L3_MISS_RETIRED:REMOTE_HITM";
    const std::string PAPITools::MEM_LOAD_RETIRED_LOCAL_PMM = "MEM_LOAD_RETIRED:LOCAL_PMM";
    const std::string PAPITools::MEM_LOAD_L3_MISS_RETIRED_REMOTE_PMM = "MEM_LOAD_L3_MISS_RETIRED:REMOTE_PMM";
    const std::string PAPITools::DTLB_LOAD_MISSES_STLB_HIT_c1 = "DTLB_LOAD_MISSES:STLB_HIT:c=1";
    const std::string PAPITools::DTLB_LOAD_MISSES_WALK_ACTIVE = "DTLB_LOAD_MISSES:WALK_ACTIVE";
    const std::string PAPITools::LD_BLOCKS_STORE_FORWARD = "LD_BLOCKS:STORE_FORWARD";
    const std::string PAPITools::LD_BLOCKS_NO_SR = "LD_BLOCKS:NO_SR";
    const std::string PAPITools::LD_BLOCKS_PARTIAL_ADDRESS_ALIAS = "LD_BLOCKS_PARTIAL:ADDRESS_ALIAS";
    const std::string PAPITools::MEM_INST_RETIRED_LOCK_LOADS = "MEM_INST_RETIRED:LOCK_LOADS";
    const std::string PAPITools::MEM_INST_RETIRED_ALL_STORES = "MEM_INST_RETIRED:ALL_STORES";
    const std::string PAPITools::L2_RQSTS_ALL_RFO = "L2_RQSTS:ALL_RFO";
    const std::string PAPITools::L2_RQSTS_RFO_HIT = "L2_RQSTS:RFO_HIT";
    const std::string PAPITools::OFFCORE_REQUESTS_OUTSTANDING_CYCLES_WITH_DEMAND_RFO = "OFFCORE_REQUESTS_OUTSTANDING:CYCLES_WITH_DEMAND_RFO";



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
        if (PAPI_add_event(eventSet, native) != PAPI_OK) {
            std::cout << "Error while adding " << event_name << std::endl;
            exit(-1);
        }
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
                StartCountingTMALevel3(filename, line, tma_cata);
                break;
            case LEVEL4:
                StartCountingTMALevel4(filename, line, tma_cata);
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
                StopCountingTMALevel3();
                break;
            case LEVEL4:
                StopCountingTMALevel4();
                break;
            default:
                break;
        }
    }

    /* Use this function to stop counting in TMA level1 */
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
            case TOTAL_CLK:
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
            WriteFile("output.txt", {fe_bound});
        } else if(tma_level1 == BACKEND_BOUND){
            float a = after_value[0] - pre_value[0];  // UOPS_ISSUED_ANY_cnt
            float b = after_value[1] - pre_value[1];  // UOPS_RETIRED_RETIRE_SLOTS_cnt
            float c = after_value[2] - pre_value[2];  // INT_MISC_RECOVERY_CYCLES_cnt
            float d = after_value[3] - pre_value[3];  // CPU_CLK_UNHALTED_cnt
            float be_bound = (1 - ( a / ( 4 *  b ) ) - ( c + ( 4 * d ) ) / ( ( 4 * b ) ) );
            std::cout << "Backend bound: " << be_bound << std::endl;
            WriteFile("output.txt", {be_bound});
        } else if (tma_level1 == RETIRING) {
            float a = after_value[0] - pre_value[0];  // UOPS_RETIRED_RETIRE_SLOTS_cnt
            float b = after_value[1] - pre_value[1];  // CPU_CLK_UNHALTED_cnt
            float retiring = ( a / ( ( 4 * b ) ) );
            std::cout << "Retiring: " << retiring << std::endl;
            WriteFile("output.txt", {retiring});
        } else if (tma_level1 == BAD_SPEC) {
            float a = after_value[0] - pre_value[0];  // UOPS_ISSUED_ANY_cnt
            float b = after_value[1] - pre_value[1];  // UOPS_RETIRED_RETIRE_SLOTS_cnt
            float c = after_value[2] - pre_value[2];  // INT_MISC_RECOVERY_CYCLES_cnt
            float d = after_value[3] - pre_value[3];  // CPU_CLK_UNHALTED_cnt
            float bad_spec = ( a - ( b ) + ( 4 * c ) ) / ( 4 * d );
            std::cout << "Bad speculation: " << bad_spec << std::endl;
            WriteFile("output.txt", {bad_spec});
        } else if (tma_level1 == TOTAL_CLK) {
            long long a = after_value[0] - pre_value[0];  // CPU_CLK_UNHALTED_cnt
            std::cout << "Total clk: " << a << std::endl;
            WriteFile("output.txt", {a});
        }
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
                AddNativeEvent(PAPITools::EXE_ACTIVITY_BOUND_ON_STORES);
                AddNativeEvent(PAPITools::CYCLE_ACTIVITY_STALLS_TOTAL);     
                break;
            case CORE_BOUND_P3:
                AddNativeEvent(PAPITools::EXE_ACTIVITY_1_PORTS_UTIL);
                AddNativeEvent(PAPITools::UOPS_RETIRED_RETIRE_SLOTS); 
                AddNativeEvent(PAPITools::EXE_ACTIVITY_2_PORTS_UTIL);
                break;
            case MEMORY_BOUND_P1:
                AddNativeEvent(PAPITools::CYCLE_ACTIVITY_STALLS_MEM_ANY);
                AddNativeEvent(PAPITools::EXE_ACTIVITY_BOUND_ON_STORES);
                AddNativeEvent(PAPITools::CYCLE_ACTIVITY_STALLS_TOTAL);
                AddNativeEvent(PAPITools::EXE_ACTIVITY_1_PORTS_UTIL);
                break;
            case MEMORY_BOUND_P2:
                AddNativeEvent(PAPITools::UOPS_RETIRED_RETIRE_SLOTS);
                AddNativeEvent(PAPITools::CPU_CLK_UNHALTED);
                break;
            case MEMORY_BOUND_P3:
                AddNativeEvent(PAPITools::EXE_ACTIVITY_2_PORTS_UTIL);
                AddNativeEvent(PAPITools::IDQ_UOPS_NOT_DELIVERED_CORE);
                AddNativeEvent(PAPITools::UOPS_ISSUED_ANY);
                AddNativeEvent(PAPITools::INT_MISC_RECOVERY_CYCLES);
                break;
            default:
                break;
        }
        allow_adding = false;
        StartCounting(filename, line);
    }

    /* Use this function to stop counting in TMA level2 */
    void PAPITools::StopCountingTMALevel2() {
        if (PAPI_stop(eventSet, after_value) != PAPI_OK) exit(-1);
        /* Check cascadelakex_metrics.json to find details about calculation */
        if (tma_level2 == FETCH_LATENCY) {
            float a = after_value[0] - pre_value[0];  // IDQ_UOPS_NOT_DELIVERED_CYCLES_0_UOPS_DELIV_CORE_cnt
            float b = after_value[1] - pre_value[1];  // CPU_CLK_UNHALTED_cnt
            float fetch_latency = ( a / b );
            std::cout << "Fetch latency: " << fetch_latency << std::endl;
        } else if (tma_level2 == FETCH_BANDWIDTH) {
            float a = after_value[0] - pre_value[0];  // IDQ_UOPS_NOT_DELIVERED_CORE_cnt
            float b = after_value[1] - pre_value[1];  // CPU_CLK_UNHALTED_cnt
            float c = after_value[2] - pre_value[2];  // IDQ_UOPS_NOT_DELIVERED_CYCLES_0_UOPS_DELIV_CORE_cnt
            float fetch_bandwidth = ( ( a / ( 4 * b ) ) - ( c / b ) );
            std::cout << "Fetch bandwidth: " << fetch_bandwidth << std::endl;
        } else if (tma_level2 == CORE_BOUND_P1) {
            long long a = after_value[0] - pre_value[0], b = after_value[1] - pre_value[1];
            long long c = after_value[2] - pre_value[2], d = after_value[3] - pre_value[3];
            std::cout << "IDQ_UOPS_NOT_DELIVERED_CORE_cnt: " << a << std::endl;
            std::cout << "CPU_CLK_UNHALTED_cnt: " << b << std::endl;
            std::cout << "UOPS_ISSUED_ANY_cnt: " << c << std::endl;
            std::cout << "INT_MISC_RECOVERY_CYCLES_cnt: " << d << std::endl;
            WriteFile("output.txt", {a, b, c, d});
        } else if (tma_level2 == CORE_BOUND_P2) {
            long long a = after_value[0] - pre_value[0], b = after_value[1] - pre_value[1];
            long long c = after_value[2] - pre_value[2];
            std::cout << "CYCLE_ACTIVITY_STALLS_MEM_ANY_cnt:" << a << std::endl;
            std::cout << "EXE_ACTIVITY_BOUND_ON_STORES_cnt:" << b << std::endl;
            std::cout << "CYCLE_ACTIVITY_STALLS_TOTAL_cnt:" << c << std::endl;
            WriteFile("output.txt", {a, b, c});
        } else if (tma_level2 == CORE_BOUND_P3) {
            long long a = after_value[0] - pre_value[0], b = after_value[1] - pre_value[1];
            long long c = after_value[2] - pre_value[2];
            std::cout << "EXE_ACTIVITY_1_PORTS_UTIL_cnt:" << a << std::endl;
            std::cout << "UOPS_RETIRED_RETIRE_SLOTS_cnt:" << b << std::endl;
            std::cout << "EXE_ACTIVITY_2_PORTS_UTIL_cnt:" << c << std::endl;
            WriteFile("output.txt", {a, b, c});
        } else if (tma_level2 == MEMORY_BOUND_P1) {
            long long a = after_value[0] - pre_value[0], b = after_value[1] - pre_value[1];
            long long c = after_value[2] - pre_value[2], d = after_value[3] - pre_value[3];
            std::cout << "CYCLE_ACTIVITY_STALLS_MEM_ANY_cnt: " << a << std::endl;
            std::cout << "EXE_ACTIVITY_BOUND_ON_STORES_cnt: " << b << std::endl;
            std::cout << "CYCLE_ACTIVITY_STALLS_TOTAL_cnt: " << c << std::endl;
            std::cout << "INT_MISC_RECOVERY_CYCLES_cnt: " << d << std::endl;
            WriteFile("output.txt", {a, b, c, d});
        } else if (tma_level2 == MEMORY_BOUND_P2) {
            long long a = after_value[0] - pre_value[0], b = after_value[1] - pre_value[1];
            std::cout << "UOPS_RETIRED_RETIRE_SLOTS_cnt: " << a << std::endl;
            std::cout << "CPU_CLK_UNHALTED_cnt: " << b << std::endl;
            WriteFile("output.txt", {a, b});
        } else if (tma_level2 == MEMORY_BOUND_P3) {
            long long a = after_value[0] - pre_value[0], b = after_value[1] - pre_value[1];
            long long c = after_value[2] - pre_value[2], d = after_value[3] - pre_value[3];
            std::cout << "EXE_ACTIVITY_2_PORTS_UTIL_cnt: " << a << std::endl;
            std::cout << "IDQ_UOPS_NOT_DELIVERED_CORE_cnt: " << b << std::endl;
            std::cout << "UOPS_ISSUED_ANY_cnt: " << c << std::endl;
            std::cout << "INT_MISC_RECOVERY_CYCLES_cnt: " << d << std::endl;
            WriteFile("output.txt", {a, b, c, d});
        }
    }

    /* This function is used to count metrics in the thrid level of TMA */
    void PAPITools::StartCountingTMALevel3(const char* filename, int line, int tma_cata) {
        if(!allow_adding) {
            std::cout << "Opps, this tool is alreay occupied!" << std::endl;
            return;
        }
        tma_level3 = tma_cata;
        switch (tma_level3) {
            case DIVIDER: 
                AddNativeEvent(ARITH_DIVIDER_ACTIVE);
                AddNativeEvent(CPU_CLK_UNHALTED);
                break;
            case PORTS_UTILIZATION_P1:
                AddNativeEvent(CPU_CLK_UNHALTED);                                        
                AddNativeEvent(ARITH_DIVIDER_ACTIVE);
                AddNativeEvent(CYCLE_ACTIVITY_STALLS_TOTAL);
                AddNativeEvent(CYCLE_ACTIVITY_STALLS_MEM_ANY);
                break;
            case PORTS_UTILIZATION_P2:
                AddNativeEvent(EXE_ACTIVITY_EXE_BOUND_0_PORTS);
                AddNativeEvent(EXE_ACTIVITY_1_PORTS_UTIL);
                AddNativeEvent(EXE_ACTIVITY_2_PORTS_UTIL); 
                AddNativeEvent(UOPS_RETIRED_RETIRE_SLOTS);
                break;
            case L1_BOUND:
                AddNativeEvent(CYCLE_ACTIVITY_STALLS_MEM_ANY);
                AddNativeEvent(CYCLE_ACTIVITY_STALLS_L1D_MISS);
                AddNativeEvent(CPU_CLK_UNHALTED);
                break;
            case L2_BOUND_P1:
                AddNativeEvent(MEM_LOAD_RETIRED_L2_HIT);
                AddNativeEvent(MEM_LOAD_RETIRED_FB_HIT);
                AddNativeEvent(MEM_LOAD_RETIRED_L1_MISS);
                AddNativeEvent(L1D_PEND_MISS_FB_FULL_c1);
                break;
            case L2_BOUND_P2:
                AddNativeEvent(CYCLE_ACTIVITY_STALLS_L1D_MISS);
                AddNativeEvent(CYCLE_ACTIVITY_STALLS_L2_MISS);
                AddNativeEvent(CPU_CLK_UNHALTED);
                break;
            case L3_BOUND:
                AddNativeEvent(CYCLE_ACTIVITY_STALLS_L2_MISS);
                AddNativeEvent(CYCLE_ACTIVITY_STALLS_L3_MISS);
                AddNativeEvent(CPU_CLK_UNHALTED);
                break;
            case DRAM_BOUND_P1:
                AddNativeEvent(CYCLE_ACTIVITY_STALLS_L3_MISS);
                AddNativeEvent(CPU_CLK_UNHALTED);
                AddNativeEvent(CYCLE_ACTIVITY_STALLS_L1D_MISS);
                AddNativeEvent(CYCLE_ACTIVITY_STALLS_L2_MISS);
                break;
            case DRAM_BOUND_P2:
                AddNativeEvent(MEM_LOAD_L3_MISS_RETIRED_REMOTE_DRAM);
                AddNativeEvent(MEM_LOAD_L3_MISS_RETIRED_LOCAL_DRAM);
                AddNativeEvent(MEM_LOAD_L3_MISS_RETIRED_REMOTE_FWD);
                AddNativeEvent(MEM_LOAD_L3_MISS_RETIRED_REMOTE_HITM);
                break;
            case DRAM_BOUND_P3:
                AddNativeEvent(MEM_LOAD_RETIRED_LOCAL_PMM);
                AddNativeEvent(MEM_LOAD_L3_MISS_RETIRED_REMOTE_PMM);
                break;
            case STORE_BOUND:
                AddNativeEvent(EXE_ACTIVITY_BOUND_ON_STORES);
                AddNativeEvent(CPU_CLK_UNHALTED);
                break;
            case MITE:
                AddNativeEvent(IDQ_ALL_MITE_CYCLES_ANY_UOPS);
                AddNativeEvent(IDQ_ALL_MITE_CYCLES_4_UOPS);
                AddNativeEvent(CPU_CLK_UNHALTED);
                break;
            case DSB:
                AddNativeEvent(IDQ_ALL_DSB_CYCLES_ANY_UOPS);
                AddNativeEvent(IDQ_ALL_DSB_CYCLES_4_UOPS);
                AddNativeEvent(CPU_CLK_UNHALTED);
                break;
            case LSD:
                AddNativeEvent(LSD_CYCLES_ACTIVE);
                AddNativeEvent(LSD_CYCLES_4_UOPS);
                AddNativeEvent(CPU_CLK_UNHALTED);
                break;
            default:
                break;
        }
        allow_adding = false;
        StartCounting(filename, line);
    }

    /* Use this function to stop counting in TMA level3 */
    void PAPITools::StopCountingTMALevel3() {  
        if (PAPI_stop(eventSet, after_value) != PAPI_OK) exit(-1);
        /* Check cascadelakex_metrics.json to find details about calculation */ 
        if (tma_level3 == DIVIDER) {
            float a = after_value[0] - pre_value[0];    // ARITH_DIVIDER_ACTIVE_cnt
            float b = after_value[1] - pre_value[1];    // CPU_CLK_UNHALTED_cnt
            float divider =  ( a / ( b ) );
            WriteFile("output.txt", {divider});
            std::cout << "Divider: " << divider << std::endl;
        } else if (tma_level3 == PORTS_UTILIZATION_P1) {
            long long a = after_value[0] - pre_value[0], b = after_value[1] - pre_value[1];
            long long c = after_value[2] - pre_value[2], d = after_value[3] - pre_value[3];
            std::cout << "CPU_CLK_UNHALTED_cnt: " << a << std::endl;
            std::cout << "ARITH_DIVIDER_ACTIVE_cnt: " << b << std::endl;
            std::cout << "CYCLE_ACTIVITY_STALLS_TOTAL_cnt: " << c << std::endl;
            std::cout << "CYCLE_ACTIVITY_STALLS_MEM_ANY_cnt: " << d << std::endl;
            WriteFile("output.txt", {a, b, c, d});
        } else if (tma_level3 == PORTS_UTILIZATION_P2) {
            long long a = after_value[0] - pre_value[0], b = after_value[1] - pre_value[1];
            long long c = after_value[2] - pre_value[2], d = after_value[3] - pre_value[3];
            std::cout << "EXE_ACTIVITY_EXE_BOUND_0_PORTS_cnt: " << a << std::endl;
            std::cout << "EXE_ACTIVITY_1_PORTS_UTIL_cnt: " << b << std::endl;
            std::cout << "EXE_ACTIVITY_2_PORTS_UTIL_cnt: " << c << std::endl;
            std::cout << "UOPS_RETIRED_RETIRE_SLOTS_cnt: " << d << std::endl;
            WriteFile("output.txt", {a, b, c, d});
        } else if (tma_level3 == L1_BOUND) {
            float a = after_value[0] - pre_value[0];    // CYCLE_ACTIVITY_STALLS_MEM_ANY_cnt
            float b = after_value[1] - pre_value[1];    // CYCLE_ACTIVITY_STALLS_L1D_MISS_cnt
            float c = after_value[2] - pre_value[2];    // CPU_CLK_UNHALTED_cnt
            float temp = (a - b) / c;
            float l1_bound = (temp > 0) ? temp : 0;
            std::cout << "L1 bound: " << l1_bound << std::endl;
            WriteFile("output.txt", {l1_bound});
        } else if (tma_level3 == L2_BOUND_P1) {
            long long a = after_value[0] - pre_value[0], b = after_value[1] - pre_value[1];
            long long c = after_value[2] - pre_value[2], d = after_value[3] - pre_value[3];
            std::cout << "MEM_LOAD_RETIRED_L2_HIT_cnt:" << a << std::endl;
            std::cout << "MEM_LOAD_RETIRED_FB_HIT_cnt:" << b << std::endl;
            std::cout << "MEM_LOAD_RETIRED_L1_MISS_cnt:" << c << std::endl;
            std::cout << "L1D_PEND_MISS_FB_FULL_c1:" << d << std::endl;
            WriteFile("output.txt", {a, b, c, d});
        } else if (tma_level3 == L2_BOUND_P2) {
            long long a = after_value[0] - pre_value[0], b = after_value[1] - pre_value[1];
            long long c = after_value[2] - pre_value[2]; 
            std::cout << "CYCLE_ACTIVITY_STALLS_L1D_MISS_cnt:" << a << std::endl;
            std::cout << "CYCLE_ACTIVITY_STALLS_L2_MISS_cnt:" << b << std::endl;
            std::cout << "CPU_CLK_UNHALTED_THREAD:" << c << std::endl;
            WriteFile("output.txt", {a, b, c});
        } else if (tma_level3 == L3_BOUND) {
            float a = after_value[0] - pre_value[0];    // CYCLE_ACTIVITY_STALLS_L2_MISS_cnt
            float b = after_value[1] - pre_value[1];    // CYCLE_ACTIVITY_STALLS_L3_MISS_cnt
            float c = after_value[2] - pre_value[2];    // CPU_CLK_UNHALTED_cnt
            float l3_bound = ( a - b ) / ( c );
            std::cout << "L3 bound:" << l3_bound << std::endl;
            WriteFile("output.txt", {l3_bound});
        } else if (tma_level3 == DRAM_BOUND_P1) {
            long long a = after_value[0] - pre_value[0], b = after_value[1] - pre_value[1];
            long long c = after_value[2] - pre_value[2], d = after_value[3] - pre_value[3]; 
            std::cout << "CYCLE_ACTIVITY_STALLS_L3_MISS_cnt:" << a << std::endl;
            std::cout << "CPU_CLK_UNHALTED_THREAD_cnt:" << b << std::endl;
            std::cout << "CYCLE_ACTIVITY_STALLS_L1D_MISS_cnt:" << c << std::endl;
            std::cout << "CYCLE_ACTIVITY_STALLS_L2_MISS_cnt:" << d << std::endl;
            WriteFile("output.txt", {a, b, c, d});
        } else if (tma_level3 == DRAM_BOUND_P2) {
            long long a = after_value[0] - pre_value[0], b = after_value[1] - pre_value[1];
            long long c = after_value[2] - pre_value[2], d = after_value[3] - pre_value[3]; 
            std::cout << "MEM_LOAD_L3_MISS_RETIRED_REMOTE_DRAM_cnt:" << a << std::endl;
            std::cout << "MEM_LOAD_L3_MISS_RETIRED_LOCAL_DRAM_cnt:" << b << std::endl;
            std::cout << "MEM_LOAD_L3_MISS_RETIRED_REMOTE_FWD_cnt:" << c << std::endl;
            std::cout << "MEM_LOAD_L3_MISS_RETIRED_REMOTE_HITM_cnt:" << d << std::endl;
            WriteFile("output.txt", {a, b, c, d});
        } else if (tma_level3 == DRAM_BOUND_P3) {
            long long a = after_value[0] - pre_value[0], b = after_value[1] - pre_value[1]; 
            std::cout << "MEM_LOAD_RETIRED.LOCAL_PMM_cnt:" << a << std::endl;
            std::cout << "MEM_LOAD_L3_MISS_RETIRED.REMOTE_PMM_cnt:" << b << std::endl;
            WriteFile("output.txt", {a, b});
        } else if (tma_level3 == STORE_BOUND) {
            float a = after_value[0] - pre_value[0];    // IDQ_ALL_MITE_CYCLES_ANY_UOPS_cnt
            float b = after_value[1] - pre_value[1];    // IDQ_ALL_MITE_CYCLES_4_UOPS_cnt
            float store_bound = a / b;
            std::cout << "Store bound:" << store_bound << std::endl;
        } else if (tma_level3 == MITE) {
            float a = after_value[0] - pre_value[0];    // IDQ_ALL_MITE_CYCLES_ANY_UOPS_cnt
            float b = after_value[1] - pre_value[1];    // IDQ_ALL_MITE_CYCLES_4_UOPS_cnt
            float c = after_value[2] - pre_value[2];    // CPU_CLK_UNHALTED_cnt
            float mite = (a - b) / c / 2;
            std::cout << "MITE:" << mite << std::endl;
        } else if (tma_level3 == DSB) {
            float a = after_value[0] - pre_value[0];    // IDQ_ALL_DSB_CYCLES_ANY_UOPS_cnt
            float b = after_value[1] - pre_value[1];    // IDQ_ALL_DSB_CYCLES_4_UOPS_cnt
            float c = after_value[2] - pre_value[2];    // CPU_CLK_UNHALTED_cnt
            float dsb = (a - b) / c / 2;
            std::cout << "DSB:" << dsb << std::endl;
        } else if (tma_level3 == LSD) {
            float a = after_value[0] - pre_value[0];    // LSD_CYCLES_ACTIVE_cnt
            float b = after_value[1] - pre_value[1];    // LSD_CYCLES_4_UOPS_cnt
            float c = after_value[2] - pre_value[2];    // CPU_CLK_UNHALTED_cnt
            float lsd = (a - b) / c / 2;
            std::cout << "LSD:" << lsd << std::endl;
        }
    }

    void PAPITools::StartCountingTMALevel4(const char* filename, int line, int tma_cata) {
        if(!allow_adding) {
            std::cout << "Opps, this tool is alreay occupied!" << std::endl;
            return;
        }
        tma_level4 = tma_cata;
        switch (tma_cata) {
            case DTLB_LOAD:
                AddNativeEvent(DTLB_LOAD_MISSES_STLB_HIT_c1);
                AddNativeEvent(DTLB_LOAD_MISSES_WALK_ACTIVE);
                AddNativeEvent(CYCLE_ACTIVITY_CYCLES_MEM_ANY);
                AddNativeEvent(CYCLE_ACTIVITY_STALLS_L1D_MISS);
                break;
            case STORE_FWD_BLK:
                AddNativeEvent(LD_BLOCKS_STORE_FORWARD);
                AddNativeEvent(CPU_CLK_UNHALTED);
                break;
            case LOCK_LATENCY_P1:
                AddNativeEvent(MEM_INST_RETIRED_LOCK_LOADS);
                AddNativeEvent(L2_RQSTS_ALL_RFO);
                AddNativeEvent(MEM_INST_RETIRED_ALL_STORES);
                AddNativeEvent(L2_RQSTS_RFO_HIT);
                break;
            case LOCK_LATENCY_P2:
                AddNativeEvent(OFFCORE_REQUESTS_OUTSTANDING_CYCLES_WITH_DEMAND_RFO);
                break;
            case SPLIT_LOADS:
                AddNativeEvent(L1D_PEND_MISS_PENDING);
                AddNativeEvent(MEM_LOAD_RETIRED_L1_MISS);
                AddNativeEvent(MEM_LOAD_RETIRED_FB_HIT);
                AddNativeEvent(LD_BLOCKS_NO_SR);
                break;
            case ALIASING_4K:
                AddNativeEvent(LD_BLOCKS_PARTIAL_ADDRESS_ALIAS);
                AddNativeEvent(CPU_CLK_UNHALTED);
                break;
            case FB_FULL:
                AddNativeEvent(L1D_PEND_MISS_PENDING);
                AddNativeEvent(MEM_LOAD_RETIRED_L1_MISS);
                AddNativeEvent(MEM_LOAD_RETIRED_FB_HIT);
                AddNativeEvent(L1D_PEND_MISS_FB_FULL_c1);
                break;
            default:
                break;
        }
        allow_adding = false;
        StartCounting(filename, line);
    }

    /* Use this function to stop counting in TMA level4 */
    void PAPITools::StopCountingTMALevel4() {
     if (PAPI_stop(eventSet, after_value) != PAPI_OK) exit(-1);
        /* Check cascadelakex_metrics.json to find details about calculation */ 
        if (tma_level4 == DTLB_LOAD) {
            long long a = after_value[0] - pre_value[0];    // DTLB_LOAD_MISSES_STLB_HIT_c1_cnt
            long long b = after_value[1] - pre_value[1];    // DTLB_LOAD_MISSES_WALK_ACTIVE_cnt
            long long c = after_value[2] - pre_value[2];    // CYCLE_ACTIVITY_CYCLES_MEM_ANY_cnt
            long long d = after_value[3] - pre_value[3];    // CYCLE_ACTIVITY_STALLS_L1D_MISS_cnt
            long long dtlb_load;
            if (c - d > 0) {
                dtlb_load = (((9 * a + b) < (c - d)) ? (9 * a + b) : (c - d));
            } else {
                dtlb_load = (9 * a + b);
            }
            std::cout << "DTLB load: " << dtlb_load << std::endl;
            WriteFile("output.txt", {dtlb_load});
        } else if (tma_level4 == STORE_FWD_BLK) {
            float a = after_value[0] - pre_value[0];    // DTLB_LOAD_MISSES_STLB_HIT_c1_cnt
            float b = after_value[1] - pre_value[1];    // DTLB_LOAD_MISSES_WALK_ACTIVE_cnt
            float store_fwd_blk = ((13 * a / b) < 1) ? (13 * a / b) : 1;
            std::cout << "Store fwd blk: " << store_fwd_blk << std::endl;
            WriteFile("output.txt", {store_fwd_blk});
        } else if (tma_level4 == LOCK_LATENCY_P1) {
            long long a = after_value[0] - pre_value[0], b = after_value[1] - pre_value[1];
            long long c = after_value[2] - pre_value[2], d = after_value[3] - pre_value[3];
            std::cout << "MEM_INST_RETIRED_LOCK_LOADS_cnt: " << a << std::endl;
            std::cout << "L2_RQSTS_ALL_RFO_cnt: " << b << std::endl;
            std::cout << "MEM_INST_RETIRED_ALL_STORES_cnt: " << c << std::endl;
            std::cout << "L2_RQSTS_RFO_HIT_cnt: " << d << std::endl;
            WriteFile("output.txt", {a, b, c, d});
        } else if (tma_level4 == LOCK_LATENCY_P2) {
            long long a = after_value[0] - pre_value[0];
            std::cout << "OFFCORE_REQUESTS_OUTSTANDING_CYCLES_WITH_DEMAND_RFO_cnt: " << a << std::endl;
            WriteFile("output.txt", {a});
        } else if (tma_level4 == SPLIT_LOADS) {
            long long a = after_value[0] - pre_value[0], b = after_value[1] - pre_value[1];
            long long c = after_value[2] - pre_value[2], d = after_value[3] - pre_value[3];
            std::cout << "L1D_PEND_MISS_PENDING_cnt: " << a << std::endl;
            std::cout << "MEM_LOAD_RETIRED_L1_MISS_cnt: " << b << std::endl;
            std::cout << "MEM_LOAD_RETIRED_FB_HIT_cnt: " << c << std::endl;
            std::cout << "LD_BLOCKS_NO_SR_cnt: " << d << std::endl;
            WriteFile("output.txt", {a, b, c, d});
        } else if (tma_level4 == ALIASING_4K) {
            float a = after_value[0] - pre_value[0];
            float b = after_value[1] - pre_value[1];
            float aliasing_4k = a / b ;
            std::cout << "ALIASING_4K_cnt:" << aliasing_4k << std::endl;
            WriteFile("output.txt", {aliasing_4k});
        } else if (tma_level4 == FB_FULL) {
            long long a = after_value[0] - pre_value[0], b = after_value[1] - pre_value[1];
            long long c = after_value[2] - pre_value[2], d = after_value[3] - pre_value[3];
            std::cout << "L1D_PEND_MISS_PENDING_cnt: " << a << std::endl;
            std::cout << "MEM_LOAD_RETIRED_L1_MISS_cnt: " << b << std::endl;
            std::cout << "MEM_LOAD_RETIRED_FB_HIT_cnt: " << c << std::endl;
            std::cout << "L1D_PEND_MISS_FB_FULL_c1: " << d << std::endl;
            WriteFile("output.txt", {a, b, c, d});
        }
    }

    template <typename type>
    void PAPITools::WriteFile(const std::string& filename, std::initializer_list<type> values) {
        std::cout << "Writing" << std::endl;
        std::ofstream outfile(filename, std::ios::app);
        if (outfile.is_open()) {
            for (const type& value : values) {
                outfile << value << " ";
            }
            outfile << "\n";
            outfile.close();
        } else {
            std::cout << "Cannot open the file " << filename << std::endl;
        }
    }
}