#!/bin/bash

export cover="join.new input_file $HOME/Sesame/build/benchmark/datasets/CoverType.txt : join.new num_points 581012 : join.new dim 54 : join.new num_clusters 7 : join.new run_cmm false"
export kdd99="join.new input_file $HOME/Sesame/build/benchmark/datasets/KDD99_F.txt : join.new num_points 4898431 : join.new dim 41 : join.new num_clusters 23 : join.new run_cmm false"
export sensor="join.new input_file $HOME/Sesame/build/benchmark/datasets/sensor.txt : join.new num_points 2219803 : join.new dim 5 : join.new num_clusters 55 : join.new run_cmm false : join.new arr_rate 0"
export insects="join.new input_file $HOME/Sesame/build/benchmark/datasets/INSECTS.txt : join.new num_points 905145 : join.new dim 33 : join.new num_clusters 24 : join.new run_cmm false : join.new arr_rate 0"

ticat ${meta} : ${cover} \
    : join.new algo 8 \
    : join.new obj 0 \
    : join.new queue_size_threshold 10000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame

ticat ${meta} : ${cover} \
    : join.new algo 8 \
    : join.new obj 1 \
    : join.new queue_size_threshold 10000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame

ticat ${meta} : ${cover} \
    : join.new algo 8 \
    : join.new obj 2 \
    : join.new queue_size_threshold 10000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 8 \
    : join.new obj 0 \
    : join.new queue_size_threshold 10000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 8 \
    : join.new obj 1 \
    : join.new queue_size_threshold 10000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 8 \
    : join.new obj 2 \
    : join.new queue_size_threshold 10000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame

ticat ${meta} : ${sensor} \
    : join.new algo 8 \
    : join.new obj 0 \
    : join.new queue_size_threshold 10000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame

ticat ${meta} : ${sensor} \
    : join.new algo 8 \
    : join.new obj 1 \
    : join.new queue_size_threshold 10000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame

ticat ${meta} : ${sensor} \
    : join.new algo 8 \
    : join.new obj 2 \
    : join.new queue_size_threshold 10000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 8 \
    : join.new obj 0 \
    : join.new queue_size_threshold 10000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 8 \
    : join.new obj 1 \
    : join.new queue_size_threshold 10000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 8 \
    : join.new obj 2 \
    : join.new queue_size_threshold 10000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame