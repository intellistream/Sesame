#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/all.sh"

for i in $(seq 1 5)
do
    eds=`eval echo \$\{eds$i\}`
    ticat ${meta} : ${eds} \
        : join.new algo 4 \
        : join.new lambda 0.998 \
        : join.new radius 20 \
        : join.new clean_interval 400 \
        : join.new min_weight 0.5 \
        : join.new alpha 0.2 \
        : join.new base 2 \
        : join.run run.sesame
done

for i in $(seq 1 3)
do
    edso=`eval echo \$\{edso$i\}`
    ticat ${meta} : ${edso} \
        : join.new algo 4 \
        : join.new lambda 0.998 \
        : join.new radius 10 \
        : join.new clean_interval 400 \
        : join.new min_weight 0.5 \
        : join.new alpha 0.2 \
        : join.new base 2 \
        : join.run run.sesame
done
