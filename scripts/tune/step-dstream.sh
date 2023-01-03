#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/all.sh"

for i in $(seq 1 5)
do
    eds=`eval echo \$\{eds$i\}`
    ticat ${meta} : ${eds} \
        : join.new algo 6 \
        : join.new lambda 0.998 \
        : join.new beta 0.001 \
        : join.new cm 4 \
        : join.new cl 2 \
        : join.new grid_width 4 \
        : join.run run.sesame
done

for i in $(seq 1 3)
do
    edso=`eval echo \$\{edso$i\}`
    ticat ${meta} : ${edso} \
        : join.new algo 6 \
        : join.new lambda 0.998 \
        : join.new beta 0.001 \
        : join.new cm 4 \
        : join.new cl 2 \
        : join.new grid_width 4 \
        : join.run run.sesame
done
