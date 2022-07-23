#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/all.sh"

for i in $(seq 1 5)
do
    eds=`eval echo \$\{eds$i\}`
    ticat ${meta} : mark-time bench.begin : ${eds} \
        : join.new algo 1 \
        : join.new seed 10 \
        : join.new coreset_size 5000 \
        : join.run run.sesame
done

for i in $(seq 1 3)
do
    edso=`eval echo \$\{edso$i\}`
    ticat ${meta} : mark-time bench.begin : ${edso} \
        : join.new algo 1 \
        : join.new seed 10 \
        : join.new coreset_size 5000 \
        : join.run run.sesame
done
