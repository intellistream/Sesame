#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/part.sh"

# ticat ${meta} : mark-time bench.begin : ${cover} \
#     : join.new algo 2 \
#     : join.new num_last_arr 2,5,8 \
#     : join.new time_window 200,275,350 \
#     : join.new num_online_clusters 80,110,140 \
#     : join.new radius 2,14,26 \
#     : join.new buf_size 1500 \
#     : join.run run.sesame

# ticat ${meta} : mark-time bench.begin : ${kdd99} \
#     : join.new algo 2 \
#     : join.new num_last_arr 2,9,16 \
#     : join.new time_window 200,275,350 \
#     : join.new num_online_clusters 60,100,140 \
#     : join.new radius 2,5,8 \
#     : join.new buf_size 1500 \
#     : join.run run.sesame

# ticat ${meta} : mark-time bench.begin : ${sensor} \
#     : join.new algo 2 \
#     : join.new num_last_arr 2,9,16 \
#     : join.new time_window 1000 \
#     : join.new num_online_clusters 100,150,200 \
#     : join.new radius 15 \
#     : join.new buf_size 500 \
#     : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${eds} \
    : join.new algo 2 \
    : join.new num_last_arr 2,9,16 \
    : join.new time_window 200,275,350 \
    : join.new num_online_clusters 1000,1500,2000 \
    : join.new radius 10,20,30 \
    : join.new buf_size 1500 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${edso} \
    : join.new algo 2 \
    : join.new num_last_arr 2,9,16 \
    : join.new time_window 200,275,350 \
    : join.new num_online_clusters 300,500,700 \
    : join.new radius 10,20,30 \
    : join.new buf_size 1500 \
    : join.run run.sesame
