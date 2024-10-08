#!/bin/bash

export OMP_NUM_THREADS=4

export cover="join.new input_file $HOME/Sesame/build/benchmark/datasets/CoverType.txt : join.new num_points 581012 : join.new dim 54 : join.new num_clusters 7 : join.new run_cmm false"

ticat ${meta} : ${cover} \
    : join.new algo 0 \
    : join.new max_in_nodes 800 \
    : join.new max_leaf_nodes 800 \
    : join.new distance_threshold 800 \
    : join.new landmark 30000 \
    : join.run run.sesame &

ticat ${meta} : ${cover} \
    : join.new algo 1 \
    : join.new seed 1 \
    : join.new coreset_size 7 \
    : join.new k 70 \
    : join.run run.sesame &

ticat ${meta} : ${cover} \
    : join.new algo 2 \
    : join.new num_last_arr 2 \
    : join.new time_window 350 \
    : join.new num_online_clusters 110 \
    : join.new radius 14 \
    : join.new buf_size 1500 \
    : join.run run.sesame &

ticat ${meta} : ${cover} \
    : join.new algo 3 \
    : join.new buf_size 500 \
    : join.new min_points 30 \
    : join.new epsilon 35 \
    : join.new base 2 \
    : join.new lambda 0.25 \
    : join.new mu 6 \
    : join.new beta 0.2 \
    : join.run run.sesame &

ticat ${meta} : ${cover} \
    : join.new algo 4 \
    : join.new lambda 0.125 \
    : join.new radius 300 \
    : join.new clean_interval 400 \
    : join.new min_weight 0.5 \
    : join.new alpha 0.5 \
    : join.new base 2 \
    : join.run run.sesame &

ticat ${meta} : ${cover} \
    : join.new algo 5 \
    : join.new radius 250 \
    : join.new delta 1500 \
    : join.new beta 1 \
    : join.new buf_size 1000 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.run run.sesame &

ticat ${meta} : ${cover} \
    : join.new algo 6 \
    : join.new lambda 0.998 \
    : join.new beta 1 \
    : join.new cm 1 \
    : join.new cl 0.8 \
    : join.new grid_width 50 \
    : join.run run.sesame &

ticat ${meta} : ${cover} \
    : join.new algo 7 \
    : join.new delta_grid 1 \
    : join.new num_samples 100 \
    : join.new sliding 100 \
    : join.run run.sesame &

wait

export kdd99="join.new input_file $HOME/Sesame/build/benchmark/datasets/KDD99_F.txt : join.new num_points 4898431 : join.new dim 41 : join.new num_clusters 23 : join.new run_cmm false"

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
    : join.new num_online_clusters 20 \
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
    : join.new algo 7 \
    : join.new delta_grid 100 \
    : join.new num_samples 10 \
    : join.new sliding 10 \
    : join.run run.sesame &

wait

export sensor="join.new input_file $HOME/Sesame/build/benchmark/datasets/Sensor.txt : join.new num_points 2219803 : join.new dim 5 : join.new num_clusters 55 : join.new run_cmm false : join.new arr_rate 0"

ticat ${meta} : ${sensor} \
    : join.new algo 0 \
    : join.new max_in_nodes 1000 \
    : join.new max_leaf_nodes 1000 \
    : join.new distance_threshold 500 \
    : join.new landmark 10000000 \
    : join.run run.sesame &

ticat ${meta} : ${sensor} \
    : join.new algo 1 \
    : join.new seed 100 \
    : join.new coreset_size 55 \
    : join.new k 1399 \
    : join.run run.sesame &

ticat ${meta} : ${sensor} \
    : join.new algo 2 \
    : join.new num_last_arr 2 \
    : join.new time_window 1000 \
    : join.new num_online_clusters 150 \
    : join.new radius 15 \
    : join.new buf_size 500 \
    : join.run run.sesame &

ticat ${meta} : ${sensor} \
    : join.new algo 3 \
    : join.new buf_size 500 \
    : join.new min_points 5 \
    : join.new epsilon 0.8 \
    : join.new base 2 \
    : join.new lambda 0.25 \
    : join.new mu 6 \
    : join.new beta 0.2 \
    : join.run run.sesame &

ticat ${meta} : ${sensor} \
    : join.new algo 4 \
    : join.new lambda 0.01 \
    : join.new radius 5 \
    : join.new clean_interval 2000 \
    : join.new min_weight 0.05 \
    : join.new alpha 0.03 \
    : join.new base 2 \
    : join.run run.sesame &

ticat ${meta} : ${sensor} \
    : join.new algo 5 \
    : join.new radius 1.6 \
    : join.new delta 300 \
    : join.new beta 0.7 \
    : join.new buf_size 10000 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.run run.sesame &

ticat ${meta} : ${sensor} \
    : join.new algo 6 \
    : join.new lambda 0.998 \
    : join.new beta 0.1 \
    : join.new cm 3 \
    : join.new cl 1 \
    : join.new grid_width 200 \
    : join.run run.sesame &

ticat ${meta} : ${sensor} \
    : join.new algo 7 \
    : join.new delta_grid 1 \
    : join.new num_samples 100 \
    : join.new sliding 100 \
    : join.run run.sesame &

wait

export insects="join.new input_file $HOME/Sesame/build/benchmark/datasets/INSECTS.txt : join.new num_points 905145 : join.new dim 33 : join.new num_clusters 24 : join.new run_cmm false : join.new arr_rate 0"

ticat ${meta} : ${insects} \
    : join.new algo 0 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 5 \
    : join.new landmark 10000000 \
    : join.run run.sesame &

ticat ${meta} : ${insects} \
    : join.new algo 1 \
    : join.new seed 1 \
    : join.new coreset_size 24 \
    : join.new k 102 \
    : join.run run.sesame &

ticat ${meta} : ${insects} \
    : join.new algo 2 \
    : join.new num_last_arr 20 \
    : join.new time_window 350 \
    : join.new num_online_clusters 30 \
    : join.new radius 30 \
    : join.new buf_size 500 \
    : join.run run.sesame &

ticat ${meta} : ${insects} \
    : join.new algo 3 \
    : join.new buf_size 1000 \
    : join.new min_points 30 \
    : join.new epsilon 0.1 \
    : join.new base 2 \
    : join.new lambda 0.25 \
    : join.new mu 6 \
    : join.new beta 0.6 \
    : join.run run.sesame &

ticat ${meta} : ${insects} \
    : join.new algo 4 \
    : join.new lambda 0.001 \
    : join.new radius 0.1 \
    : join.new clean_interval 1500 \
    : join.new min_weight 0.01 \
    : join.new alpha 0.001 \
    : join.new base 2 \
    : join.run run.sesame &

ticat ${meta} : ${insects} \
    : join.new algo 5 \
    : join.new radius 0.4 \
    : join.new delta 1500 \
    : join.new beta 0.4 \
    : join.new buf_size 10000 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.run run.sesame &

ticat ${meta} : ${insects} \
    : join.new algo 6 \
    : join.new lambda 0.998 \
    : join.new beta 0.001 \
    : join.new cm 3 \
    : join.new cl 0.7 \
    : join.new grid_width 0.08 \
    : join.run run.sesame &

ticat ${meta} : ${insects} \
    : join.new algo 7 \
    : join.new delta_grid 1 \
    : join.new num_samples 100 \
    : join.new sliding 100 \
    : join.run run.sesame &

wait