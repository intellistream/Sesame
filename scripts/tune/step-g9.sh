#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/all.sh"

for i in $(seq 1 5)
do
    eds=`eval echo \$\{eds$i\}`
    ticat ${meta} : ${eds} \
        : join.new algo 29 \
        : join.new beta 0.001,0.3 \
        : join.new cm 4 \
        : join.new cl 2 \
        : join.new landmark 1000 \
        : join.new outlier_distance_threshold 10 \
        : join.new outlier_cap 100 \
        : join.run run.sesame
done

for i in $(seq 1 3)
do
    edso=`eval echo \$\{edso$i\}`
    ticat ${meta} : ${edso} \
        : join.new algo 29 \
        : join.new beta 0.001,0.3 \
        : join.new cm 4\
        : join.new cl 2 \
        : join.new landmark 1000 \
        : join.new outlier_distance_threshold 10 \
        : join.new outlier_cap 100 \
        : join.run run.sesame
done
