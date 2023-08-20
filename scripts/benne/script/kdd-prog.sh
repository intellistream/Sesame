#!/bin/bash

export kdd99="join.new input_file $HOME/Sesame/build/benchmark/datasets/KDD99_F.txt : join.new num_points 50000,250000,450000,650000,850000,1050000,1250000,1450000,1650000,1850000,2050000,2250000,2450000,2650000,2850000,3050000,3250000,3450000,3650000,3850000,4050000,4250000,4450000,4650000,4850000,4898431 : join.new dim 41 : join.new num_clusters 23 : join.new run_cmm false"

export balance=0
export accuracy=1
export efficiency=2
export acc_no_mig=3

export OMP_NUM_THREADS=8

meta="{bench.tag=step.kdd} $meta"

# ticat ${meta} : ${kdd99} \
#     : join.new algo 8 \
#     : join.new obj $balance \
#     : join.new landmark 200000 \
#     : join.new queue_size_threshold 70000 \
#     : join.new dim_threshold 30 \
#     : join.new variance_threshold 100.0 \
#     : join.new outliers_num_threshold 200 \
#     : join.new outliers_dist_threshold 50.0 \
#     : join.run run.sesame &

ticat ${meta} : ${kdd99} \
    : join.new algo 8 \
    : join.new obj $accuracy \
    : join.new landmark 50000 \
    : join.new distance_threshold 40 \
    : join.new queue_size_threshold 10000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.new outlier_cap 2 \
    : join.new clean_interval 100000 \
    : join.new k 10 \
    : join.run run.sesame &

ticat ${meta} : ${kdd99} \
    : join.new algo 8 \
    : join.new obj $acc_no_mig \
    : join.new landmark 50000 \
    : join.new distance_threshold 40 \
    : join.new queue_size_threshold 10000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.new outlier_cap 2 \
    : join.new clean_interval 100000 \
    : join.new k 10 \
    : join.run run.sesame &

ticat ${meta} : ${kdd99} \
    : join.new algo 8 \
    : join.new obj $efficiency \
    : join.new sliding 1000 \
    : join.new landmark 500000 \
    : join.new queue_size_threshold 3000000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame &

ticat ${meta} : ${kdd99} \
    : join.new algo 36 \
    : join.new sliding 1000 \
    : join.new landmark 500000 \
    : join.new queue_size_threshold 3000000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame &

wait