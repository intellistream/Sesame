#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/all.sh"

ticat ${meta} : ${cover} \
    : join.new algo 7 \
    : join.new delta_grid 0.2 \
    : join.new num_samples 100 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 7 \
    : join.new delta_grid 0.2 \
    : join.new num_samples 100 \
    : join.run run.sesame

ticat ${meta} : ${sensor} \
    : join.new algo 7 \
    : join.new delta_grid 0.2 \
    : join.new num_samples 100 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 7 \
    : join.new delta_grid 0.2 \
    : join.new num_samples 100 \
    : join.run run.sesame

# ticat ${meta} : ${eds} \
#     : join.new algo 7 \
#     : join.new delta_grid 0.2 \
#     : join.new num_samples 100 \
#     : join.run run.sesame

# ticat ${meta} : ${edso} \
#     : join.new algo 7 \
#     : join.new delta_grid 0.2 \
#     : join.new num_samples 100 \
#     : join.run run.sesame
