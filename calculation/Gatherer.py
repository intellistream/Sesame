import subprocess
import json

papi_interval = 2000000
num_points = 2000000

class AlgoStructer:
    def __init__(self, algo_name, algo_number):
        self.algo_name = algo_name
        self.algo_number = algo_number
        self.algo_metrics = {}
        
    def PutMetric(self, metric_map):
        if metric_map["name"] not in self.algo_metrics:
            self.algo_metrics[metric_map["name"]] = []
        self.algo_metrics[metric_map["name"]].append(metric_map["value"])
        

def ToNumber(num):
    return round(float(num), 4)

def CalAverageWithoutOutlier(values):
    mean = sum(values) / len(values)
    std_deviation = (sum((x - mean) ** 2 for x in values) / len(values)) ** 0.5
    threshold = 1 * std_deviation
    # Remove outliers
    filtered_values = [x for x in values if abs(x - mean) <= threshold]
    filtered_mean = 0
    if len(filtered_values) is not 0:
        filtered_mean = sum(filtered_values) / len(filtered_values)
    # filtered_std_deviation = (sum((x - filtered_mean) ** 2 for x in filtered_values) / len(filtered_values)) ** 0.5
    return filtered_mean

def CalculateCoreBound(algo):
    file = open('output.txt', 'r+')
    file.truncate(0)
    t = []
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
        '-level=2', '-metric=3', '-papi_print=false', '-papi_interval=' + str(papi_interval),
        '-num_points=' + str(num_points)])
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
        '-level=2', '-metric=4', '-papi_print=false', '-papi_interval=' + str(papi_interval),
        '-num_points=' + str(num_points)])
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
        '-level=2', '-metric=5', '-papi_print=false', '-papi_interval=' + str(papi_interval),
        '-num_points=' + str(num_points)])
    for line in file:
        t.extend([float(s) for s in line.split() if s.replace('.', '', 1).isdigit()])
    res_core = ( ( 1 - ( t[0] / ( ( 4 ) * ( ( t[1] ) ) ) ) - ( t[2] + ( 4 ) * ( t[3] ) ) / ( ( 4 ) * ( ( t[1] ) ) ) ) - ( ( ( t[4] + t[5] ) / ( t[6] + ( t[7] + ( ( t[8] ) / ( ( 4 ) * ( ( t[1] ) ) ) ) * t[9] ) + t[5] ) ) * ( 1 - ( t[0] / ( ( 4 ) * ( ( t[1] ) ) ) ) - ( t[2] + ( 4 ) * ( t[3] ) ) / ( ( 4 ) * ( ( t[1] ) ) ) ) ) )
    algo.PutMetric({"name": "Core bound", "value": ToNumber(res_core)})
    
def CalculateMemoryBound(algo):
    file = open('output.txt', 'r+')
    file.truncate(0)
    t = []
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
        '-level=2', '-metric=6', '-papi_print=false', '-papi_interval=' + str(papi_interval),
        '-num_points=' + str(num_points)])
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
        '-level=2', '-metric=7', '-papi_print=false', '-papi_interval=' + str(papi_interval),
        '-num_points=' + str(num_points)])
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
        '-level=2', '-metric=8', '-papi_print=false', '-papi_interval=' + str(papi_interval),
        '-num_points=' + str(num_points)])
    for line in file:
        t.extend([float(s) for s in line.split() if s.replace('.', '', 1).isdigit()])
    res_mem = ( ( ( t[0] + t[1] ) / ( t[2] + ( t[3] + ( ( t[4] ) / ( ( 4 ) * ( ( t[5] ) ) ) ) * t[6] ) + t[1] ) ) * ( 1 - ( t[7] / ( ( 4 ) * ( ( t[5] ) ) ) ) - ( t[8] + ( 4 ) * ( t[9] ) ) / ( ( 4 ) * ( ( t[5] ) ) ) ) )
    algo.PutMetric({"name": "Memory bound", "value": ToNumber(res_mem)})
    
    
