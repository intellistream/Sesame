#!/bin/bash

export eds1="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 45690 : join.new dim 2 : join.new num_clusters 75"
export eds2="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 100270 : join.new dim 2 : join.new num_clusters 145"
export eds3="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 150645 : join.new dim 2 : join.new num_clusters 218"
export eds4="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 200060 : join.new dim 2 : join.new num_clusters 289"
export eds5="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 245270 : join.new dim 2 : join.new num_clusters 363"

export edso1="join.new input_file $HOME/Sesame/build/benchmark/datasets/ODS.txt : join.new num_points 94270 : join.new dim 2 : join.new num_clusters 90"
export edso2="join.new input_file $HOME/Sesame/build/benchmark/datasets/ODS.txt : join.new num_points 97360 : join.new dim 2 : join.new num_clusters 90"
export edso3="join.new input_file $HOME/Sesame/build/benchmark/datasets/ODS.txt : join.new num_points 100000 : join.new dim 2 : join.new num_clusters 90"

function run(){
# ticat ${meta} : ${eds} \
#     : join.new algo 0 \
#     : join.new max_in_nodes 500 \
#     : join.new max_leaf_nodes 100 \
#     : join.new distance_threshold 100 \
#     : join.new landmark 10000000 \
#     : join.run run.sesame

# ticat ${meta} : ${eds}\
#     : join.new algo 1 \
#     : join.new seed 10 \
#     : join.new coreset_size 363 \
#     : join.new k 588 \
#     : join.run run.sesame

# ticat ${meta} : ${eds}\
#     : join.new algo 2 \
#     : join.new num_last_arr 2 \
#     : join.new time_window 200 \
#     : join.new num_online_clusters 1000 \
#     : join.new radius 10 \
#     : join.new buf_size 1500 \
#     : join.run run.sesame

# ticat ${meta} : ${eds}\
#     : join.new algo 3 \
#     : join.new buf_size 500 \
#     : join.new min_points 30 \
#     : join.new epsilon 200 \
#     : join.new base 2 \
#     : join.new lambda 0.25 \
#     : join.new mu 6 \
#     : join.new beta 0.2 \
#     : join.run run.sesame

# ticat ${meta} : ${eds}\
#     : join.new algo 4 \
#     : join.new lambda 0.01 \
#     : join.new radius 100 \
#     : join.new clean_interval 400 \
#     : join.new min_weight 0.001 \
#     : join.new alpha 0.02 \
#     : join.new base 2 \
#     : join.run run.sesame

# ticat ${meta} : ${eds}\
#     : join.new algo 5 \
#     : join.new radius 0.5 \
#     : join.new delta 2500 \
#     : join.new beta 1 \
#     : join.new buf_size 5000 \
#     : join.new alpha 0.998 \
#     : join.new lambda 1 \
#     : join.run run.sesame

# ticat ${meta} : ${eds}\
#     : join.new algo 6 \
#     : join.new lambda 0.998 \
#     : join.new beta 0.1 \
#     : join.new cm 3 \
#     : join.new cl 1 \
#     : join.new grid_width 50 \
#     : join.run run.sesame

# ticat ${meta} : ${eds}\
#     : join.new algo 7 \
#     : join.new delta_grid 1 \
#     : join.new num_samples 100 \
#     : join.new sliding 10 \
#     : join.run run.sesame

ticat ${meta} : ${eds}\
    : join.new algo 21 \
    : join.new max_in_nodes 500 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark 10000 \
    : join.new outlier_distance_threshold 500 \
    : join.new outlier_cap 5 \
    : join.new k 10000 \
    : join.run run.sesame

ticat ${meta} : ${eds}\
    : join.new algo 22 \
    : join.new max_in_nodes 500 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark 10000 \
    : join.new outlier_distance_threshold 500 \
    : join.new outlier_cap 5 \
    : join.new min_points 10 \
    : join.new epsilon 10 \
    : join.run run.sesame

ticat ${meta} : ${eds}\
    : join.new algo 23 \
    : join.new max_in_nodes 500 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark 1000 \
    : join.new outlier_distance_threshold 500 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${eds}\
    : join.new algo 24 \
    : join.new max_in_nodes 100 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new sliding 20000 \
    : join.new outlier_distance_threshold 15 \
    : join.new outlier_cap 500 \
    : join.run run.sesame

ticat ${meta} : ${eds}\
    : join.new algo 25 \
    : join.new max_in_nodes 50 \
    : join.new max_leaf_nodes 50 \
    : join.new distance_threshold 40 \
    : join.new outlier_distance_threshold 60 \
    : join.new outlier_cap 5 \
    : join.new lambda 0.5 \
    : join.new alpha 1.1 \
    : join.new buf_size 10000 \
    : join.run run.sesame

ticat ${meta} : ${eds}\
    : join.new algo 26 \
    : join.new max_in_nodes 500 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark 10000 \
    : join.new outlier_distance_threshold 500 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${eds}\
    : join.new algo 27 \
    : join.new max_in_nodes 500 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark 1000 \
    : join.new outlier_density_threshold 500 \
    : join.new neighbor_distance 2000 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${eds}\
    : join.new algo 28 \
    : join.new distance_threshold 200 \
    : join.new landmark 1000 \
    : join.new outlier_distance_threshold 300 \
    : join.new outlier_cap 10 \
    : join.run run.sesame

ticat ${meta} : ${eds}\
    : join.new algo 29 \
    : join.new beta 0.1 \
    : join.new cm 10 \
    : join.new cl 0.2 \
    : join.new outlier_distance_threshold 15 \
    : join.new outlier_cap 5 \
    : join.new grid_width 50 \
    : join.new landmark 1000 \
    : join.run run.sesame

ticat ${meta} : ${eds} \
    : join.new algo 30 \
    : join.new radius 5 \
    : join.new delta 1500 \
    : join.new beta 1 \
    : join.new buf_size 1000 \
    : join.new alpha 0.998 \
    : join.new lambda 1 \
    : join.new landmark 5000 \
    : join.run run.sesame

ticat ${meta} : ${eds}\
    : join.new algo 31,32,33 \
    : join.new max_in_nodes 500 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark 10000 \
    : join.new outlier_distance_threshold 500 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${eds} \
    : join.new algo 34 \
    : join.new delta_grid 1 \
    : join.new num_samples 2000 \
    : join.new sliding 10 \
    : join.new k 5000 \
    : join.new landmark 20000 \
    : join.run run.sesame

ticat ${meta} : ${eds} \
    : join.new algo 35 \
    : join.new seed 1 \
    : join.new landmark 10000 \
    : join.new coreset_size 50 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

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

export eds="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 245270 : join.new dim 2 : join.new num_clusters 363 : join.new run_eval false : join.new arr_rate 25000,50000,100000,200000"
run

