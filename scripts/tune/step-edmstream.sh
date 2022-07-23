#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/all.sh"

for i in $(seq 1 5)
do
    eds=`eval echo \$\{eds$i\}`
    ticat ${meta} : mark-time bench.begin : ${eds} \
        : join.new algo 5 \
        : join.new radius 30 \
        : join.new delta 300 \
        : join.new beta 1 \
        : join.new buf_size 4000 \
        : join.new alpha 0.998 \
        : join.new lambda 1 \
        : join.run run.sesame
done


for i in $(seq 1 3)
do
    edso=`eval echo \$\{edso$i\}`
    ticat ${meta} : mark-time bench.begin : ${edso} \
        : join.new algo 5 \
        : join.new radius 20 \
        : join.new delta 300 \
        : join.new beta 1 \
        : join.new buf_size 4000 \
        : join.new alpha 0.998 \
        : join.new lambda 1 \
        : join.run run.sesame
done