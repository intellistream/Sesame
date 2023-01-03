#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/all.sh"

for i in $(seq 1 5)
do
    eds=`eval echo \$\{eds$i\}`
    ticat ${meta} : ${eds} \
        : join.new algo 0 \
        : join.new max_in_nodes 100 \
        : join.new max_leaf_nodes 100 \
        : join.new distance_threshold 100 \
        : join.run run.sesame
done

for i in $(seq 1 3)
do
    edso=`eval echo \$\{edso$i\}`
    ticat ${meta} : ${edso} \
        : join.new algo 0 \
        : join.new max_in_nodes 100 \
        : join.new max_leaf_nodes 100 \
        : join.new distance_threshold 50 \
        : join.run run.sesame
done
