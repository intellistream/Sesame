#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/all.sh"

for i in $(seq 1 5)
do
    eds=`eval echo \$\{eds$i\}`
    ticat ${meta} : mark-time bench.begin : ${eds} \
        : join.new algo 3 \
        : join.new buf_size 500 \
        : join.new min_points 30 \
        : join.new epsilon 20 \
        : join.new base 2 \
        : join.new lambda 0.25 \
        : join.new mu 6 \
        : join.new beta 0.2 \
        : join.run run.sesame
done

for i in $(seq 1 3)
do
    edso=`eval echo \$\{edso$i\}`
    ticat ${meta} : mark-time bench.begin : ${edso} \
        : join.new algo 3 \
        : join.new buf_size 500 \
        : join.new min_points 30 \
        : join.new epsilon 20 \
        : join.new base 2 \
        : join.new lambda 0.25 \
        : join.new mu 6 \
        : join.new beta 0.2 \
        : join.run run.sesame
done
