#!/bin/bash

meta -e "DROP VIEW IF EXISTS temp;"
meta -e "CREATE VIEW temp as SELECT * FROM sesame WHERE run_begin>=FROM_UNIXTIME($bench_begin);"

sql_files=`ls sql/`
for sql in ${sql_files[@]}; do
    meta <sql/$sql 2>/dev/null | sed 's/\t/,/g' > raw/${sql%.*}.csv
done

./draw.py
