#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/part.sh"

# ticat ${meta} : mark-time bench_begin : ${cover} \
#     : join.new algo 22 \
#     : join.new max_in_nodes 400 \
#     : join.new max_leaf_nodes 100 \
#     : join.new distance_threshold 1600 \
#     : join.new landmark 1000,10000,20000 \
#     : join.new outlier_distance_threshold 1000 \
#     : join.new outlier_cap 100,300,500 \
#     : join.new min_points 10 \
#     : join.new epsilon 25 \
#     : join.run run.sesame

# ticat ${meta} : mark-time bench_begin : ${kdd99} \
#     : join.new algo 22 \
#     : join.new max_in_nodes 400 \
#     : join.new max_leaf_nodes 100 \
#     : join.new distance_threshold 100 \
#     : join.new landmark 1000,10000,20000 \
#     : join.new outlier_distance_threshold 1000 \
#     : join.new outlier_cap 100,300,500 \
#     : join.new min_points 10 \
#     : join.new epsilon 25 \
#     : join.run run.sesame

# ticat ${meta} : mark-time bench_begin : ${sensor} \
#     : join.new algo 22 \
#     : join.new max_in_nodes 400 \
#     : join.new max_leaf_nodes 100 \
#     : join.new distance_threshold 100 \
#     : join.new landmark 1000,10000,20000 \
#     : join.new outlier_distance_threshold 20 \
#     : join.new outlier_cap 100,300,500 \
#     : join.new min_points 5 \
#     : join.new epsilon 0.8 \
#     : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${eds} \
    : join.new algo 22 \
    : join.new max_in_nodes 100 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark 1000,10000,20000 \
    : join.new outlier_distance_threshold 10,15 \
    : join.new outlier_cap 100,300,500 \
    : join.new min_points 10 \
    : join.new epsilon 10 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${edso} \
    : join.new algo 22 \
    : join.new max_in_nodes 100 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 20 \
    : join.new landmark 1000,10000,20000 \
    : join.new outlier_distance_threshold 10,15 \
    : join.new outlier_cap 100,300,500 \
    : join.new min_points 10 \
    : join.new epsilon 10 \
    : join.run run.sesame
