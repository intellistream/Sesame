#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/all.sh"

ticat ${meta} : mark-time bench.begin : ${cover} \
    : join.new algo 0 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 600 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${kdd99} \
    : join.new algo 0 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 600 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${sensor} \
    : join.new algo 0 \
    : join.new max_in_nodes 1000 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 50 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${insects} \
    : join.new algo 0 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 600 \
    : join.run run.sesame

# ticat ${meta} : mark-time bench.begin : ${eds} \
#     : join.new algo 0 \
#     : join.new max_in_nodes 100 \
#     : join.new max_leaf_nodes 100 \
#     : join.new distance_threshold 100 \
#     : join.run run.sesame

# ticat ${meta} : mark-time bench.begin : ${edso} \
#     : join.new algo 0 \
#     : join.new max_in_nodes 100 \
#     : join.new max_leaf_nodes 100 \
#     : join.new distance_threshold 200 \
#     : join.run run.sesame
