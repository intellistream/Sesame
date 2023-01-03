#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/all.sh"

ticat ${meta} : ${cover} \
    : join.new algo 4 \
    : join.new lambda 0.125 \
    : join.new radius 300 \
    : join.new clean_interval 400 \
    : join.new min_weight 0.5 \
    : join.new alpha 0.2 \
    : join.new base 2 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 4 \
    : join.new lambda 0.25 \
    : join.new radius 200 \
    : join.new clean_interval 500 \
    : join.new min_weight 0.5 \
    : join.new alpha 0.4 \
    : join.new base 2 \
    : join.run run.sesame

ticat ${meta} : ${sensor} \
    : join.new algo 4 \
    : join.new lambda 0.01 \
    : join.new radius 5 \
    : join.new clean_interval 1500 \
    : join.new min_weight 0.1 \
    : join.new alpha 0.03 \
    : join.new base 2 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 4 \
    : join.new lambda 0.001 \
    : join.new radius 0.1 \
    : join.new clean_interval 1500 \
    : join.new min_weight 0.01 \
    : join.new alpha 0.001 \
    : join.new base 2 \
    : join.run run.sesame

# ticat ${meta} : ${eds} \
#     : join.new algo 4 \
#     : join.new lambda 0.998 \
#     : join.new radius 20 \
#     : join.new clean_interval 400 \
#     : join.new min_weight 0.5 \
#     : join.new alpha 0.2 \
#     : join.new base 2 \
#     : join.run run.sesame

# ticat ${meta} : ${edso} \
#     : join.new algo 4 \
#     : join.new lambda 0.998 \
#     : join.new radius 10 \
#     : join.new clean_interval 400 \
#     : join.new min_weight 0.5 \
#     : join.new alpha 0.2 \
#     : join.new base 2 \
#     : join.run run.sesame
