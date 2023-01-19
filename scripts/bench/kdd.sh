#!/bin/bash

export kdd99="join.new input_file $HOME/Sesame/build/benchmark/datasets/KDD99_F.txt : join.new num_points 4898431 : join.new dim 41 : join.new num_clusters 23 : join.new run_cmm false"

# ticat ${meta} : ${kdd99} \
#     : join.new algo 0 \
#     : join.new max_in_nodes 40000 \
#     : join.new max_leaf_nodes 40000 \
#     : join.new distance_threshold 60000 \
#     : join.new landmark 10000000 \
#     : join.run run.sesame

# ticat ${meta} : ${kdd99} \
#     : join.new algo 1 \
#     : join.new seed 100 \
#     : join.new coreset_size 23 \
#     : join.new k 454 \
#     : join.run run.sesame

# ticat ${meta} : ${kdd99} \
#     : join.new algo 2 \
#     : join.new num_last_arr 2 \
#     : join.new time_window 350 \
#     : join.new num_online_clusters 7 \
#     : join.new radius 50 \
#     : join.new buf_size 3000 \
#     : join.run run.sesame

# ticat ${meta} : ${kdd99} \
#     : join.new algo 3 \
#     : join.new buf_size 500 \
#     : join.new min_points 30 \
#     : join.new epsilon 35 \
#     : join.new base 2 \
#     : join.new lambda 0.25 \
#     : join.new mu 6 \
#     : join.new beta 0.2 \
#     : join.run run.sesame

# ticat ${meta} : ${kdd99} \
#     : join.new algo 4 \
#     : join.new lambda 0.01 \
#     : join.new radius 1000 \
#     : join.new clean_interval 5000 \
#     : join.new min_weight 0.5 \
#     : join.new alpha 0.4 \
#     : join.new base 2 \
#     : join.run run.sesame

# ticat ${meta} : ${kdd99} \
#     : join.new algo 5 \
#     : join.new radius 100 \
#     : join.new delta 1500 \
#     : join.new beta 0.7 \
#     : join.new buf_size 10000 \
#     : join.new alpha 0.998 \
#     : join.new lambda 1 \
#     : join.run run.sesame

# ticat ${meta} : ${kdd99} \
#     : join.new algo 6 \
#     : join.new lambda 0.998 \
#     : join.new beta 0.001 \
#     : join.new cm 1 \
#     : join.new cl 0.7 \
#     : join.new grid_width 50 \
#     : join.run run.sesame

# ticat ${meta} : ${kdd99} \
#     : join.new algo 7 \
#     : join.new delta_grid 100 \
#     : join.new num_samples 10 \
#     : join.new sliding 10 \
#     : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 21 \
    : join.new max_in_nodes 40000 \
    : join.new max_leaf_nodes 40000 \
    : join.new distance_threshold 60000 \
    : join.new landmark 20000 \
    : join.new outlier_distance_threshold 80000 \
    : join.new outlier_cap 5 \
    : join.new k 10000 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 22 \
    : join.new max_in_nodes 40000 \
    : join.new max_leaf_nodes 40000 \
    : join.new distance_threshold 60000 \
    : join.new landmark 20000 \
    : join.new outlier_distance_threshold 80000 \
    : join.new outlier_cap 5 \
    : join.new min_points 10 \
    : join.new epsilon 25 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 23 \
    : join.new max_in_nodes 70000 \
    : join.new max_leaf_nodes 70000 \
    : join.new distance_threshold 70000 \
    : join.new landmark 300000 \
    : join.new outlier_distance_threshold 80000 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 24 \
    : join.new max_in_nodes 40000 \
    : join.new max_leaf_nodes 40000 \
    : join.new distance_threshold 40000 \
    : join.new sliding 200 \
    : join.new outlier_distance_threshold 80000 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 25 \
    : join.new max_in_nodes 40000 \
    : join.new max_leaf_nodes 40000 \
    : join.new distance_threshold 60000 \
    : join.new outlier_distance_threshold 80000 \
    : join.new outlier_cap 5 \
    : join.new buf_size 100000 \
    : join.new lambda 0.5 \
    : join.new alpha 1.1 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 26 \
    : join.new max_in_nodes 80000 \
    : join.new max_leaf_nodes 80000 \
    : join.new distance_threshold 70000 \
    : join.new landmark 450000 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 27 \
    : join.new max_in_nodes 40000 \
    : join.new max_leaf_nodes 40000 \
    : join.new distance_threshold 60000 \
    : join.new landmark 10000 \
    : join.new outlier_density_threshold 100 \
    : join.new neighbor_distance 500 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 28 \
    : join.new distance_threshold 60000 \
    : join.new landmark 1000 \
    : join.new outlier_distance_threshold 80000 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 29 \
    : join.new beta 0.001 \
    : join.new cm 100 \
    : join.new cl 0.7 \
    : join.new landmark 1000 \
    : join.new outlier_distance_threshold 800 \
    : join.new outlier_cap 5 \
    : join.new grid_width 50 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 30 \
    : join.new radius 80000 \
    : join.new delta 5000 \
    : join.new beta 0.1 \
    : join.new buf_size 5000 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.new landmark 200000 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 31 \
    : join.new max_in_nodes 60000 \
    : join.new max_leaf_nodes 60000 \
    : join.new distance_threshold 60000 \
    : join.new landmark 300000 \
    : join.new outlier_distance_threshold 100000 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 32 \
    : join.new max_in_nodes 60000 \
    : join.new max_leaf_nodes 60000 \
    : join.new distance_threshold 60000 \
    : join.new landmark 400000 \
    : join.new outlier_distance_threshold 100000 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 33 \
    : join.new max_in_nodes 60000 \
    : join.new max_leaf_nodes 60000 \
    : join.new distance_threshold 60000 \
    : join.new landmark 200000 \
    : join.new outlier_distance_threshold 100000 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 34 \
    : join.new delta_grid 1 \
    : join.new num_samples 200 \
    : join.new sliding 5000 \
    : join.new landmark 1000000 \
    : join.new k 5 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 35 \
    : join.new seed 1 \
    : join.new landmark 500000 \
    : join.new coreset_size 5 \
    : join.new outlier_cap 5 \
    : join.run run.sesame
