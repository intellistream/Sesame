#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/all.sh"

for i in $(seq 1 5)
do
    eds=`eval echo \$\{eds$i\}`
    ticat ${meta} : mark-time bench.begin : ${eds} \
        : join.new algo 23 \
        : join.new max_in_nodes 100 \
        : join.new max_leaf_nodes 100 \
        : join.new distance_threshold 100 \
        : join.new landmark 1000 \
        : join.new outlier_distance_threshold 15 \
        : join.new outlier_cap 500 \
        : join.run run.sesame
done

for i in $(seq 1 3)
do
    edso=`eval echo \$\{edso$i\}`
    ticat ${meta} : mark-time bench.begin : ${edso} \
        : join.new algo 23 \
        : join.new max_in_nodes 100 \
        : join.new max_leaf_nodes 100 \
        : join.new distance_threshold 50 \
        : join.new landmark 1000 \
        : join.new outlier_distance_threshold 15 \
        : join.new outlier_cap 500 \
        : join.run run.sesame
done
