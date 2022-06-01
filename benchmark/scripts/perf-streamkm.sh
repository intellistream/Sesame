#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/all.sh"

ticat ${meta} : mark-time bench.begin : ${cover} \
    : join.new algo 1 \
    : join.new seed 1 \
    : join.new coreset_size 5000 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${kdd99} \
    : join.new algo 1 \
    : join.new seed 100 \
    : join.new coreset_size 10000 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${sensor} \
    : join.new algo 1 \
    : join.new seed 100 \
    : join.new coreset_size 10000 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${insects} \
    : join.new algo 1 \
    : join.new seed 100 \
    : join.new coreset_size 10000 \
    : join.run run.sesame

# ticat ${meta} : mark-time bench.begin : ${eds} \
#     : join.new algo 1 \
#     : join.new seed 10 \
#     : join.new coreset_size 5000 \
#     : join.run run.sesame

# ticat ${meta} : mark-time bench.begin : ${edso} \
#     : join.new algo 1 \
#     : join.new seed 10 \
#     : join.new coreset_size 5000 \
#     : join.run run.sesame
