#!/bin/bash

export insects="join.new input_file /home/shaun/Sesame/build/benchmark/datasets/INSECTS.txt : join.new num_points 905145 : join.new dim 33 : join.new num_clusters 24 : join.new run_cmm false : join.new arr_rate 0"

ticat ${meta} : mark-time bench.begin : ${insects} \
    : join.new algo 0 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 0.05 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${insects} \
    : join.new algo 1 \
    : join.new seed 1 \
    : join.new coreset_size 24 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${insects} \
    : join.new algo 2 \
    : join.new num_last_arr 2 \
    : join.new time_window 350 \
    : join.new num_online_clusters 60 \
    : join.new radius 8 \
    : join.new buf_size 1500 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${insects} \
    : join.new algo 3 \
    : join.new buf_size 500 \
    : join.new min_points 30 \
    : join.new epsilon 35 \
    : join.new base 2 \
    : join.new lambda 0.25 \
    : join.new mu 6 \
    : join.new beta 0.2 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${insects} \
    : join.new algo 4 \
    : join.new lambda 0.001 \
    : join.new radius 0.1 \
    : join.new clean_interval 1500 \
    : join.new min_weight 0.01 \
    : join.new alpha 0.001 \
    : join.new base 2 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${insects} \
    : join.new algo 5 \
    : join.new radius 0.04 \
    : join.new delta 1500 \
    : join.new beta 0.4 \
    : join.new buf_size 10000 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${insects} \
    : join.new algo 6 \
    : join.new lambda 0.998 \
    : join.new beta 0.001 \
    : join.new cm 1 \
    : join.new cl 0.7 \
    : join.new grid_width 0.14 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${insects} \
    : join.new algo 7 \
    : join.new delta_grid 0.2 \
    : join.new num_samples 100 \
    : join.new sliding 100 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${insects} \
    : join.new algo 21,22,23 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 0.05 \
    : join.new landmark 1000 \
    : join.new outlier_distance_threshold 0.12 \
    : join.new outlier_cap 100 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${insects} \
    : join.new algo 24 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 0.05 \
    : join.new sliding 200000 \
    : join.new outlier_distance_threshold 0.12 \
    : join.new outlier_cap 100 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${insects} \
    : join.new algo 25 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 0.05 \
    : join.new outlier_distance_threshold 0.12 \
    : join.new outlier_cap 100 \
    : join.new lambda 0.5 \
    : join.new alpha 1.1 \
    : join.new buf_size 10000 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${insects} \
    : join.new algo 26 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 0.05 \
    : join.new landmark 1000 \
    : join.new outlier_distance_threshold 0.12 \
    : join.new outlier_cap 100 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${insects} \
    : join.new algo 27 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 0.05 \
    : join.new landmark 10000 \
    : join.new outlier_density_threshold 100 \
    : join.new neighbor_distance 300 \
    : join.new outlier_cap 100 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${insects} \
    : join.new algo 28 \
    : join.new distance_threshold 600 \
    : join.new landmark 10000 \
    : join.new outlier_distance_threshold 0.15 \
    : join.new outlier_cap 100 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${insects} \
    : join.new algo 29 \
    : join.new beta 0.001 \
    : join.new cm 3 \
    : join.new cl 0.7 \
    : join.new landmark 10000 \
    : join.new outlier_distance_threshold 0.12 \
    : join.new outlier_cap 100 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${insects} \
    : join.new algo 30 \
    : join.new radius 0.04 \
    : join.new delta 1500 \
    : join.new beta 0.4 \
    : join.new buf_size 10000 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.new landmark 10000 \
    : join.run run.sesame
