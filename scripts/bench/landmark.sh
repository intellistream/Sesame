#!/bin/bash

export cover="join.new input_file $HOME/Sesame/build/benchmark/datasets/CoverType.txt : join.new num_points 581012 : join.new dim 54 : join.new num_clusters 7 : join.new run_cmm false"
export kdd99="join.new input_file $HOME/Sesame/build/benchmark/datasets/KDD99_F.txt : join.new num_points 4898431 : join.new dim 41 : join.new num_clusters 23 : join.new run_cmm false"
export sensor="join.new input_file $HOME/Sesame/build/benchmark/datasets/sensor.txt : join.new num_points 2219803 : join.new dim 5 : join.new num_clusters 55 : join.new run_cmm false : join.new arr_rate 0"
export insects="join.new input_file $HOME/Sesame/build/benchmark/datasets/INSECTS.txt : join.new num_points 905145 : join.new dim 33 : join.new num_clusters 24 : join.new run_cmm false : join.new arr_rate 0"

meta="${meta} : {bench.tag=extra.conf.landmark}"

ticat ${meta} : ${cover} \
    : join.new algo 21 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 600 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_distance_threshold 1000 \
    : join.new outlier_cap 500 \
    : join.new k 70 \
    : join.run run.sesame

ticat ${meta} : ${cover} \
    : join.new algo 22 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 600 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_distance_threshold 1000 \
    : join.new outlier_cap 500 \
    : join.new min_points 10 \
    : join.new epsilon 25 \
    : join.run run.sesame

ticat ${meta} : ${cover} \
    : join.new algo 23 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 600 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_distance_threshold 1000 \
    : join.new outlier_cap 500 \
    : join.run run.sesame

ticat ${meta} : ${cover} \
    : join.new algo 26 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 600 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.run run.sesame

ticat ${meta} : ${cover} \
    : join.new algo 27 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 600 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_density_threshold 100 \
    : join.new neighbor_distance 5000 \
    : join.new outlier_cap 300 \
    : join.run run.sesame

ticat ${meta} : ${cover} \
    : join.new algo 28 \
    : join.new distance_threshold 600 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_distance_threshold 1000 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${cover} \
    : join.new algo 31,32,33 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 600 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_distance_threshold 1000 \
    : join.new outlier_cap 500 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 21 \
    : join.new max_in_nodes 40000 \
    : join.new max_leaf_nodes 40000 \
    : join.new distance_threshold 60000 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_distance_threshold 80000 \
    : join.new outlier_cap 5 \
    : join.new k 454 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 22 \
    : join.new max_in_nodes 40000 \
    : join.new max_leaf_nodes 40000 \
    : join.new distance_threshold 60000 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_distance_threshold 80000 \
    : join.new outlier_cap 5 \
    : join.new min_points 10 \
    : join.new epsilon 25 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 23 \
    : join.new max_in_nodes 40000 \
    : join.new max_leaf_nodes 40000 \
    : join.new distance_threshold 60000 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_distance_threshold 80000 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 26 \
    : join.new max_in_nodes 40000 \
    : join.new max_leaf_nodes 40000 \
    : join.new distance_threshold 60000 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 27 \
    : join.new max_in_nodes 40000 \
    : join.new max_leaf_nodes 40000 \
    : join.new distance_threshold 60000 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_density_threshold 100 \
    : join.new neighbor_distance 500 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 28 \
    : join.new distance_threshold 60000 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_distance_threshold 80000 \
    : join.new outlier_cap 3 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 31,32,33 \
    : join.new max_in_nodes 40000 \
    : join.new max_leaf_nodes 40000 \
    : join.new distance_threshold 60000 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_distance_threshold 80000 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${sensor} \
    : join.new algo 21 \
    : join.new max_in_nodes 1000 \
    : join.new max_leaf_nodes 1000 \
    : join.new distance_threshold 500 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_distance_threshold 20 \
    : join.new outlier_cap 5 \
    : join.new k 1399 \
    : join.run run.sesame

ticat ${meta} : ${sensor} \
    : join.new algo 22 \
    : join.new max_in_nodes 1000 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 500 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_distance_threshold 20 \
    : join.new outlier_cap 5 \
    : join.new min_points 5 \
    : join.new epsilon 0.8 \
    : join.run run.sesame

ticat ${meta} : ${sensor} \
    : join.new algo 23 \
    : join.new max_in_nodes 1000 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 500 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_distance_threshold 20 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${sensor} \
    : join.new algo 26 \
    : join.new max_in_nodes 1000 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 500 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${sensor} \
    : join.new algo 27 \
    : join.new max_in_nodes 1000 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 500 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_density_threshold 100 \
    : join.new neighbor_distance 300 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${sensor} \
    : join.new algo 28 \
    : join.new distance_threshold 500 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_distance_threshold 1000 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${sensor} \
    : join.new algo 31,32,33 \
    : join.new max_in_nodes 1000 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 500 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_distance_threshold 20 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 21 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 5 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_distance_threshold 0.12 \
    : join.new outlier_cap 5 \
    : join.new k 102 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 22 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 5 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_distance_threshold 0.12 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 23 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 5 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_distance_threshold 0.12 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 26 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 5 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_distance_threshold 0.12 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 27 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 0.7 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_density_threshold 100 \
    : join.new neighbor_distance 300 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 28 \
    : join.new distance_threshold 5 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_distance_threshold 10 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 31,32,33 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 5 \
    : join.new landmark 1000,2000,4000,8000,16000,32000,50000,100000 \
    : join.new outlier_distance_threshold 0.12 \
    : join.new outlier_cap 5 \
    : join.run run.sesame
