#!/bin/bash

ori_meta=$meta

export cover="join.new input_file $HOME/Sesame/build/benchmark/datasets/CoverType.txt : join.new num_points 581012 : join.new dim 54 : join.new num_clusters 7 : join.new run_cmm false"
export kdd99="join.new input_file $HOME/Sesame/build/benchmark/datasets/KDD99_F.txt : join.new num_points 4898431 : join.new dim 41 : join.new num_clusters 23 : join.new run_cmm false"
export sensor="join.new input_file $HOME/Sesame/build/benchmark/datasets/sensor.txt : join.new num_points 2219803 : join.new dim 5 : join.new num_clusters 55 : join.new run_cmm false : join.new arr_rate 0"
export insects="join.new input_file $HOME/Sesame/build/benchmark/datasets/INSECTS.txt : join.new num_points 905145 : join.new dim 33 : join.new num_clusters 24 : join.new run_cmm false : join.new arr_rate 0"

export balance=0
export accuracy=1
export efficiency=2

export OMP_NUM_THREADS=6

meta="{bench.tag=extra.queue} $ori_meta"
# ticat ${meta} : ${cover} \
#     : join.new algo 8 \
#     : join.new obj $balance \
#     : join.new landmark 50000 \
#     : join.new queue_size_threshold [20000,100000,20000] \
#     : join.new dim_threshold 30 \
#     : join.new variance_threshold 100.0 \
#     : join.new outliers_num_threshold 200 \
#     : join.new outliers_dist_threshold 50.0 \
#     : join.run run.sesame &

ticat ${meta} : ${cover} \
    : join.new algo 8 \
    : join.new obj $accuracy \
    : join.new distance_threshold 500 \
    : join.new queue_size_threshold [20000,100000,20000] \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame &

ticat ${meta} : ${cover} \
    : join.new algo 8 \
    : join.new obj $efficiency \
    : join.new landmark 20000 \
    : join.new queue_size_threshold [20000,100000,20000] \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame &

# meta="{bench.tag=extra.dim} $ori_meta"
# ticat ${meta} : ${cover} \
#     : join.new algo 8 \
#     : join.new obj $balance \
#     : join.new landmark 50000 \
#     : join.new queue_size_threshold 100 \
#     : join.new dim_threshold 40,50,60 \
#     : join.new variance_threshold 100.0 \
#     : join.new outliers_num_threshold 200 \
#     : join.new outliers_dist_threshold 50.0 \
#     : join.run run.sesame &

# ticat ${meta} : ${cover} \
#     : join.new algo 8 \
#     : join.new obj $accuracy \
#     : join.new distance_threshold 500 \
#     : join.new queue_size_threshold 70000 \
#     : join.new dim_threshold 40,50,60 \
#     : join.new variance_threshold 100.0 \
#     : join.new outliers_num_threshold 200 \
#     : join.new outliers_dist_threshold 50.0 \
#     : join.run run.sesame &

# ticat ${meta} : ${cover} \
#     : join.new algo 8 \
#     : join.new obj $efficiency \
#     : join.new landmark 20000 \
#     : join.new queue_size_threshold 1000 \
#     : join.new dim_threshold 40,50,60 \
#     : join.new variance_threshold 100.0 \
#     : join.new outliers_num_threshold 200 \
#     : join.new outliers_dist_threshold 50.0 \
#     : join.run run.sesame &

meta="{bench.tag=extra.var} $ori_meta"
# ticat ${meta} : ${cover} \
#     : join.new algo 8 \
#     : join.new obj $balance \
#     : join.new landmark 50000 \
#     : join.new queue_size_threshold 100 \
#     : join.new dim_threshold 30 \
#     : join.new variance_threshold [500,4000,500] \
#     : join.new outliers_num_threshold 200 \
#     : join.new outliers_dist_threshold 50.0 \
#     : join.run run.sesame &

ticat ${meta} : ${cover} \
    : join.new algo 8 \
    : join.new obj $accuracy \
    : join.new distance_threshold 500 \
    : join.new queue_size_threshold 10000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold [400,4000,400] \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame &

ticat ${meta} : ${cover} \
    : join.new algo 8 \
    : join.new obj $efficiency \
    : join.new landmark 20000 \
    : join.new queue_size_threshold 300000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold [400,4000,400] \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame &


meta="{bench.tag=extra.out_dist} $ori_meta"
# ticat ${meta} : ${cover} \
#     : join.new algo 8 \
#     : join.new obj $balance \
#     : join.new landmark 50000 \
#     : join.new queue_size_threshold 100 \
#     : join.new dim_threshold 30 \
#     : join.new variance_threshold 100.0 \
#     : join.new outliers_num_threshold 200 \
#     : join.new outliers_dist_threshold [10,100,10] \
#     : join.run run.sesame &

ticat ${meta} : ${cover} \
    : join.new algo 8 \
    : join.new obj $accuracy \
    : join.new distance_threshold 500 \
    : join.new queue_size_threshold 10000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold [10,100,10] \
    : join.run run.sesame &

ticat ${meta} : ${cover} \
    : join.new algo 8 \
    : join.new obj $efficiency \
    : join.new landmark 20000 \
    : join.new queue_size_threshold 300000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold [10,100,10] \
    : join.run run.sesame &

meta="{bench.tag=extra.out_num} $ori_meta"
# ticat ${meta} : ${cover} \
#     : join.new algo 8 \
#     : join.new obj $balance \
#     : join.new landmark 50000 \
#     : join.new queue_size_threshold 100 \
#     : join.new dim_threshold 30 \
#     : join.new variance_threshold 100.0 \
#     : join.new outliers_num_threshold [10,100,10] \
#     : join.new outliers_dist_threshold 50.0 \
#     : join.run run.sesame &

ticat ${meta} : ${cover} \
    : join.new algo 8 \
    : join.new obj $accuracy \
    : join.new distance_threshold 500 \
    : join.new queue_size_threshold 10000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold [10,100,10] \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame &

ticat ${meta} : ${cover} \
    : join.new algo 8 \
    : join.new obj $efficiency \
    : join.new landmark 20000 \
    : join.new queue_size_threshold 300000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold [10,100,10] \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame &

wait