#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/part.sh"

ticat ${meta} : ${cover} \
    : join.new algo 7 \
    : join.new delta_grid 0.2,0.5,0.7 \
    : join.new num_samples 100,500,1000 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 7 \
    : join.new delta_grid 0.2,0.5,0.7 \
    : join.new num_samples 100,500,1000 \
    : join.run run.sesame

ticat ${meta} : ${sensor} \
    : join.new algo 7 \
    : join.new delta_grid 0.2,0.5,0.7 \
    : join.new num_samples 100,500,1000 \
    : join.run run.sesame

ticat ${meta} : ${eds} \
    : join.new algo 7 \
    : join.new delta_grid 0.2,0.5,0.7 \
    : join.new num_samples 100,500,1000 \
    : join.run run.sesame

ticat ${meta} : ${edso} \
    : join.new algo 7 \
    : join.new delta_grid 0.2,0.5,0.7 \
    : join.new num_samples 100,500,1000 \
    : join.run run.sesame
