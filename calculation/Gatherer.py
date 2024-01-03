import subprocess

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

if __name__ == "__main__":
    CalculateCoreBound(20)
    CalculateMemoryBound(20)