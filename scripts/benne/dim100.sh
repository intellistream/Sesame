#!/bin/bash

export dim100="join.new input_file $HOME/Sesame/build/benchmark/datasets/Dim-100.txt : join.new num_points 500000 : join.new dim 100 : join.new num_clusters 50 : join.new run_cmm false : join.new arr_rate 0"

export balance=0
export accuracy=1
export efficiency=2

export OMP_NUM_THREADS=8

ticat ${meta} : ${dim100} \
    : join.new algo 0 \
    : join.new max_in_nodes 800 \
    : join.new max_leaf_nodes 800 \
    : join.new distance_threshold 800 \
    : join.new landmark 30000 \
    : join.run run.sesame &

ticat ${meta} : ${dim100} \
    : join.new algo 1 \
    : join.new seed 1 \
    : join.new coreset_size 7 \
    : join.new k 70 \
    : join.run run.sesame &

ticat ${meta} : ${dim100} \
    : join.new algo 2 \
    : join.new num_last_arr 2 \
    : join.new time_window 350 \
    : join.new num_online_clusters 110 \
    : join.new radius 14 \
    : join.new buf_size 1500 \
    : join.run run.sesame &

ticat ${meta} : ${dim100} \
    : join.new algo 3 \
    : join.new buf_size 500 \
    : join.new min_points 30 \
    : join.new epsilon 35 \
    : join.new base 2 \
    : join.new lambda 0.25 \
    : join.new mu 6 \
    : join.new beta 0.2 \
    : join.run run.sesame &

ticat ${meta} : ${dim100} \
    : join.new algo 4 \
    : join.new lambda 0.001 \
    : join.new radius 4 \
    : join.new clean_interval 400 \
    : join.new min_weight 0.05 \
    : join.new alpha 0.5 \
    : join.new base 1 \
    : join.run run.sesame &

ticat ${meta} : ${dim100} \
    : join.new algo 5 \
    : join.new radius 250 \
    : join.new delta 1500 \
    : join.new beta 1 \
    : join.new buf_size 1000 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.run run.sesame &

ticat ${meta} : ${dim100} \
    : join.new algo 6 \
    : join.new lambda 0.5 \
    : join.new beta 1 \
    : join.new cm 1 \
    : join.new cl 0.8 \
    : join.new grid_width 1 \
    : join.run run.sesame &

ticat ${meta} : ${dim100} \
    : join.new algo 7 \
    : join.new delta_grid 1 \
    : join.new num_samples 100 \
    : join.new sliding 10 \
    : join.run run.sesame &

ticat ${meta} : ${dim100} \
    : join.new algo 8 \
    : join.new obj $balance \
    : join.new landmark 50000 \
    : join.new queue_size_threshold 100 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.new coreset_size 10 \
    : join.run run.sesame &

ticat ${meta} : ${dim100} \
    : join.new algo 8 \
    : join.new obj $accuracy \
    : join.new landmark 1000 \
    : join.new distance_threshold 50 \
    : join.new queue_size_threshold 1000 \
    : join.new dim_threshold 50 \
    : join.new variance_threshold 50.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame &

ticat ${meta} : ${dim100} \
    : join.new algo 8 \
    : join.new obj $efficiency \
    : join.new landmark 50000 \
    : join.new sliding 300 \
    : join.new queue_size_threshold 500000 \
    : join.new dim_threshold 50 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame &

wait