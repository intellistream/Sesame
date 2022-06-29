#!/bin/bash

export insects="join.new input_file /home/shaun/Sesame/benchmark/datasets/INSECTS.txt : join.new num_points 100000 : join.new dim 33 : join.new num_clusters 24"

# ticat ${meta} : mark-time bench_begin : ${insects} \
#     : join.new algo 0 \
#     : join.new max_in_nodes [10,40,10] \
#     : join.new max_leaf_nodes [10,40,10] \
#     : join.new distance_threshold 50,100,250,600 \
#     : join.run run.sesame

# ticat ${meta} : mark-time bench.begin : ${insects} \
#     : join.new algo 2 \
#     : join.new num_last_arr 2 \
#     : join.new time_window 200 \
#     : join.new num_online_clusters 30 \
#     : join.new radius 30,50,0.1 \
#     : join.new buf_size 1500 \
#     : join.run run.sesame

# ticat ${meta} : ${insects} \
#     : join.new algo 4 \
#     : join.new lambda 0.25,0.125 \
#     : join.new alpha 0.4 \
#     : join.new radius [0.04,0.2,0.02] \
#     : join.new clean_interval 500 \
#     : join.new min_weight 0.5 \
#     : join.new base 2 \
#     : join.run run.sesame

# ticat ${meta} : mark-time bench.begin : ${insects} \
#     : join.new algo 3 \
#     : join.new buf_size 500,1000,1500,2000 \
#     : join.new min_points 30 \
#     : join.new epsilon [0.04,0.2,0.02] \
#     : join.new base 2 \
#     : join.new lambda 0.25 \
#     : join.new mu 6 \
#     : join.new beta 0.2,0.4,0.6,0.8 \
#     : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 6 \
    : join.new lambda 0.998,0.999 \
    : join.new beta 0.001,0.3 \
    : join.new cm 1 \
    : join.new cl 0.7,0.8,0.9,0.999,1.001 \
    : join.new grid_width [0.04,0.2,0.02] \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 5 \
    : join.new radius [0.04,0.2,0.02] \
    : join.new beta [0.1,1,0.3] \
    : join.new buf_size [1000,10000,3000] \
    : join.new delta 1500 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.run run.sesame

# ticat ${meta} : mark-time bench.begin : ${insects} \
#     : join.new algo 7 \
#     : join.new num_samples 100,500,1000 \
#     : join.new sliding 100,1000,2000 \
#     : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 1 \
    : join.new seed 1,10,100 \
    : join.new coreset_size [1000,10000,1000] \
    : join.run run.sesame

# ticat ${meta} : mark-time bench_begin : ${insects} \
#     : join.new algo 21 \
#     : join.new max_in_nodes 400 \
#     : join.new max_leaf_nodes 100 \
#     : join.new distance_threshold 0.05,0.08,0.1 \
#     : join.new landmark 1000,10000,20000 \
#     : join.new outlier_distance_threshold 0.12,0.15,0.18,0.2 \
#     : join.new outlier_cap 100 \
#     : join.run run.sesame

# ticat ${meta} : mark-time bench_begin : ${insects} \
#     : join.new algo 24 \
#     : join.new max_in_nodes 400 \
#     : join.new max_leaf_nodes 100 \
#     : join.new distance_threshold 100 \
#     : join.new sliding 1000,10000,20000 \
#     : join.new outlier_distance_threshold 1000 \
#     : join.new outlier_cap 100,300,500 \
#     : join.run run.sesame

# ticat ${meta} : mark-time bench_begin : ${insects} \
#     : join.new algo 25 \
#     : join.new max_in_nodes 400 \
#     : join.new max_leaf_nodes 100 \
#     : join.new distance_threshold 100 \
#     : join.new outlier_distance_threshold 1000 \
#     : join.new outlier_cap 100,300,500 \
#     : join.new lambda 0.5,1.5,2 \
#     : join.new alpha 1.1,1.5,2 \
#     : join.run run.sesame

# ticat ${meta} : mark-time bench_begin : ${insects} \
#     : join.new algo 27 \
#     : join.new max_in_nodes 400 \
#     : join.new max_leaf_nodes 100 \
#     : join.new distance_threshold 100 \
#     : join.new landmark 1000,10000,20000 \
#     : join.new outlier_density_threshold 100,300,500 \
#     : join.new neighbor_distance 200,300,500 \
#     : join.new outlier_cap 100,300,500 \
#     : join.run run.sesame

# ticat ${meta} : mark-time bench_begin : ${insects} \
#     : join.new algo 28 \
#     : join.new distance_threshold 100 \
#     : join.new landmark 1000,10000,20000 \
#     : join.new outlier_distance_threshold 1000 \
#     : join.new outlier_cap 100,300,500 \
#     : join.run run.sesame

# ticat ${meta} : mark-time bench_begin : ${insects} \
#     : join.new algo 29 \
#     : join.new beta 0.001,0.3 \
#     : join.new cm 1.001,3 \
#     : join.new cl 0.7,0.8,0.9,0.999,0.1001 \
#     : join.new landmark 1000,10000,20000 \
#     : join.new outlier_distance_threshold 1000 \
#     : join.new outlier_cap 100,300,500 \
#     : join.run run.sesame
