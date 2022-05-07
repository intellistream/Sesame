#!/bin/bash

. "`cd $(dirname ${BASH_SOURCE[0]}) && pwd`/part.sh"


ticat ${meta} : ${cover} \
    : join.new algo 1 \
    : join.new seed 1,10,100 \
    : join.new coreset_size [500,5000,500] \
    : join.run run.sesame

ticat ${meta} : ${eds} \
    : join.new algo 1 \
    : join.new seed 1,10,100 \
    : join.new coreset_size [500,5000,500] \
    : join.run run.sesame

ticat ${meta} : ${edso} \
    : join.new algo 1 \
    : join.new seed 1,10,100 \
    : join.new coreset_size [500,5000,500] \
    : join.run run.sesame


ticat ${meta} : ${kdd99} \
    : join.new algo 1 \
    : join.new seed 1,10,100 \
    : join.new coreset_size [1000,10000,1000] \
    : join.run run.sesame

ticat ${meta} : ${sensor} \
    : join.new algo 1 \
    : join.new seed 1,10,100 \
    : join.new coreset_size [1000,10000,1000] \
    : join.run run.sesame
