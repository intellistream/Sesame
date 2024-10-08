#!/bin/bash

set -e

export PATH=$PATH:${HOME}/Sesame/build/benchmark
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${HOME}/Sesame/build
cd $(dirname ${BASH_SOURCE[0]})

pwd
which ticat

export bench_id=2024

sh_files=$(ls script/)
for sh in ${sh_files[@]}; do
    echo "running $sh..."
    script/$sh 2>&1 | tee run-${bench_id}.log
done

export bench_end=`date +"%Y-%m-%d %H:%M:%S"`

$meta_cmd -e "CREATE OR REPLACE VIEW benne AS SELECT *, sum_us/1000000 as sum_s, win_us/1000000 as win_s, ds_us/1000000 as ds_s, out_us/1000000 as out_s, ref_us/1000000 as off_s FROM sesame WHERE bench_id=${bench_id};"

sql_files=`ls sql/`
for sql in ${sql_files[@]}; do
    echo "query $sql..."
    $meta_cmd <sql/$sql 2>/dev/null | sed 's/\t/,/g' > raw/${sql%.*}.csv
done

./draw.py