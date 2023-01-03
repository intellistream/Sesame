#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/part.sh"

ticat ${meta} : mark-time bench_begin : ${cover} \
    : join.new algo 27 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 1600 \
    : join.new landmark 1000,10000,20000 \
    : join.new outlier_density_threshold 100,300,500 \
    : join.new neighbor_distance 2000,3000,5000 \
    : join.new outlier_cap 100,300,500 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${kdd99} \
    : join.new algo 27 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark 1000,10000,20000 \
    : join.new outlier_density_threshold 100,300,500 \
    : join.new neighbor_distance 200,300,500 \
    : join.new outlier_cap 100,300,500 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${sensor} \
    : join.new algo 27 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark 1000,10000,20000 \
    : join.new outlier_density_threshold 100,300,500 \
    : join.new neighbor_distance 200,300,500 \
    : join.new outlier_cap 100,300,500 \
    : join.run run.sesame

ticat ${meta} : ${eds} \
    : join.new algo 27 \
    : join.new max_in_nodes 100 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark 1000,10000,20000 \
    : join.new outlier_density_threshold 100,300,500 \
    : join.new neighbor_distance 200,300,500 \
    : join.new outlier_cap 100,300,500 \
    : join.run run.sesame

ticat ${meta} : ${edso} \
    : join.new algo 27 \
    : join.new max_in_nodes 100 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 20 \
    : join.new landmark 1000,10000,20000 \
    : join.new outlier_density_threshold 100,300,500 \
    : join.new neighbor_distance 30,50 \
    : join.new outlier_cap 100,300,500 \
    : join.run run.sesame
