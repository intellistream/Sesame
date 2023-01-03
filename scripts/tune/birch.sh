#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/part.sh"

ticat ${meta} : mark-time bench_begin : ${cover} \
    : join.new algo 0 \
    : join.new max_in_nodes [10,40,10] \
    : join.new max_leaf_nodes [10,40,10] \
    : join.new distance_threshold 100,200,300 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${kdd99} \
    : join.new algo 0 \
    : join.new max_in_nodes [10,40,10] \
    : join.new max_leaf_nodes [10,40,10] \
    : join.new distance_threshold 50,100,250,600 \
    : join.run run.sesame

ticat ${meta} : mark-time bench_begin : ${sensor} \
    : join.new algo 0 \
    : join.new max_in_nodes [10,40,10] \
    : join.new max_leaf_nodes [10,40,10] \
    : join.new distance_threshold 10,20,30,40 \
    : join.run run.sesame

ticat ${meta} : ${eds} \
    : join.new algo 0 \
    : join.new max_in_nodes [10,40,10] \
    : join.new max_leaf_nodes [10,40,10] \
    : join.new distance_threshold [10,100,10] \
    : join.run run.sesame

ticat ${meta} : ${edso} \
    : join.new algo 0 \
    : join.new max_in_nodes [10,40,10] \
    : join.new max_leaf_nodes [10,40,10] \
    : join.new distance_threshold [10,100,10] \
    : join.run run.sesame
