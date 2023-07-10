#!/bin/bash

export eds1="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 45690 : join.new dim 2 : join.new num_clusters 75"
export eds2="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 100270 : join.new dim 2 : join.new num_clusters 145"
export eds3="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 150645 : join.new dim 2 : join.new num_clusters 218"
export eds4="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 200060 : join.new dim 2 : join.new num_clusters 289"
export eds5="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 245270 : join.new dim 2 : join.new num_clusters 363"

export edso1="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS_O.txt : join.new num_points 94270 : join.new dim 2 : join.new num_clusters 90"
export edso2="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS_O.txt : join.new num_points 97360 : join.new dim 2 : join.new num_clusters 90"
export edso3="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS_O.txt : join.new num_points 100000 : join.new dim 2 : join.new num_clusters 90"

export balance=0
export accuracy=1
export efficiency=2

export OMP_NUM_THREADS=32

function run(){

ticat ${meta} : ${eds} \
    : join.new algo 8 \
    : join.new obj $balance \
    : join.new landmark 10000 \
    : join.new queue_size_threshold 250000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame

ticat ${meta} : ${eds} \
    : join.new algo 8 \
    : join.new obj $accuracy \
    : join.new distance_threshold 8 \
    : join.new queue_size_threshold 50000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 50.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame

ticat ${meta} : ${eds} \
    : join.new algo 8 \
    : join.new obj $efficiency \
    : join.new sliding 2000 \
    : join.new queue_size_threshold 80000 \
    : join.new dim_threshold 30 \
    : join.new variance_threshold 100.0 \
    : join.new outliers_num_threshold 200 \
    : join.new outliers_dist_threshold 50.0 \
    : join.run run.sesame
    
}

for i in $(seq 1 5)
do
    export eds=`eval echo \$\{eds$i\}`
    run
done

# for i in $(seq 1 3)
# do
#     export eds=`eval echo \$\{edso$i\}`
#     run
# done
