#!/bin/bash

. $HOME/.bashrc
cd $(dirname ${BASH_SOURCE[0]})

export bench_begin=`date +"%Y-%m-%d %H:%M:%S"`

# export bench_begin='2022-09-30 00:08:09'

sh_files=`ls bench/`
for sh in ${sh_files[@]}; do
    echo "running $sh..."
    bash <bench/$sh >>run.log 2>&1
done

export bench_end=`date +"%Y-%m-%d %H:%M:%S"`

# export bench_end='2022-09-30 10:40:00'

$meta_cmd -e "CREATE OR REPLACE VIEW perf AS SELECT *, sum_us/1000000 as sum_s, win_us/1000000 as win_s, ds_us/1000000 as ds_s, out_us/1000000 as out_s, ref_us/1000000 as off_s FROM sesame;"
$meta_cmd -e "CREATE OR REPLACE VIEW main as SELECT * FROM perf WHERE run_begin>=\"$bench_begin\" AND run_begin<=\"$bench_end\" AND tag NOT LIKE \"extra%\";"
$meta_cmd -e "CREATE OR REPLACE VIEW extra as SELECT * FROM perf WHERE run_begin>=\"$bench_begin\" AND run_begin<=\"$bench_end\" AND tag LIKE \"extra%\";"

sql_files=`ls sql/`
for sql in ${sql_files[@]}; do
    echo "query $sql..."
    $meta_cmd <sql/$sql 2>/dev/null | sed 's/\t/,/g' > raw/${sql%.*}.csv
done

./draw.py