def CalculateFrontendBound(algo):
    frontend_bound = []
    file = open('output.txt', 'r+')
    file.truncate(0)
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
                     '-level=1', '-metric=1', '-papi_print=false', '-papi_interval=' + str(papi_interval),
                     '-num_points=' + str(num_points)])
    for line in file:
        frontend_bound.append(float(line))
    res = CalAverageWithoutOutlier(frontend_bound)
    algo.PutMetric({"name": "Frontend bound", "value": ToNumber(res)})
    # print(algo.algo_name, "Frontend bound: ", res)
    
    
def CalculateBackendBound(algo):
    fe_bound = algo.algo_metrics["Frontend bound"]
    be_bound = algo.algo_metrics["Bad speculation"]
    retiring = algo.algo_metrics["Retiring"]
    res = 1 - CalAverageWithoutOutlier(fe_bound) - CalAverageWithoutOutlier(be_bound) - CalAverageWithoutOutlier(retiring)
    algo.PutMetric({"name": "Backend bound", "value": ToNumber(res)})
    # print(algo.algo_name, "Backend bound: ", be_bound)
    

def CalculateRetiring(algo):
    retiring = []
    file = open('output.txt', 'r+')
    file.truncate(0)
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
                     '-level=1', '-metric=2', '-papi_print=false', '-papi_interval=' + str(papi_interval),
                     '-num_points=' + str(num_points)])
    for line in file:
        retiring.append(float(line))
    res = CalAverageWithoutOutlier(retiring)
    algo.PutMetric({"name": "Retiring", "value": ToNumber(res)})
    # print(algo.algo_name, "Retiring: ", res)
    
    
def CalculateBadSpec(algo):
    bad_spec = []
    file = open('output.txt', 'r+')
    file.truncate(0)
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
                     '-level=1', '-metric=3', '-papi_print=false', '-papi_interval=' + str(papi_interval),
                     '-num_points=' + str(num_points)])
    for line in file:
        bad_spec.append(float(line))
    res = CalAverageWithoutOutlier(bad_spec)
    algo.PutMetric({"name": "Bad speculation", "value": ToNumber(res)})
    # print(algo.algo_name, "Bad speculati on: ", res)
    

def CalculateFetchLatency(algo):
    fetch_latency = []
    file = open('output.txt', 'r+')
    file.truncate(0)
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
                     '-level=2', '-metric=1', '-papi_print=false', '-papi_interval=' + str(papi_interval),
                     '-num_points=' + str(num_points)])
    for line in file:
        fetch_latency.append(float(line))
    res = CalAverageWithoutOutlier(fetch_latency)
    algo.PutMetric({"name": "Fetch latency", "value": ToNumber(res)})


def CalculateFetchBandwidth(algo):
    fetch_bandwidth = []
    file = open('output.txt', 'r+')
    file.truncate(0)
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
                     '-level=2', '-metric=2', '-papi_print=false', '-papi_interval=' + str(papi_interval),
                     '-num_points=' + str(num_points)])
    for line in file:
        fetch_bandwidth.append(float(line))
    res = CalAverageWithoutOutlier(fetch_bandwidth)
    algo.PutMetric({"name": "Fetch bandwidth", "value": ToNumber(res)})
    
    
def CalculateITLBMisses(algo):
    itlb_misses = []
    file = open('output.txt', 'r+')
    file.truncate(0)
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
                     '-level=3', '-metric=16', '-papi_print=false', '-papi_interval=' + str(papi_interval),
                     '-num_points=' + str(num_points)])
    for line in file:
        itlb_misses.append(float(line))
    res = CalAverageWithoutOutlier(itlb_misses)
    algo.PutMetric({"name": "ITLB misses", "value": ToNumber(res)})
    
    
def CalculateICACHEMisses(algo):
    icache_misses = []
    file = open('output.txt', 'r+')
    file.truncate(0)
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
                     '-level=3', '-metric=17', '-papi_print=false', '-papi_interval=' + str(papi_interval),
                     '-num_points=' + str(num_points)])
    for line in file:
        icache_misses.append(float(line))
    res = CalAverageWithoutOutlier(icache_misses)
    algo.PutMetric({"name": "ICACHE misses", "value": ToNumber(res)})
    
    
