import subprocess

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
    threshold = 0.5 * std_deviation
    # Remove outliers
    filtered_values = [x for x in values if abs(x - mean) <= threshold]
    filtered_mean = 0
    if len(filtered_values) is not 0:
        filtered_mean = sum(filtered_values) / len(filtered_values)
    # filtered_std_deviation = (sum((x - filtered_mean) ** 2 for x in filtered_values) / len(filtered_values)) ** 0.5
    return filtered_mean

def CalculateCoreBound(n):
    core_bound = []
    for i in range(n):
        file = open('output.txt', 'r+')
        file.truncate(0)
        t = []
        subprocess.call(['../build/benchmark/benchmark', '-algo=7', '-level=2', '-metric=3'])
        subprocess.call(['../build/benchmark/benchmark', '-algo=7', '-level=2', '-metric=4'])
        subprocess.call(['../build/benchmark/benchmark', '-algo=7', '-level=2', '-metric=5'])
        for line in file:
            t.extend([float(s) for s in line.split() if s.replace('.', '', 1).isdigit()])
        res_core = ( ( 1 - ( t[0] / ( ( 4 ) * ( ( t[1] ) ) ) ) - ( t[2] + ( 4 ) * ( t[3] ) ) / ( ( 4 ) * ( ( t[1] ) ) ) ) - ( ( ( t[4] + t[5] ) / ( t[6] + ( t[7] + ( ( t[8] ) / ( ( 4 ) * ( ( t[1] ) ) ) ) * t[9] ) + t[5] ) ) * ( 1 - ( t[0] / ( ( 4 ) * ( ( t[1] ) ) ) ) - ( t[2] + ( 4 ) * ( t[3] ) ) / ( ( 4 ) * ( ( t[1] ) ) ) ) ) )
        core_bound.append(res_core)
    print("Core bound:", core_bound)
    
def CalculateMemoryBound(n):
    mem_bound = []
    for i in range(n):
        file = open('output.txt', 'r+')
        file.truncate(0)
        t = []
        subprocess.call(['../build/benchmark/benchmark', '-algo=7', '-level=2', '-metric=6'])
        subprocess.call(['../build/benchmark/benchmark', '-algo=7', '-level=2', '-metric=7'])
        subprocess.call(['../build/benchmark/benchmark', '-algo=7', '-level=2', '-metric=8'])
        for line in file:
            t.extend([float(s) for s in line.split() if s.replace('.', '', 1).isdigit()])
        res_mem = ( ( ( t[0] + t[1] ) / ( t[2] + ( t[3] + ( ( t[4] ) / ( ( 4 ) * ( ( t[5] ) ) ) ) * t[6] ) + t[1] ) ) * ( 1 - ( t[7] / ( ( 4 ) * ( ( t[5] ) ) ) ) - ( t[8] + ( 4 ) * ( t[9] ) ) / ( ( 4 ) * ( ( t[5] ) ) ) ) )
        mem_bound.append(res_mem)
    print("Mem bound:", mem_bound)
    
    
def CalculateFrontendBound(algo):
    frontend_bound = []
    file = open('output.txt', 'r+')
    file.truncate(0)
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
                     '-level=1', '-metric=1', '-papi_print=false', '-papi_interval=10000'])
    for line in file:
        frontend_bound.append(float(line))
    res = CalAverageWithoutOutlier(frontend_bound)
    algo.PutMetric({"name": "Frontend bound", "value": ToNumber(res)})
    # print(algo.algo_name, "Frontend bound: ", res)
    
    
def CalculateBackendBound(algo):
    fe_bound = algo.algo_metrics["Frontend bound"]
    be_bound = algo.algo_metrics["Bad speculation"]
    retiring = algo.algo_metrics["Retiring"]
    res = 1 - fe_bound - be_bound - retiring
    algo.PutMetric({"name": "Backend bound", "value": ToNumber(res)})
    # print(algo.algo_name, "Backend bound: ", be_bound)
    

def CalculateRetiring(algo):
    retiring = []
    file = open('output.txt', 'r+')
    file.truncate(0)
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
                     '-level=1', '-metric=2', '-papi_print=false', '-papi_interval=10000'])
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
                     '-level=1', '-metric=3', '-papi_print=false', '-papi_interval=10000'])
    for line in file:
        bad_spec.append(float(line))
    res = CalAverageWithoutOutlier(bad_spec)
    algo.PutMetric({"name": "Bad speculation", "value": ToNumber(res)})
    # print(algo.algo_name, "Bad speculation: ", res)
    

def CalculateFetchLatency(algo):
    fetch_latency = []
    file = open('output.txt', 'r+')
    file.truncate(0)
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
                     '-level=2', '-metric=1', '-papi_print=false', '-papi_interval=10000'])
    for line in file:
        fetch_latency.append(float(line))
    res = CalAverageWithoutOutlier(fetch_latency)
    algo.PutMetric({"name": "Fetch latency", "value": ToNumber(res)})


def CalculateFetchBandwidth(algo):
    fetch_bandwidth = []
    file = open('output.txt', 'r+')
    file.truncate(0)
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
                     '-level=2', '-metric=2', '-papi_print=false', '-papi_interval=10000'])
    for line in file:
        fetch_bandwidth.append(float(line))
    res = CalAverageWithoutOutlier(fetch_bandwidth)
    algo.PutMetric({"name": "Fetch bandwidth", "value": ToNumber(res)})
    
    
def CalculateITLBMisses(algo):
    itlb_misses = []
    file = open('output.txt', 'r+')
    file.truncate(0)
    subprocess.call(['../build/benchmark/benchmark', '-algo=' + str(algo.algo_number), 
                     '-level=3', '-metric=16', '-papi_print=false', '-papi_interval=10000'])
    for line in file:
        itlb_misses.append(float(line))
    res = CalAverageWithoutOutlier(itlb_misses)
    algo.PutMetric({"name": "ITLB misses", "value": ToNumber(res)})
    

def CreateAlgoStructs(algos):
    res = []
    for key in algos.keys():
        res.append(AlgoStructer(key, algos[key]))
    return res
    
    
def CalculateTMALevel1(n):
    for i in range(test_turn):
        CalculateFrontendBound(algo)
        CalculateRetiring(algo)
        CalculateBadSpec(algo)
    CalculateBackendBound(algo)


if __name__ == "__main__":
    algo_names = {
        # "Birch": 0,
        "StreamKMeansType": 1,
        "DenStreamType": 3,
        "DBStreamType": 4,
        "EDMStreamType": 5,
        "DStreamType": 6,
        "SLKMeansType":7 , 
        # "Benne": 8
        }
    algo_structs = CreateAlgoStructs(algo_names)
    test_turn = 10
    for algo in algo_structs:
        for turn in range(test_turn):
            # CalculateFetchLatency(algo)
            # CalculateFetchBandwidth(algo)
            CalculateITLBMisses(algo)
        print(algo.algo_name, algo.algo_metrics)