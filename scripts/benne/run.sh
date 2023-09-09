$meta_cmd -e "CREATE OR REPLACE VIEW benne AS SELECT *, sum_us/1000000 as sum_s, win_us/1000000 as win_s, ds_us/1000000 as ds_s, out_us/1000000 as out_s, ref_us/1000000 as off_s FROM sesame WHERE bench_id=${bench_id};"

sql_files=`ls sql/`
for sql in ${sql_files[@]}; do
    echo "query $sql..."
    $meta_cmd <sql/$sql 2>/dev/null | sed 's/\t/,/g' > raw/${sql%.*}.csv
done