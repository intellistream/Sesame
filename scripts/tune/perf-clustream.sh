#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/all.sh"

ticat ${meta} : ${cover} \
    : join.new algo 2 \
    : join.new num_last_arr 2 \
    : join.new time_window 350 \
    : join.new num_online_clusters 110 \
    : join.new radius 14 \
    : join.new buf_size 1500 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 2 \
    : join.new num_last_arr 2 \
    : join.new time_window 350 \
    : join.new num_online_clusters 60 \
    : join.new radius 8 \
    : join.new buf_size 1500 \
    : join.run run.sesame

ticat ${meta} : ${sensor} \
    : join.new algo 2 \
    : join.new num_last_arr 2 \
    : join.new time_window 1000 \
    : join.new num_online_clusters 200 \
    : join.new radius 15 \
    : join.new buf_size 500 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 2 \
    : join.new num_last_arr 2 \
    : join.new time_window 350 \
    : join.new num_online_clusters 60 \
    : join.new radius 8 \
    : join.new buf_size 1500 \
    : join.run run.sesame

# ticat ${meta} : ${eds} \
#     : join.new algo 2 \
#     : join.new num_last_arr 2,9,16 \
#     : join.new time_window 200,275,350 \
#     : join.new num_online_clusters 80,110,140 \
#     : join.new radius 10,20,30 \
#     : join.new buf_size 1500 \
#     : join.run run.sesame

# ticat ${meta} : ${edso} \
#     : join.new algo 2 \
#     : join.new num_last_arr 2,9,16 \
#     : join.new time_window 200,275,350 \
#     : join.new num_online_clusters 80,110,140 \
#     : join.new radius 10,20,30 \
#     : join.new buf_size 1500 \
#     : join.run run.sesame
