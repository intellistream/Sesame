#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/all.sh"

ticat ${meta} : ${cover} \
    : join.new algo 30 \
    : join.new radius 100 \
    : join.new delta 1500 \
    : join.new beta 1 \
    : join.new buf_size 10000 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.new landmark 10000 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 30 \
    : join.new radius 100 \
    : join.new delta 1500 \
    : join.new beta 0.7 \
    : join.new buf_size 10000 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.new landmark 10000 \
    : join.run run.sesame

ticat ${meta} : ${sensor} \
    : join.new algo 30 \
    : join.new radius 1.6 \
    : join.new delta 300 \
    : join.new beta 0.7 \
    : join.new buf_size 10000 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.new landmark 10000 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 30 \
    : join.new radius 0.04 \
    : join.new delta 1500 \
    : join.new beta 0.4 \
    : join.new buf_size 10000 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.new landmark 10000 \
    : join.run run.sesame

ticat ${meta} : ${eds} \
    : join.new algo 30 \
    : join.new radius 30 \
    : join.new delta 300 \
    : join.new beta 1 \
    : join.new buf_size 4000 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.run run.sesame

# ticat ${meta} : ${edso} \
#     : join.new algo 30 \
#     : join.new radius 20 \
#     : join.new delta 300 \
#     : join.new beta 1 \
#     : join.new buf_size 4000 \
#     : join.new alpha 0.998 \
#     : join.new lambda 1 \
#     : join.run run.sesame
