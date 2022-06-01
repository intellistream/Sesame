#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/all.sh"

ticat ${meta} : mark-time bench.begin : ${cover} \
    : join.new algo 3 \
    : join.new buf_size 500 \
    : join.new min_points 30 \
    : join.new epsilon 35 \
    : join.new base 2 \
    : join.new lambda 0.25 \
    : join.new mu 6 \
    : join.new beta 0.2 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${kdd99} \
    : join.new algo 3 \
    : join.new buf_size 500 \
    : join.new min_points 30 \
    : join.new epsilon 35 \
    : join.new base 2 \
    : join.new lambda 0.25 \
    : join.new mu 6 \
    : join.new beta 0.2 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${sensor} \
    : join.new algo 3 \
    : join.new buf_size 500 \
    : join.new min_points 5 \
    : join.new epsilon 0.8 \
    : join.new base 2 \
    : join.new lambda 0.25 \
    : join.new mu 6 \
    : join.new beta 0.2 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${insects} \
    : join.new algo 3 \
    : join.new buf_size 500 \
    : join.new min_points 30 \
    : join.new epsilon 35 \
    : join.new base 2 \
    : join.new lambda 0.25 \
    : join.new mu 6 \
    : join.new beta 0.2 \
    : join.run run.sesame

# ticat ${meta} : mark-time bench.begin : ${eds} \
#     : join.new algo 3 \
#     : join.new buf_size 500 \
#     : join.new min_points 30 \
#     : join.new epsilon 20 \
#     : join.new base 2 \
#     : join.new lambda 0.25 \
#     : join.new mu 6 \
#     : join.new beta 0.2,0.4,0.6,0.8 \
#     : join.run run.sesame

# ticat ${meta} : mark-time bench.begin : ${edso} \
#     : join.new algo 3 \
#     : join.new buf_size 500 \
#     : join.new min_points 30 \
#     : join.new epsilon 20 \
#     : join.new base 2 \
#     : join.new lambda 0.25 \
#     : join.new mu 6 \
#     : join.new beta 0.2 \
#     : join.run run.sesame
