#!/bin/bash

export eds1="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 45690 : join.new dim 2 : join.new num_clusters 75 : join.new run_cmm true"
export eds2="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 100270 : join.new dim 2 : join.new num_clusters 145 : join.new run_cmm true"
export eds3="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 150645 : join.new dim 2 : join.new num_clusters 218 : join.new run_cmm true"
export eds4="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 200060 : join.new dim 2 : join.new num_clusters 289 : join.new run_cmm true"
export eds5="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 245270 : join.new dim 2 : join.new num_clusters 363 : join.new run_cmm true"

export edso1="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS_O.txt : join.new num_points 94270 : join.new dim 2 : join.new num_clusters 90"
export edso2="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS_O.txt : join.new num_points 97360 : join.new dim 2 : join.new num_clusters 90"
export edso3="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS_O.txt : join.new num_points 100000 : join.new dim 2 : join.new num_clusters 90"

export eods="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS_ODS.txt : join.new num_points 45690,100270,150645,200060,245270,339990,342630,345270 : join.new dim 2 : join.new num_clusters 454"

export balance=0
export accuracy="1,3"
export efficiency=2

export OMP_NUM_THREADS=8

function run(){

ticat ${meta} : ${eds} \
    : join.new algo 0 \
    : join.new max_in_nodes 500 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark 10000000 \
    : join.run run.sesame &

ticat ${meta} : ${eds}\
    : join.new algo 1 \
    : join.new seed 10 \
    : join.new coreset_size 50 \
    : join.new k 363 \
    : join.run run.sesame &

ticat ${meta} : ${eds}\
    : join.new algo 2 \
    : join.new num_last_arr 2 \
    : join.new time_window 200 \
    : join.new num_online_clusters 1000 \
    : join.new radius 10 \
    : join.new buf_size 1500 \
    : join.run run.sesame &

ticat ${meta} : ${eds}\
    : join.new algo 3 \
    : join.new buf_size 500 \
    : join.new min_points 30 \
    : join.new epsilon 200 \
    : join.new base 2 \
    : join.new lambda 0.25 \
    : join.new mu 6 \
    : join.new beta 0.2 \
    : join.run run.sesame &

ticat ${meta} : ${eds}\
    : join.new algo 4 \
    : join.new lambda 0.01 \
    : join.new radius 100 \
    : join.new clean_interval 400 \
    : join.new min_weight 0.001 \
    : join.new alpha 0.02 \
    : join.new base 2 \
    : join.run run.sesame &

wait

ticat ${meta} : ${eds}\
    : join.new algo 5 \
    : join.new radius 0.5 \
    : join.new delta 2500 \
    : join.new beta 1 \
    : join.new buf_size 5000 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.run run.sesame &

ticat ${meta} : ${eds}\
    : join.new algo 6 \
    : join.new lambda 0.998 \
    : join.new beta 0.1 \
    : join.new cm 3 \
    : join.new cl 1 \
    : join.new grid_width 50 \
    : join.run run.sesame &

ticat ${meta} : ${eds}\
    : join.new algo 7 \
    : join.new delta_grid 1 \
    : join.new num_samples 100 \
    : join.new sliding 10 \
    : join.run run.sesame &

# ticat ${meta} : ${eds} \
#     : join.new algo 8 \
#     : join.new obj $balance \
#     : join.new distance_threshold 8 \
#     : join.new landmark 2000 \
#     : join.new queue_size_threshold 10000 \
#     : join.new dim_threshold 10 \
#     : join.new variance_threshold 1000.0 \
#     : join.new outliers_num_threshold 200 \
#     : join.new outliers_dist_threshold 50.0 \
#     : join.new coreset_size 10 \
#     : join.new k 1000 \
#     : join.run run.sesame &

ticat ${meta} : ${eds} \
    : join.new algo 8 \
    : join.new obj $accuracy \
    : join.new distance_threshold 8 \
    : join.new queue_size_threshold 50000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 50.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame &

ticat ${meta} : ${eds} \
    : join.new algo 8,36 \
    : join.new obj $efficiency \
    : join.new sliding 2000 \
    : join.new queue_size_threshold 80000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame &

wait
    
}

for i in $(seq 1 5)
do
    export eds=`eval echo \$\{eds$i\}`
    run
done

for i in $(seq 1 3)
do
    export eds=`eval echo \$\{edso$i\}`
    run
done

# export eds=$eods
# run
