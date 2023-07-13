#!/bin/bash

export kdd99="join.new input_file $HOME/Sesame/build/benchmark/datasets/KDD99_F.txt : join.new num_points 4898431 : join.new dim 41 : join.new num_clusters 23 : join.new run_cmm false"

export balance=0
export accuracy=1
export efficiency=2

export OMP_NUM_THREADS=8

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
    : join.new queue_size_threshold 3000000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame &