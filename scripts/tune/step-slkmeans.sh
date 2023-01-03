#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/all.sh"

for i in $(seq 1 5)
do
    eds=`eval echo \$\{eds$i\}`
    ticat ${meta} : ${eds} \
        : join.new algo 7 \
        : join.new delta_grid 0.2 \
        : join.new num_samples 100 \
        : join.run run.sesame
done

for i in $(seq 1 3)
do
    edso=`eval echo \$\{edso$i\}`
    ticat ${meta} : ${edso} \
        : join.new algo 7 \
        : join.new delta_grid 0.2 \
        : join.new num_samples 100 \
        : join.run run.sesame
done
