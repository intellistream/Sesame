#!/bin/bash

prof_files=`ls /tmp/*.prof`
for prof in ${prof_files[@]}; do
    base=`basename $prof .prof`
    algo="${base##*.}"
    workload="${base%.*}"
    res=`pprof --text $(which benchmark) $prof 2>/dev/null | grep 'L1Dist\|L2Dist\|setFeatureItem\|getFeatureItem' | (echo 0; cat -) | awk '{ printf "%f\n", $2 }' | awk '{s+=$1} END {print s}'`
    echo $algo,$workload,$res
done
