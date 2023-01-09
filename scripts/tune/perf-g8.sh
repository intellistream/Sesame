#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/all.sh"

ticat ${meta} : ${cover} \
    : join.new algo 28 \
    : join.new distance_threshold 600 \
    : join.new landmark 10000 \
    : join.new outlier_distance_threshold 1000 \
    : join.new outlier_cap 300 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 28 \
    : join.new distance_threshold 600 \
    : join.new landmark 10000 \
    : join.new outlier_distance_threshold 1000 \
    : join.new outlier_cap 100 \
    : join.run run.sesame

ticat ${meta} : ${sensor} \
    : join.new algo 28 \
    : join.new distance_threshold 100 \
    : join.new landmark 10000 \
    : join.new outlier_distance_threshold 20 \
    : join.new outlier_cap 100 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 28 \
    : join.new distance_threshold 600 \
    : join.new landmark 10000 \
    : join.new outlier_distance_threshold 1000 \
    : join.new outlier_cap 100 \
    : join.run run.sesame

# ticat ${meta} : ${eds} \
#     : join.new algo 28 \
#     : join.new distance_threshold 100 \
#     : join.new landmark 1000,10000,20000 \
#     : join.new outlier_distance_threshold 10,15 \
#     : join.new outlier_cap 100,300,500 \
#     : join.run run.sesame

# ticat ${meta} : ${edso} \
#     : join.new algo 28 \
#     : join.new distance_threshold 20 \
#     : join.new landmark 1000,10000,20000 \
#     : join.new outlier_distance_threshold 10,15 \
#     : join.new outlier_cap 100,300,500 \
#     : join.run run.sesame
