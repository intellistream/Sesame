#!/bin/bash

export insects="join.new input_file $HOME/Sesame/build/benchmark/datasets/INSECTS.txt : join.new num_points 905145 : join.new dim 33 : join.new num_clusters 24 : join.new run_cmm false : join.new arr_rate 0"

ticat ${meta} : ${insects} \
    : join.new algo 0 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 5 \
    : join.new landmark 10000000 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 1 \
    : join.new seed 1 \
    : join.new coreset_size 24 \
    : join.new k 102 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 2 \
    : join.new num_last_arr 20 \
    : join.new time_window 350 \
    : join.new num_online_clusters 30 \
    : join.new radius 30 \
    : join.new buf_size 500 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 3 \
    : join.new buf_size 1000 \
    : join.new min_points 30 \
    : join.new epsilon 0.1 \
    : join.new base 2 \
    : join.new lambda 0.25 \
    : join.new mu 6 \
    : join.new beta 0.6 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 4 \
    : join.new lambda 0.001 \
    : join.new radius 0.1 \
    : join.new clean_interval 1500 \
    : join.new min_weight 0.01 \
    : join.new alpha 0.001 \
    : join.new base 2 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 5 \
    : join.new radius 0.4 \
    : join.new delta 1500 \
    : join.new beta 0.4 \
    : join.new buf_size 10000 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 6 \
    : join.new lambda 0.998 \
    : join.new beta 0.001 \
    : join.new cm 3 \
    : join.new cl 0.7 \
    : join.new grid_width 0.08 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 7 \
    : join.new delta_grid 1 \
    : join.new num_samples 100 \
    : join.new sliding 100 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 21 \
    : join.new max_in_nodes 4000 \
    : join.new max_leaf_nodes 1000 \
    : join.new distance_threshold 0.8 \
    : join.new landmark 8000 \
    : join.new outlier_distance_threshold 12 \
    : join.new outlier_cap 5 \
    : join.new k 10000 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 22 \
    : join.new max_in_nodes 4000 \
    : join.new max_leaf_nodes 1000 \
    : join.new distance_threshold 0.8 \
    : join.new landmark 8000 \
    : join.new outlier_distance_threshold 12 \
    : join.new outlier_cap 5 \
    : join.new min_points 10 \
    : join.new epsilon 20 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 23 \
    : join.new max_in_nodes 4000 \
    : join.new max_leaf_nodes 1000 \
    : join.new distance_threshold 0.8 \
    : join.new landmark 8000 \
    : join.new outlier_distance_threshold 12 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 24 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 10 \
    : join.new sliding 100 \
    : join.new outlier_distance_threshold 12 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 25 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 1 \
    : join.new outlier_distance_threshold 1.2 \
    : join.new outlier_cap 20 \
    : join.new lambda 0.5 \
    : join.new alpha 1.1 \
    : join.new buf_size 10000 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 26 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 5 \
    : join.new landmark 20000 \
    : join.new outlier_distance_threshold 0.12 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 27 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 0.7 \
    : join.new landmark 10000 \
    : join.new outlier_density_threshold 100 \
    : join.new neighbor_distance 300 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 28 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark 5000 \
    : join.new outlier_distance_threshold 200 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 29 \
    : join.new beta 0.001 \
    : join.new cm 3 \
    : join.new cl 0.7 \
    : join.new landmark 10000 \
    : join.new outlier_distance_threshold 0.12 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 30 \
    : join.new radius 5000 \
    : join.new delta 1500 \
    : join.new beta 0.1 \
    : join.new buf_size 10000 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.new landmark 40000 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 31 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 400 \
    : join.new distance_threshold 0.75 \
    : join.new landmark 20000 \
    : join.new outlier_distance_threshold 1.8 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 32 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 5 \
    : join.new landmark 20000 \
    : join.new outlier_distance_threshold 0.7 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 33 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 5 \
    : join.new landmark 20000 \
    : join.new outlier_distance_threshold 0.7 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 34 \
    : join.new delta_grid 1 \
    : join.new num_samples 200 \
    : join.new sliding 20 \
    : join.new k 2 \
    : join.new landmark 900000 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 35 \
    : join.new seed 1 \
    : join.new landmark 300000 \
    : join.new coreset_size 5 \
    : join.new outlier_cap 5 \
    : join.run run.sesame