def CalculateBranchResteers(algo):
    branch_resteers = []
    file = open('output.txt', 'r+')
    file.truncate(0)
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
                     '-level=3', '-metric=18', '-papi_print=false', '-papi_interval=' + str(papi_interval),
                     '-num_points=' + str(num_points)])
    for line in file:
        branch_resteers.append(float(line))
    res = CalAverageWithoutOutlier(branch_resteers)
    algo.PutMetric({"name": "Branch resteers", "value": ToNumber(res)})
    
    
def CalculateDSBSwitches(algo):
    dsb_switches = []
    file = open('output.txt', 'r+')
    file.truncate(0)
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
                     '-level=3', '-metric=19', '-papi_print=false', '-papi_interval=' + str(papi_interval),
                     '-num_points=' + str(num_points)])
    for line in file:
        dsb_switches.append(float(line))
    res = CalAverageWithoutOutlier(dsb_switches)
    algo.PutMetric({"name": "DSB switches", "value": ToNumber(res)})
    

def CalculateLCP(algo):
    lcp = []
    file = open('output.txt', 'r+')
    file.truncate(0)
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
                     '-level=3', '-metric=20', '-papi_print=false', '-papi_interval=' + str(papi_interval),
                     '-num_points=' + str(num_points)])
    for line in file:
        lcp.append(float(line))
    res = CalAverageWithoutOutlier(lcp)
    algo.PutMetric({"name": "LCP", "value": ToNumber(res)})
    
    
def CalculateMSSwitches(algo):
    ms_switches = []
    file = open('output.txt', 'r+')
    file.truncate(0)
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
                     '-level=3', '-metric=21', '-papi_print=false', '-papi_interval=' + str(papi_interval),
                     '-num_points=' + str(num_points)])
    for line in file:
        ms_switches.append(float(line))
    res = CalAverageWithoutOutlier(ms_switches)
    algo.PutMetric({"name": "MS switches", "value": ToNumber(res)})
    
    
def CalculateDivider(algo):
    divider = []
    file = open('output.txt', 'r+')
    file.truncate(0)
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
                     '-level=3', '-metric=1', '-papi_print=false', '-papi_interval=' + str(papi_interval),
                     '-num_points=' + str(num_points)])
    for line in file:
        divider.append(float(line))
    res = CalAverageWithoutOutlier(divider)
    algo.PutMetric({"name": "Divider", "value": ToNumber(res)})
    
    
def CalculatePortsUtil(algo):
    file = open('output.txt', 'r+')
    file.truncate(0)
    t = []
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
                     '-level=3', '-metric=2', '-papi_print=false', '-papi_interval=' + str(papi_interval),
                     '-num_points=' + str(num_points)])
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
                     '-level=3', '-metric=3', '-papi_print=false', '-papi_interval=' + str(papi_interval),
                     '-num_points=' + str(num_points)])
    for line in file:
        t.extend([float(s) for s in line.split() if s.replace('.', '', 1).isdigit()])
    res_core = ( t[0] + ( t[1] + ( ( t[2] ) / ( ( 4 ) * ( ( t[3] ) ) ) ) * t[4] ) ) / ( t[3] ) if ( t[5] < ( t[6] - t[7] ) ) else ( t[1] + ( ( t[2] ) / ( ( 4 ) * ( ( t[3] ) ) ) ) * t[4] ) / ( t[3] )
    algo.PutMetric({"name": "Ports utilizations", "value": ToNumber(res_core)})
    

def CalculatePortsUtil0(algo):
    ports_0 = []
    file = open('output.txt', 'r+')
    file.truncate(0)
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
                     '-level=4', '-metric=8', '-papi_print=false', '-papi_interval=' + str(papi_interval),
                     '-num_points=' + str(num_points)])
    for line in file:
        ports_0.append(float(line))
    res = CalAverageWithoutOutlier(ports_0)
    algo.PutMetric({"name": "Ports util 0", "value": ToNumber(res)})
    

