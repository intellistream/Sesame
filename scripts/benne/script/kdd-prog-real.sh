#!/bin/bash

export kdd99="join.new input_file $HOME/Sesame/build/benchmark/datasets/KDD99_F.txt : join.new num_points 48984,97968,146952,195936,244920,293904,342888,391872,440856,489840,538824,587808,636792,685776,734760,783744,832728,881712,930696,979680,1028664,1077648,1126632,1175616,1224600,1273584,1322568,1371552,1420536,1469520,1518504,1567488,1616472,1665456,1714440,1763424,1812408,1861392,1910376,1959360,2008344,2057328,2106312,2155296,2204280,2253264,2302248,2351232,2400216,2449200,2498184,2547168,2596152,2645136,2694120,2743104,2792088,2841072,2890056,2939040,2988024,3037008,3085992,3134976,3183960,3232944,3281928,3330912,3379896,3428880,3477864,3526848,3575832,3624816,3673800,3722784,3771768,3820752,3869736,3918720,3967704,4016688,4065672,4114656,4163640,4212624,4261608,4310592,4359576,4408560,4457544,4506528,4555512,4604496,4653480,4702464,4751448,4800432,4849416,4898431 : join.new dim 41 : join.new num_clusters 23 : join.new run_cmm false"

export balance=0
export accuracy=1
export efficiency=2
export acc_no_mig=3

export OMP_NUM_THREADS=10

meta="{bench.tag=step.kdd.real} $meta"

ticat ${meta} : ${kdd99} \
    : join.new algo 7 \
    : join.new delta_grid 100 \
    : join.new num_samples 10 \
    : join.new sliding 10 \
    : join.run run.sesame &

ticat ${meta} : ${kdd99} \
    : join.new algo 0 \
    : join.new max_in_nodes 40000 \
    : join.new max_leaf_nodes 40000 \
    : join.new distance_threshold 60000 \
    : join.new landmark 500000 \
    : join.run run.sesame &

ticat ${meta} : ${kdd99} \
    : join.new algo 1 \
    : join.new seed 100 \
    : join.new coreset_size 23 \
    : join.new k 454 \
    : join.run run.sesame &

ticat ${meta} : ${kdd99} \
    : join.new algo 2 \
    : join.new num_last_arr 2 \
    : join.new time_window 350 \
    : join.new num_online_clusters 200 \
    : join.new radius 8 \
    : join.new buf_size 1500 \
    : join.run run.sesame &

ticat ${meta} : ${kdd99} \
    : join.new algo 3 \
    : join.new buf_size 500 \
    : join.new min_points 30 \
    : join.new epsilon 35 \
    : join.new base 2 \
    : join.new lambda 0.25 \
    : join.new mu 6 \
    : join.new beta 0.2 \
    : join.run run.sesame &

ticat ${meta} : ${kdd99} \
    : join.new algo 4 \
    : join.new lambda 0.01 \
    : join.new radius 1000 \
    : join.new clean_interval 5000 \
    : join.new min_weight 0.5 \
    : join.new alpha 0.4 \
    : join.new base 2 \
    : join.run run.sesame &

ticat ${meta} : ${kdd99} \
    : join.new algo 5 \
    : join.new radius 100 \
    : join.new delta 1500 \
    : join.new beta 0.7 \
    : join.new buf_size 10000 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.run run.sesame &

ticat ${meta} : ${kdd99} \
    : join.new algo 6 \
    : join.new lambda 0.998 \
    : join.new beta 0.001 \
    : join.new cm 1 \
    : join.new cl 0.7 \
    : join.new grid_width 50 \
    : join.run run.sesame &

ticat ${meta} : ${kdd99} \
    : join.new algo 23 \
    : join.new max_in_nodes 4000 \
    : join.new max_leaf_nodes 4000 \
    : join.new distance_threshold 6000 \
    : join.new landmark 30000 \
    : join.new outlier_distance_threshold 80000 \
    : join.new outlier_cap 5 \
    : join.run run.sesame &

wait