#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/all.sh"

ticat ${meta} : mark-time bench.begin : ${cover} \
    : join.new algo 7 \
    : join.new delta_grid 0.2 \
    : join.new num_samples 1000 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${kdd99} \
    : join.new algo 7 \
    : join.new delta_grid 0.2 \
    : join.new num_samples 1000 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${sensor} \
    : join.new algo 7 \
    : join.new delta_grid 0.2 \
    : join.new num_samples 1000 \
    : join.run run.sesame

# ticat ${meta} : mark-time bench.begin : ${eds} \
#     : join.new algo 7 \
#     : join.new delta_grid 0.2 \
#     : join.new num_samples 1000 \
#     : join.run run.sesame

# ticat ${meta} : mark-time bench.begin : ${edso} \
#     : join.new algo 7 \
#     : join.new delta_grid 0.2 \
#     : join.new num_samples 1000 \
#     : join.run run.sesame