def CalculatePortsUtil1(algo):
    ports_1 = []
    file = open('output.txt', 'r+')
    file.truncate(0)
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
                     '-level=4', '-metric=9', '-papi_print=false', '-papi_interval=' + str(papi_interval),
                     '-num_points=' + str(num_points)])
    for line in file:
        ports_1.append(float(line))
    res = CalAverageWithoutOutlier(ports_1)
    algo.PutMetric({"name": "Ports util 1", "value": ToNumber(res)})
    
    
def CalculatePortsUtil2(algo):
    ports_2 = []
    file = open('output.txt', 'r+')
    file.truncate(0)
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
                     '-level=4', '-metric=10', '-papi_print=false', '-papi_interval=' + str(papi_interval),
                     '-num_points=' + str(num_points)])
    for line in file:
        ports_2.append(float(line))
    res = CalAverageWithoutOutlier(ports_2)
    algo.PutMetric({"name": "Ports util 2", "value": ToNumber(res)})
    
    
def CalculatePortsUtil3m(algo):
    ports_3m = []
    file = open('output.txt', 'r+')
    file.truncate(0)
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
                     '-level=4', '-metric=11', '-papi_print=false', '-papi_interval=' + str(papi_interval),
                     '-num_points=' + str(num_points)])
    for line in file:
        ports_3m.append(float(line))
    res = CalAverageWithoutOutlier(ports_3m)
    algo.PutMetric({"name": "Ports util 3m", "value": ToNumber(res)})
    

def CreateAlgoStructs(algos):
    res = []
    for key in algos.keys():
        res.append(AlgoStructer(key, algos[key]))
    return res
    
    
def CalculateTMALevel1(n, algo):
    for i in range(n):
        CalculateFrontendBound(algo)
        CalculateRetiring(algo)
        CalculateBadSpec(algo)
    CalculateBackendBound(algo)
    

def CalculateFrontendLevel2(n, algo):
    for i in range(n):
        CalculateFetchLatency(algo)
        CalculateFetchBandwidth(algo)
        
        
def CalculateFetchLatencyLevel3(n, algo):
    for i in range(n):
        CalculateITLBMisses(algo)
        CalculateICACHEMisses(algo)
        CalculateBranchResteers(algo)
        CalculateDSBSwitches(algo)
        CalculateLCP(algo)
        CalculateMSSwitches(algo)
        
        
def PrintAlgoMetrics(algo):
    print("Algorithm name " + algo.algo_name)
    for key in algo.algo_metrics.keys():
        print(key + ":" + str(CalAverageWithoutOutlier(algo.algo_metrics[key])))
        
        
def DumpData(algos):
    res_dict = {}
    for item in algos:
        if item.algo_name not in res_dict:
            res_dict[item.algo_name] = {}
        for key in item.algo_metrics.keys():
            res = CalAverageWithoutOutlier(item.algo_metrics[key])
            if res != 0:
                res_dict[item.algo_name][key] = CalAverageWithoutOutlier(item.algo_metrics[key])
    with open("data_ports_breakdown.json", "w") as file:
        json.dump(res_dict, file, indent=4)

if __name__ == "__main__":
    algo_names = {
        # "StreamKMeansType": 1,
        "DenStreamType": 3,
        "DBStreamType": 4,
        "EDMStreamType": 5,
        "DStreamType": 6,
        # "SLKMeansType": 7 , 
        }
    algo_structs = CreateAlgoStructs(algo_names)
    test_turn = 1
    for algo in algo_structs:
        # CalculateTMALevel1(test_turn, algo)
        CalculatePortsUtil0(algo)
        CalculatePortsUtil1(algo)
        CalculatePortsUtil2(algo)
        CalculatePortsUtil3m(algo)
        PrintAlgoMetrics(algo)
    DumpData(algo_structs)