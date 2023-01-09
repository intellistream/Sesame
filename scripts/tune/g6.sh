#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/part.sh"

ticat ${meta} : ${cover} \
    : join.new algo 26 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 1600 \
    : join.new landmark 1000,10000,20000 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 26 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark 1000,10000,20000 \
    : join.run run.sesame

ticat ${meta} : ${sensor} \
    : join.new algo 26 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark 1000,10000,20000 \
    : join.run run.sesame

ticat ${meta} : ${eds} \
    : join.new algo 26 \
    : join.new max_in_nodes 100 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark 1000,10000,20000 \
    : join.run run.sesame

ticat ${meta} : ${edso} \
    : join.new algo 26 \
    : join.new max_in_nodes 100 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 20 \
    : join.new landmark 1000,10000,20000 \
    : join.run run.sesame
