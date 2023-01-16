#!/bin/bash

# . "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/part.sh"

ticat ${meta} : ${cover} \
    : join.new algo 6 \
    : join.new lambda 0.998 \
    : join.new beta 0.1,0.2,0.3 \
    : join.new cm [5,20,5] \
    : join.new cl [0.4,1,0.2] \
    : join.new grid_width [20,200,20] \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 6 \
    : join.new lambda 0.998 \
    : join.new beta 0.1,0.2,0.3 \
    : join.new cm [5,20,5] \
    : join.new cl [0.4,1,0.2] \
    : join.new grid_width [1000,10000,1000] \
    : join.run run.sesame


ticat ${meta} : ${sensor} \
    : join.new algo 6 \
    : join.new lambda 0.998 \
    : join.new beta 0.1,0.2,0.3 \
    : join.new cm [3,18,3] \
    : join.new cl [0.2,1,0.2] \
    : join.new grid_width [20,200,20] \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 6 \
    : join.new lambda 0.998 \
    : join.new beta 0.1,0.2,0.3 \
    : join.new cm [5,20,5] \
    : join.new cl [0.4,1,0.2] \
    : join.new grid_width [20,200,20] \
    : join.run run.sesame

ticat ${meta} : ${eds} \
    : join.new algo 6 \
    : join.new lambda 0.998 \
    : join.new beta 0.1,0.2,0.3 \
    : join.new cm [3,18,3] \
    : join.new cl [0.4,1,0.2] \
    : join.new grid_width [20,200,20] \
    : join.run run.sesame


ticat ${meta} : ${edso} \
    : join.new algo 6 \
    : join.new lambda 0.998 \
    : join.new beta 0.001,0.3 \
    : join.new cm [3,18,3] \
    : join.new cl [0.4,1,0.2] \
    : join.new grid_width [4,8,1] \
    : join.run run.sesame
