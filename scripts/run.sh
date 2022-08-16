#!/bin/bash

. $HOME/.bashrc
cd $(dirname ${BASH_SOURCE[0]})

export bench_begin=`date +%s`

sh_files=`ls bench/`
for sh in ${sh_files[@]}; do
    echo "running $sh..."
    sleep 1
    bash <bench/$sh 2>/dev/null
done

meta -e "CREATE VIEW IF NOT EXISTS perf AS SELECT id, run_begin, algo, workload, num_points, num_res, arr_rate, et_s, purity, cmm, qps, lat_us, sum_us/1000000 as sum_s, win_us/1000000 as win_s, ds_us/1000000 as ds_s, out_us/1000000 as out_s, ref_us/1000000 as off_s, on_20, on_40, on_60, on_80, on_100 FROM sesame;"
meta -e "DROP VIEW IF EXISTS temp;"
meta -e "CREATE VIEW temp as SELECT * FROM perf WHERE run_begin>=FROM_UNIXTIME($bench_begin);"

sql_files=`ls sql/`
for sql in ${sql_files[@]}; do
    meta <sql/$sql 2>/dev/null | sed 's/\t/,/g' > raw/${sql%.*}.csv
done

./draw.py
