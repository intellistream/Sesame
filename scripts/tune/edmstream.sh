#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/part.sh"

ticat ${meta} : ${cover} \
    : join.new algo 5 \
    : join.new radius [100,1600,500] \
    : join.new beta [0.1,1,0.3] \
    : join.new buf_size [1000,4000,1000] \
    : join.new delta 1500 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 5 \
    : join.new radius [100,500,100] \
    : join.new beta [0.1,1,0.3] \
    : join.new buf_size [1000,10000,3000] \
    : join.new delta 1500 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.run run.sesame

ticat ${meta} : ${sensor} \
    : join.new algo 5 \
    : join.new radius [0.2,3,0.7] \
    : join.new beta [0.1,1,0.3] \
    : join.new buf_size [1000,10000,3000] \
    : join.new delta 300 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.run run.sesame

ticat ${meta} : ${eds} \
    : join.new algo 5 \
    : join.new radius [10,30,10] \
    : join.new beta [0.1,1,0.3] \
    : join.new buf_size [1000,4000,1000] \
    : join.new delta 300 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.run run.sesame

ticat ${meta} : ${edso} \
    : join.new algo 5 \
    : join.new radius [10,10,30] \
    : join.new beta [0.1,1,0.3] \
    : join.new buf_size [1000,4000,1000] \
    : join.new delta 300 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.run run.sesame
