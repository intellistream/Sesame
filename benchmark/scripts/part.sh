#!/bin/bash

export cover="join.new input_file /home/shaun/Sesame/build/benchmark/datasets/CoverType.txt : join.new num_points 100000 : join.new dim 54 : join.new num_clusters 7"
export kdd99="join.new input_file /home/shaun/Sesame/build/benchmark/datasets/KDD99.txt : join.new num_points 100000 : join.new dim 41 : join.new num_clusters 23"
export sensor="join.new input_file /home/shaun/Sesame/build/benchmark/datasets/sensor.txt : join.new num_points 100000 : join.new dim 5 : join.new num_clusters 55"
export eds="join.new input_file /home/shaun/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 245270 : join.new dim 2 : join.new num_clusters 363"
export edso="join.new input_file /home/shaun/Sesame/build/benchmark/datasets/EDS_O.txt : join.new num_points 100000 : join.new dim 2 : join.new num_clusters 90"

function SEQ()
{
    seq $1 $2 $3 | sed 's/,/./g'
}
