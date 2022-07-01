#!/bin/bash

export sensor="join.new input_file /home/shaun/Sesame/build/benchmark/datasets/sensor.txt : join.new num_points 1000000 : join.new dim 5 : join.new num_clusters 55"

ticat ${meta} : mark-time bench.begin : ${sensor} \
    : join.new algo 0 \
    : join.new max_in_nodes 1000 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 50 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${sensor} \
    : join.new algo 1 \
    : join.new seed 100 \
    : join.new coreset_size 10000 \
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
    : join.new lambda 0.998 \
    : join.new radius 2 \
    : join.new clean_interval 1500 \
    : join.new min_weight 0.5 \
    : join.new alpha 0.3 \
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
    : join.new num_samples 100 \
    : join.run run.sesame
