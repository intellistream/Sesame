#!/bin/bash

export kdd99="join.new input_file $HOME/Sesame/build/benchmark/datasets/KDD99_F.txt : join.new num_points 50000,100000,150000,200000,250000,300000,350000,400000,450000,500000,550000,600000,650000,700000,750000,800000,850000,900000,950000,1000000,1050000,1100000,1150000,1200000,1250000,1300000,1350000,1400000,1450000,1500000,1550000,1600000,1650000,1700000,1750000,1800000,1850000,1900000,1950000,2000000,2050000,2100000,2150000,2200000,2250000,2300000,2350000,2400000,2450000,2500000,2550000,2600000,2650000,2700000,2750000,2800000,2850000,2900000,2950000,3000000,3050000,3100000,3150000,3200000,3250000,3300000,3350000,3400000,3450000,3500000,3550000,3600000,3650000,3700000,3750000,3800000,3850000,3900000,3950000,4000000,4050000,4100000,4150000,4200000,4250000,4300000,4350000,4400000,4450000,4500000,4550000,4600000,4650000,4700000,4750000,4800000,4850000,4898431 : join.new dim 41 : join.new num_clusters 23 : join.new run_cmm false"

export balance=0
export accuracy=1
export efficiency=2

export OMP_NUM_THREADS=16

meta="{bench.tag=step.kdd} $meta"

ticat ${meta} : ${kdd99} \
    : join.new algo 8 \
    : join.new obj $balance \
    : join.new landmark 200000 \
    : join.new queue_size_threshold 70000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame &

ticat ${meta} : ${kdd99} \
    : join.new algo 8 \
    : join.new obj $accuracy \
    : join.new landmark 10000 \
    : join.new queue_size_threshold 5000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
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

wait