#!/bin/bash

export kdd99="join.new input_file /home/shaun/Sesame/build/benchmark/datasets/KDD99.txt : join.new num_points 494021 : join.new dim 41 : join.new num_clusters 23 : join.new run_cmm false"

ticat ${meta} : mark-time bench.begin : ${kdd99} \
    : join.new algo 0 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 600 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${kdd99} \
    : join.new algo 1 \
    : join.new seed 100 \
    : join.new coreset_size 23 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${kdd99} \
    : join.new algo 2 \
    : join.new num_last_arr 2 \
    : join.new time_window 350 \
    : join.new num_online_clusters 60 \
    : join.new radius 8 \
    : join.new buf_size 1500 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${kdd99} \
    : join.new algo 3 \
    : join.new buf_size 500 \
    : join.new min_points 30 \
    : join.new epsilon 35 \
    : join.new base 2 \
    : join.new lambda 0.25 \
    : join.new mu 6 \
    : join.new beta 0.2 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${kdd99} \
    : join.new algo 4 \
    : join.new lambda 0.01 \
    : join.new radius 100 \
    : join.new clean_interval 500 \
    : join.new min_weight 0.5 \
    : join.new alpha 0.4 \
    : join.new base 2 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${kdd99} \
    : join.new algo 5 \
    : join.new radius 100 \
    : join.new delta 1500 \
    : join.new beta 0.7 \
    : join.new buf_size 10000 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${kdd99} \
    : join.new algo 6 \
    : join.new lambda 0.998 \
    : join.new beta 0.001 \
    : join.new cm 1 \
    : join.new cl 0.7 \
    : join.new grid_width 0.001 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${kdd99} \
    : join.new algo 7 \
    : join.new delta_grid 0.2 \
    : join.new num_samples 100 \
    : join.new sliding 100 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${kdd99} \
    : join.new algo 21 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 600 \
    : join.new landmark 20000 \
    : join.new outlier_distance_threshold 1000 \
    : join.new outlier_cap 100 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${kdd99} \
    : join.new algo 22 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 600 \
    : join.new landmark 20000 \
    : join.new outlier_distance_threshold 1000 \
    : join.new outlier_cap 100 \
    : join.new min_points 10 \
    : join.new epsilon 25 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${kdd99} \
    : join.new algo 23 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 600 \
    : join.new landmark 20000 \
    : join.new outlier_distance_threshold 1000 \
    : join.new outlier_cap 100 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${kdd99} \
    : join.new algo 24 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 600 \
    : join.new sliding 10000 \
    : join.new outlier_distance_threshold 1000 \
    : join.new outlier_cap 100 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${kdd99} \
    : join.new algo 25 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 600 \
    : join.new outlier_distance_threshold 1000 \
    : join.new outlier_cap 100 \
    : join.new lambda 0.5 \
    : join.new alpha 1.1 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${kdd99} \
    : join.new algo 26 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 600 \
    : join.new landmark 20000 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${kdd99} \
    : join.new algo 27 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 600 \
    : join.new landmark 10000 \
    : join.new outlier_density_threshold 100 \
    : join.new neighbor_distance 500 \
    : join.new outlier_cap 500 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${kdd99} \
    : join.new algo 28 \
    : join.new distance_threshold 600 \
    : join.new landmark 10000 \
    : join.new outlier_distance_threshold 1000 \
    : join.new outlier_cap 100 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${kdd99} \
    : join.new algo 29 \
    : join.new beta 0.001 \
    : join.new cm 3 \
    : join.new cl 0.7 \
    : join.new landmark 10000 \
    : join.new outlier_distance_threshold 1000 \
    : join.new outlier_cap 1000 \
    : join.run run.sesame
