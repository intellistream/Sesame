#!/bin/bash

export eds="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 245270 : join.new dim 2 : join.new num_clusters 363"
export edso="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS_O.txt : join.new num_points 100000 : join.new dim 2 : join.new num_clusters 90"

function SEQ()
{
    seq $1 $2 $3 | sed 's/,/./g'
}

# window model
ticat ${meta} : mark-time bench.begin : ${eds} \
    : join.new algo 23 \
    : join.new max_in_nodes 100 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark [30000,200000,10000] \
    : join.new outlier_distance_threshold 15 \
    : join.new outlier_cap 500 \
    : join.run run.sesame

# ticat ${meta} : mark-time bench.begin : ${edso} \
#     : join.new algo 23 \
#     : join.new max_in_nodes 100 \
#     : join.new max_leaf_nodes 100 \
#     : join.new distance_threshold 50 \
#     : join.new landmark [2000,20000,2000] \
#     : join.new outlier_distance_threshold 15 \
#     : join.new outlier_cap 500 \
#     : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${eds} \
    : join.new algo 24 \
    : join.new max_in_nodes 100 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new sliding [30000,200000,10000] \
    : join.new outlier_distance_threshold 15 \
    : join.new outlier_cap 500 \
    : join.run run.sesame

# ticat ${meta} : mark-time bench.begin : ${edso} \
#     : join.new algo 24 \
#     : join.new max_in_nodes 100 \
#     : join.new max_leaf_nodes 100 \
#     : join.new distance_threshold 20 \
#     : join.new sliding [2000,20000,2000] \
#     : join.new outlier_distance_threshold 15 \
#     : join.new outlier_cap 500 \
#     : join.run run.sesame

for l in $(SEQ 0.2 0.2 2.0)
do
    # a=$(python3 -c "print($l+1.0)")
    # echo a=$a,l=$l
    ticat ${meta} : mark-time bench.begin : ${eds} \
    : join.new algo 25 \
    : join.new max_in_nodes 100 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark 1000 \
    : join.new outlier_distance_threshold 15 \
    : join.new outlier_cap 500 \
    : join.new lambda $l \
    : join.new alpha 1.1 \
    : join.run run.sesame
done

for a in $(SEQ 1.2 0.2 3.0)
do
    ticat ${meta} : mark-time bench.begin : ${edso} \
        : join.new algo 25 \
        : join.new max_in_nodes 100 \
        : join.new max_leaf_nodes 100 \
        : join.new distance_threshold 50 \
        : join.new landmark 1000 \
        : join.new outlier_distance_threshold 15 \
        : join.new outlier_cap 500 \
        : join.new lambda 0.5 \
        : join.new alpha $a \
        : join.run run.sesame
done

# buffer size
# ticat ${meta} : mark-time bench.begin : ${eds} \
#     : join.new algo 23 \
#     : join.new max_in_nodes 100 \
#     : join.new max_leaf_nodes 100 \
#     : join.new distance_threshold 100 \
#     : join.new landmark 1000 \
#     : join.new outlier_distance_threshold [10,40,5] \
#     : join.new outlier_cap 500 \
#     : join.run run.sesame

# ticat ${meta} : mark-time bench.begin : ${edso} \
#     : join.new algo 23 \
#     : join.new max_in_nodes 100 \
#     : join.new max_leaf_nodes 100 \
#     : join.new distance_threshold 50 \
#     : join.new landmark 1000 \
#     : join.new outlier_distance_threshold [10,40,5] \
#     : join.new outlier_cap 500 \
#     : join.run run.sesame

# cluster cap

for m in $(SEQ 100 100 2000)
do
ticat ${meta} : mark-time bench.begin : ${eds} \
    : join.new algo 23 \
    : join.new max_in_nodes $m \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark 1000 \
    : join.new outlier_distance_threshold 15 \
    : join.new outlier_cap 500 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${eds} \
    : join.new algo 23 \
    : join.new max_in_nodes 100 \
    : join.new max_leaf_nodes $m \
    : join.new distance_threshold 50 \
    : join.new landmark 1000 \
    : join.new outlier_distance_threshold 15 \
    : join.new outlier_cap 500 \
    : join.run run.sesame
done