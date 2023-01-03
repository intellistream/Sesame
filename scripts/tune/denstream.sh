#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/part.sh"

ticat ${meta} : ${cover} \
    : join.new algo 3 \
    : join.new buf_size 500,1000,1500,2000 \
    : join.new min_points 30 \
    : join.new epsilon 35 \
    : join.new base 2,4,6,8 \
    : join.new lambda 0.25 \
    : join.new mu 6 \
    : join.new beta 0.2,0.4,0.6,0.8 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 3 \
    : join.new buf_size 500,1000,1500,2000 \
    : join.new min_points 30 \
    : join.new epsilon 35 \
    : join.new base 2,4,6,8 \
    : join.new lambda 0.25 \
    : join.new mu 6 \
    : join.new beta 0.2,0.4,0.6,0.8 \
    : join.run run.sesame

ticat ${meta} : ${sensor} \
    : join.new algo 3 \
    : join.new buf_size 500,1000,1500,2000 \
    : join.new min_points 5 \
    : join.new epsilon 0.8 \
    : join.new base 2,4,6,8 \
    : join.new lambda 0.25 \
    : join.new mu 6 \
    : join.new beta 0.2,0.4,0.6,0.8 \
    : join.run run.sesame

ticat ${meta} : ${eds} \
    : join.new algo 3 \
    : join.new buf_size 500,1000,1500,2000 \
    : join.new min_points 30 \
    : join.new epsilon 20 \
    : join.new base 2,4,6,8 \
    : join.new lambda 0.25 \
    : join.new mu 6 \
    : join.new beta 0.2,0.4,0.6,0.8 \
    : join.run run.sesame

ticat ${meta} : ${edso} \
    : join.new algo 3 \
    : join.new buf_size 500,1000,1500,2000 \
    : join.new min_points 30 \
    : join.new epsilon 20 \
    : join.new base 2,4,6,8 \
    : join.new lambda 0.25 \
    : join.new mu 6 \
    : join.new beta 0.2,0.4,0.6,0.8 \
    : join.run run.sesame
