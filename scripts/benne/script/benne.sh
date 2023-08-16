#!/bin/bash

export cover="join.new input_file $HOME/Sesame/build/benchmark/datasets/CoverType.txt : join.new num_points 581012 : join.new dim 54 : join.new num_clusters 7 : join.new run_cmm false"
export kdd99="join.new input_file $HOME/Sesame/build/benchmark/datasets/KDD99_F.txt : join.new num_points 4898431 : join.new dim 41 : join.new num_clusters 23 : join.new run_cmm false"
export sensor="join.new input_file $HOME/Sesame/build/benchmark/datasets/sensor.txt : join.new num_points 2219803 : join.new dim 5 : join.new num_clusters 55 : join.new run_cmm false : join.new arr_rate 0"
export insects="join.new input_file $HOME/Sesame/build/benchmark/datasets/INSECTS.txt : join.new num_points 905145 : join.new dim 33 : join.new num_clusters 24 : join.new run_cmm false : join.new arr_rate 0"

export balance=0
export accuracy=1
export efficiency=2

export OMP_NUM_THREADS=8

# balance 
# var: (4363,4364)
# ticat ${meta} : ${cover} \
#     : join.new algo 8 \
#     : join.new obj $balance \
#     : join.new landmark 3000 \
#     : join.new queue_size_threshold 15000 \
#     : join.new dim_threshold 30 \
#     : join.new variance_threshold 4363 \
#     : join.new outliers_num_threshold 20 \
#     : join.new outliers_dist_threshold 0.5 \
#     : join.new coreset_size 50 \
#     : join.new k 200 \
#     : join.run run.sesame &

# TODO

# ticat ${meta} : ${sensor} \
#     : join.new algo 8 \
#     : join.new obj $balance \
#     : join.new landmark 3000 \
#     : join.new distance_threshold 400 \
#     : join.new queue_size_threshold 50000 \
#     : join.new dim_threshold 3 \
#     : join.new variance_threshold 10000.0 \
#     : join.new outliers_num_threshold 200 \
#     : join.new outliers_dist_threshold 50.0 \
#     : join.new k 2000 \
#     : join.new coreset_size 2 \
#     : join.run run.sesame &

# ticat ${meta} : ${insects} \
#     : join.new algo 8 \
#     : join.new obj $balance \
#     : join.new landmark 2000 \
#     : join.new queue_size_threshold 2000 \
#     : join.new dim_threshold 30 \
#     : join.new variance_threshold 100.0 \
#     : join.new outliers_num_threshold 500 \
#     : join.new outliers_dist_threshold 50 \
#     : join.new coreset_size 2 \
#     : join.run run.sesame &

# accuracy
ticat ${meta} : ${cover} \
    : join.new algo 8 \
    : join.new obj $accuracy \
    : join.new distance_threshold 500 \
    : join.new queue_size_threshold 10000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame &

ticat ${meta} : ${sensor} \
    : join.new algo 8 \
    : join.new obj $accuracy \
    : join.new landmark 40000 \
    : join.new distance_threshold 5 \
    : join.new queue_size_threshold 5000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 1000.0 \
    : join.new outliers_num_threshold 100 \
    : join.new outliers_dist_threshold 5.0 \
    : join.new k 1000 \
    : join.run run.sesame &

ticat ${meta} : ${insects} \
    : join.new algo 8 \
    : join.new obj $accuracy \
    : join.new landmark 5000 \
    : join.new distance_threshold 0.5 \
    : join.new queue_size_threshold 10000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame &

# efficiency
ticat ${meta} : ${cover} \
    : join.new algo 8 \
    : join.new obj $efficiency \
    : join.new landmark 150000 \
    : join.new queue_size_threshold 300000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame &

ticat ${meta} : ${sensor} \
    : join.new algo 8 \
    : join.new obj $efficiency \
    : join.new landmark 2000 \
    : join.new queue_size_threshold 3000000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame &

ticat ${meta} : ${insects} \
    : join.new algo 8 \
    : join.new obj $efficiency \
    : join.new sliding 20000 \
    : join.new queue_size_threshold 400000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 5.0 \
    : join.run run.sesame &

wait