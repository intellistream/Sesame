#!/bin/bash

export bench_begin=`date +"%Y-%m-%d %H:%M:%S"`
export bench_id=`date +"%H%M%S"`

. $HOME/.bashrc
cd $(dirname ${BASH_SOURCE[0]})
rm run.log

echo "bench_id=${bench_id}"

# export bench_begin='2022-09-30 17:19:10'

sh_files=`ls bench/`
for sh in ${sh_files[@]}; do
    echo "running $sh..."
    bash <bench/$sh >>run-${bench_id}.log 2>&1
done

export bench_end=`date +"%Y-%m-%d %H:%M:%S"`

# export bench_end='2022-10-01 00:45:13'

$meta_cmd -e "CREATE OR REPLACE VIEW perf AS SELECT *, sum_us/1000000 as sum_s, win_us/1000000 as win_s, ds_us/1000000 as ds_s, out_us/1000000 as out_s, ref_us/1000000 as off_s FROM sesame WHERE run_begin>=\"$bench_begin\" AND run_begin<=\"$bench_end\";"
$meta_cmd -e "CREATE OR REPLACE VIEW main as SELECT * FROM perf WHERE tag NOT LIKE \"extra%\";"
$meta_cmd -e "CREATE OR REPLACE VIEW extra as SELECT * FROM perf WHERE tag LIKE \"extra%\";"

sql_files=`ls sql/`
for sql in ${sql_files[@]}; do
    echo "query $sql..."
    $meta_cmd <sql/$sql 2>/dev/null | sed 's/\t/,/g' > raw/${sql%.*}.csv
done

./draw.py
