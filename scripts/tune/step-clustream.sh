#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/all.sh"

for i in $(seq 1 5)
do
    eds=`eval echo \$\{eds$i\}`
    ticat ${meta} : mark-time bench.begin : ${eds} \
        : join.new algo 2 \
        : join.new num_last_arr 2 \
        : join.new time_window 200 \
        : join.new num_online_clusters 1000 \
        : join.new radius 10 \
        : join.new buf_size 1500 \
        : join.run run.sesame
done

for i in $(seq 1 3)
do
    edso=`eval echo \$\{edso$i\}`
    ticat ${meta} : mark-time bench.begin : ${edso} \
        : join.new algo 2 \
        : join.new num_last_arr 2 \
        : join.new time_window 200 \
        : join.new num_online_clusters 300 \
        : join.new radius 10 \
        : join.new buf_size 1500 \
        : join.run run.sesame
done
