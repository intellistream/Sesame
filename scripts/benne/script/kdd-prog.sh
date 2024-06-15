#!/bin/bash

# export kdd99="join.new input_file $HOME/Sesame/build/benchmark/datasets/KDD99_F.txt : join.new num_points 250000 : join.new dim 41 : join.new num_clusters 23 : join.new run_cmm false"

export kdd99="join.new input_file $HOME/Sesame/build/benchmark/datasets/KDD99_F.txt : join.new num_points 48984,97968,146952,195936,244920,293904,342888,391872,440856,489840,538824,587808,636792,685776,734760,783744,832728,881712,930696,979680,1028664,1077648,1126632,1175616,1224600,1273584,1322568,1371552,1420536,1469520,1518504,1567488,1616472,1665456,1714440,1763424,1812408,1861392,1910376,1959360,2008344,2057328,2106312,2155296,2204280,2253264,2302248,2351232,2400216,2449200,2498184,2547168,2596152,2645136,2694120,2743104,2792088,2841072,2890056,2939040,2988024,3037008,3085992,3134976,3183960,3232944,3281928,3330912,3379896,3428880,3477864,3526848,3575832,3624816,3673800,3722784,3771768,3820752,3869736,3918720,3967704,4016688,4065672,4114656,4163640,4212624,4261608,4310592,4359576,4408560,4457544,4506528,4555512,4604496,4653480,4702464,4751448,4800432,4849416,4898431 : join.new dim 41 : join.new num_clusters 23 : join.new run_cmm false"

export balance=0
export accuracy=1
export efficiency=2
export acc_no_mig=3

export OMP_NUM_THREADS=4

meta="{bench.tag=step.kdd} $meta"

ticat ${meta} : ${kdd99} \
    : join.new algo 8 \
    : join.new obj $balance \
    : join.new landmark 200000 \
    : join.new queue_size_threshold 70000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame &

ticat ${meta} : ${kdd99} \
    : join.new algo 8 \
    : join.new obj $accuracy \
    : join.new landmark 50000 \
    : join.new distance_threshold 400 \
    : join.new queue_size_threshold 50000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.new outlier_cap 2 \
    : join.new clean_interval 100000 \
    : join.new k 10 \
    : join.run run.sesame &

# ticat ${meta} : ${kdd99} \
#     : join.new algo 8 \
#     : join.new obj $acc_no_mig \
#     : join.new landmark 50000 \
#     : join.new distance_threshold 4000 \
#     : join.new queue_size_threshold 50000 \
#     : join.new dim_threshold 30 \
#     : join.new variance_threshold 100.0 \
#     : join.new outliers_num_threshold 200 \
#     : join.new outliers_dist_threshold 50.0 \
#     : join.new outlier_cap 2 \
#     : join.new clean_interval 100000 \
#     : join.new k 10 \
#     : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 8 \
    : join.new obj $efficiency \
    : join.new sliding 1000 \
    : join.new landmark 500000 \
    : join.new queue_size_threshold 3000000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame &

# ticat ${meta} : ${kdd99} \
#     : join.new algo 36 \
#     : join.new sliding 1000 \
#     : join.new landmark 500000 \
#     : join.new queue_size_threshold 3000000 \
#     : join.new dim_threshold 30 \
#     : join.new variance_threshold 100.0 \
#     : join.new outliers_num_threshold 200 \
#     : join.new outliers_dist_threshold 50.0 \
#     : join.run run.sesame &

wait