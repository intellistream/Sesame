#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/all.sh"

export cover="join.new input_file $HOME/Sesame/build/benchmark/datasets/CoverType.txt : join.new num_points 581012 : join.new dim 54 : join.new num_clusters 7"
export kdd99="join.new input_file $HOME/Sesame/build/benchmark/datasets/KDD99.txt : join.new num_points 494021 : join.new dim 41 : join.new num_clusters 23"
export sensor="join.new input_file $HOME/Sesame/build/benchmark/datasets/sensor.txt : join.new num_points 1000000 : join.new dim 5 : join.new num_clusters 55"

ticat ${meta} : ${cover} \
    : join.new algo 1 \
    : join.new seed 1 \
    : join.new coreset_size 5000 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 1 \
    : join.new seed 100 \
    : join.new coreset_size 10000 \
    : join.run run.sesame

ticat ${meta} : ${sensor} \
    : join.new algo 1 \
    : join.new seed 100 \
    : join.new coreset_size 10000 \
    : join.run run.sesame

# ticat ${meta} : ${insects} \
#     : join.new algo 1 \
#     : join.new seed 100 \
#     : join.new coreset_size 10000 \
#     : join.run run.sesame

# ticat ${meta} : ${eds} \
#     : join.new algo 1 \
#     : join.new seed 10 \
#     : join.new coreset_size 5000 \
#     : join.run run.sesame

# ticat ${meta} : ${edso} \
#     : join.new algo 1 \
#     : join.new seed 10 \
#     : join.new coreset_size 5000 \
#     : join.run run.sesame
