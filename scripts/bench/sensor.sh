#!/bin/bash

export sensor="join.new input_file $HOME/Sesame/build/benchmark/datasets/sensor.txt : join.new num_points 2219803 : join.new dim 5 : join.new num_clusters 55 : join.new run_cmm false : join.new arr_rate 0"

ticat ${meta} : mark-time bench.begin : ${sensor} \
    : join.new algo 0 \
    : join.new max_in_nodes 1000 \
    : join.new max_leaf_nodes 1000 \
    : join.new distance_threshold 50 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${sensor} \
    : join.new algo 1 \
    : join.new seed 100 \
    : join.new coreset_size 55 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${sensor} \
    : join.new algo 2 \
    : join.new num_last_arr 2 \
    : join.new time_window 1000 \
    : join.new num_online_clusters 200 \
    : join.new radius 15 \
    : join.new buf_size 500 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${sensor} \
    : join.new algo 3 \
    : join.new buf_size 500 \
    : join.new min_points 5 \
    : join.new epsilon 0.8 \
    : join.new base 2 \
    : join.new lambda 0.25 \
    : join.new mu 6 \
    : join.new beta 0.2 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${sensor} \
    : join.new algo 4 \
    : join.new lambda 0.01 \
    : join.new radius 5 \
    : join.new clean_interval 2000 \
    : join.new min_weight 0.05 \
    : join.new alpha 0.03 \
    : join.new base 2 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${sensor} \
    : join.new algo 5 \
    : join.new radius 1.6 \
    : join.new delta 300 \
    : join.new beta 0.7 \
    : join.new buf_size 10000 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${sensor} \
    : join.new algo 6 \
    : join.new lambda 0.998 \
    : join.new beta 0.001 \
    : join.new cm 4 \
    : join.new cl 2 \
    : join.new grid_width 0.15 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${sensor} \
    : join.new algo 7 \
    : join.new delta_grid 0.2 \
    : join.new num_samples 10 \
    : join.new sliding 100 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${sensor} \
    : join.new algo 21 \
    : join.new max_in_nodes 1000 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 50 \
    : join.new landmark 10000 \
    : join.new outlier_distance_threshold 20 \
    : join.new outlier_cap 500 \
    : join.new k 1399 \ 
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${sensor} \
    : join.new algo 22 \
    : join.new max_in_nodes 1000 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 50 \
    : join.new landmark 10000 \
    : join.new outlier_distance_threshold 20 \
    : join.new outlier_cap 500 \
    : join.new min_points 5 \
    : join.new epsilon 0.8 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${sensor} \
    : join.new algo 23 \
    : join.new max_in_nodes 1000 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 50 \
    : join.new landmark 10000 \
    : join.new outlier_distance_threshold 20 \
    : join.new outlier_cap 500 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${sensor} \
    : join.new algo 24 \
    : join.new max_in_nodes 1000 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 50 \
    : join.new sliding 1000 \
    : join.new outlier_distance_threshold 20 \
    : join.new outlier_cap 500 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${sensor} \
    : join.new algo 25 \
    : join.new max_in_nodes 1000 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 50 \
    : join.new outlier_distance_threshold 20 \
    : join.new outlier_cap 100 \
    : join.new lambda 1 \
    : join.new alpha 0.998 \
    : join.new buf_size 10000 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${sensor} \
    : join.new algo 26 \
    : join.new max_in_nodes 1000 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 50 \
    : join.new landmark 10000 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${sensor} \
    : join.new algo 27 \
    : join.new max_in_nodes 1000 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 50 \
    : join.new landmark 20000 \
    : join.new outlier_density_threshold 100 \
    : join.new neighbor_distance 300 \
    : join.new outlier_cap 500 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${sensor} \
    : join.new algo 28 \
    : join.new distance_threshold 100 \
    : join.new landmark 10000 \
    : join.new outlier_distance_threshold 20 \
    : join.new outlier_cap 100 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${sensor} \
    : join.new algo 29 \
    : join.new beta 0.001 \
    : join.new cm 4 \
    : join.new cl 2 \
    : join.new landmark 1000 \
    : join.new outlier_distance_threshold 100 \
    : join.new outlier_cap 100 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${sensor} \
    : join.new algo 30 \
    : join.new radius 100 \
    : join.new delta 300 \
    : join.new beta 0.7 \
    : join.new buf_size 500 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.new landmark 10000 \
    : join.run run.sesame

# radius: larger faster
# buf_size: larger slower