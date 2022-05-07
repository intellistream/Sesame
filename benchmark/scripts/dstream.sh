#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/part.sh"

ticat ${meta} : ${cover} \
    : join.new algo 6 \
    : join.new lambda 0.998,0.999 \
    : join.new beta 0.001,0.3 \
    : join.new cm [5,20,5] \
    : join.new cl 0.8,1.0,0.001 \
    : join.new grid_width [12,15,1] \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 6 \
    : join.new lambda 0.998,0.999 \
    : join.new beta 0.001,0.3 \
    : join.new cm 1.001,3 \
    : join.new cl 0.7,0.8,0.9,0.999,1.001 \
    : join.new grid_width [4,8,1] \
    : join.run run.sesame


ticat ${meta} : ${sensor} \
    : join.new algo 6 \
    : join.new lambda 0.998 \
    : join.new beta 0.001 \
    : join.new cm 4 \
    : join.new cl 2 \
    : join.new grid_width [0.15,0.5,0.05] \
    : join.run run.sesame



ticat ${meta} : ${eds} \
    : join.new algo 6 \
    : join.new lambda 0.998 \
    : join.new beta 0.001 0.3 \
    : join.new cm 4 \
    : join.new cl 2 \
    : join.new grid_width [4,8,1] \
    : join.run run.sesame


ticat ${meta} : ${edso} \
    : join.new algo 6 \
    : join.new lambda 0.998 \
    : join.new beta 0.001,0.3 \
    : join.new cm 4 \
    : join.new cl 2 \
    : join.new grid_width [4,8,1] \
    : join.run run.sesame
