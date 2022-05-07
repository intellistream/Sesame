#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/part.sh"

ticat ${meta} : mark-time bench_begin : ${cover} \
    : join.new algo 0 \
    : join.new max_in_nodes [100,1000,300] \
    : join.new max_leaf_nodes [100,1000,300] \
    : join.new distance_threshold [100,1600,500] \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${kdd99} \
    : join.new algo 0 \
    : join.new max_in_nodes [100,1000,300] \
    : join.new max_leaf_nodes [100,1000,300] \
    : join.new distance_threshold [100,1600,500] \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${sensor} \
    : join.new algo 0 \
    : join.new max_in_nodes [100,1000,300] \
    : join.new max_leaf_nodes [100,1000,300] \
    : join.new distance_threshold [10,100,30] \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${eds} \
    : join.new algo 0 \
    : join.new max_in_nodes [100,1000,300] \
    : join.new max_leaf_nodes [100,1000,300] \
    : join.new distance_threshold [20,100,20] \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${edso} \
    : join.new algo 0 \
    : join.new max_in_nodes [100,1000,300] \
    : join.new max_leaf_nodes [100,1000,300] \
    : join.new distance_threshold [20,100,20] \
    : join.run run.sesame
