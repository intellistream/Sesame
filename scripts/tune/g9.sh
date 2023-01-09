#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/part.sh"

ticat ${meta} : ${cover} \
    : join.new algo 29 \
    : join.new beta 0.001,0.3 \
    : join.new cm [5,20,5] \
    : join.new cl 0.8,1.0,0.001 \
    : join.new landmark 1000,10000,20000 \
    : join.new outlier_distance_threshold 1000 \
    : join.new outlier_cap 100,300,500 \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 29 \
    : join.new beta 0.001,0.3 \
    : join.new cm 1.001,3 \
    : join.new cl 0.7,0.8,0.9,0.999,0.1001 \
    : join.new landmark 1000,10000,20000 \
    : join.new outlier_distance_threshold 1000 \
    : join.new outlier_cap 100,300,500 \
    : join.run run.sesame

ticat ${meta} : ${sensor} \
    : join.new algo 29 \
    : join.new beta 0.001 \
    : join.new cm 4 \
    : join.new cl 2 \
    : join.new landmark 1000,10000,20000 \
    : join.new outlier_distance_threshold 0.1,0.5,100 \
    : join.new outlier_cap 100 \
    : join.run run.sesame

ticat ${meta} : ${eds} \
    : join.new algo 29 \
    : join.new beta 0.001,0.002 \
    : join.new cm 0.0005,0.001,0.002 \
    : join.new cl 0.0005,0.001,0.002 \
    : join.new landmark 1000,10000,20000 \
    : join.new outlier_distance_threshold 0.002,0.003,0.005 \
    : join.new outlier_cap 100 \
    : join.run run.sesame

ticat ${meta} : ${edso} \
    : join.new algo 29 \
    : join.new beta 0.001,0.3 \
    : join.new cm 4\
    : join.new cl 2 \
    : join.new landmark 1000,10000,20000 \
    : join.new outlier_distance_threshold 10,15 \
    : join.new outlier_cap 100,300,500 \
    : join.run run.sesame
