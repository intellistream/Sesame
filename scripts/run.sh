#!/bin/bash

. $HOME/.bashrc
cd $(dirname ${BASH_SOURCE[0]})

export bench_begin=`date +"%Y-%m-%d %H:%M:%S"`

# export bench_begin='2022-09-23 16:54:37'

sh_files=`ls bench/`
for sh in ${sh_files[@]}; do
    echo "running $sh..."
    sleep 1
    bash <bench/$sh >>run.log 2>&1
done

export bench_end=`date +"%Y-%m-%d %H:%M:%S"`

# export bench_end='2022-09-23 19:47:28'

$meta_cmd -e "CREATE OR REPLACE VIEW perf AS SELECT id, run_begin, algo_id, algo, workload, num_points, num_res, arr_rate, et_s, purity, cmm, qps, lat_us, sum_us/1000000 as sum_s, win_us/1000000 as win_s, ds_us/1000000 as ds_s, out_us/1000000 as out_s, ref_us/1000000 as off_s, on_20, on_40, on_60, on_80, on_100 FROM sesame;"
$meta_cmd -e "DROP VIEW IF EXISTS temp;"
$meta_cmd -e "CREATE VIEW temp as SELECT * FROM perf WHERE run_begin>=\"$bench_begin\" AND run_begin<=\"$bench_end\";"

sql_files=`ls sql/`
for sql in ${sql_files[@]}; do
    echo "query $sql..."
    $meta_cmd <sql/$sql 2>/dev/null | sed 's/\t/,/g' > raw/${sql%.*}.csv
done

./draw.py
