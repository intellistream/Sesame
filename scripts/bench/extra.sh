ori_meta=$meta

meta="$ori_meta {bench.tag=extra.outlier_cap}"

export cover="join.new input_file $HOME/Sesame/build/benchmark/datasets/CoverType.txt : join.new num_points 581012 : join.new dim 54 : join.new num_clusters 7 : join.new run_cmm false"
export kdd99="join.new input_file $HOME/Sesame/build/benchmark/datasets/KDD99.txt : join.new num_points 494021 : join.new dim 41 : join.new num_clusters 23 : join.new run_cmm false"
export sensor="join.new input_file $HOME/Sesame/build/benchmark/datasets/sensor.txt : join.new num_points 2219803 : join.new dim 5 : join.new num_clusters 55 : join.new run_cmm false : join.new arr_rate 0"
export insects="join.new input_file $HOME/Sesame/build/benchmark/datasets/INSECTS.txt : join.new num_points 905145 : join.new dim 33 : join.new num_clusters 24 : join.new run_cmm false : join.new arr_rate 0"
export eds="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 245270 : join.new dim 2 : join.new num_clusters 363 : join.new run_cmm false"

ticat ${meta} : mark-time bench.begin : ${eds}\
    : join.new algo 23 \
    : join.new max_in_nodes 50 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark 1000 \
    : join.new outlier_distance_threshold 500 \
    : join.new outlier_cap 100,200,500,1000,2000,5000,10000,20000,40000,60000,80000,100000 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${eds}\
    : join.new algo 27 \
    : join.new max_in_nodes 100 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark 1000 \
    : join.new outlier_density_threshold 100 \
    : join.new neighbor_distance 500 \
    : join.new outlier_cap 100,200,500,1000,2000,5000,10000,20000,40000,60000,80000,100000 \
    : join.run run.sesame

meta="$ori_meta {bench.tag=extra.landmark}"

ticat ${meta} : mark-time bench.begin : ${eds}\
    : join.new algo 23 \
    : join.new max_in_nodes 50 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark 1000,2000,4000,6000,8000,16000,24000,32000,64000,128000 \
    : join.new outlier_distance_threshold 500 \
    : join.new outlier_cap 2000 \
    : join.run run.sesame

meta="$ori_meta {bench.tag=extra.sliding}"

ticat ${meta} : mark-time bench.begin : ${eds}\
    : join.new algo 24 \
    : join.new max_in_nodes 100 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new sliding 1000,2000,4000,8000,16000,20000,24000,32000,64000,128000 \
    : join.new outlier_distance_threshold 15 \
    : join.new outlier_cap 500 \
    : join.run run.sesame

meta="$ori_meta {bench.tag=extra.damped}"

ticat ${meta} : mark-time bench.begin : ${eds}\
    : join.new algo 25 \
    : join.new max_in_nodes 100 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark 1000 \
    : join.new outlier_distance_threshold 15 \
    : join.new outlier_cap 500 \
    : join.new lambda [0.2,2,0.2] \
    : join.new alpha 1.1 \
    : join.new buf_size 10000 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${eds}\
    : join.new algo 25 \
    : join.new max_in_nodes 100 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark 1000 \
    : join.new outlier_distance_threshold 15 \
    : join.new outlier_cap 500 \
    : join.new lambda 0.5 \
    : join.new alpha [1.1,2.9,0.2] \
    : join.new buf_size 10000 \
    : join.run run.sesame

meta="$ori_meta {bench.tag=extra.offline} join.new run_offline false,true"

ticat ${meta} : mark-time bench.begin : ${cover} \
    : join.new algo 1 \
    : join.new seed 1 \
    : join.new coreset_size 7 \
    : join.new k 70 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${kdd99} \
    : join.new algo 1 \
    : join.new seed 100 \
    : join.new coreset_size 23 \
    : join.new k 454 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${sensor} \
    : join.new algo 1 \
    : join.new seed 100 \
    : join.new coreset_size 55 \
    : join.new k 1399 \
    : join.run run.sesame

ticat ${meta} : mark-time bench.begin : ${insects} \
    : join.new algo 1 \
    : join.new seed 1 \
    : join.new coreset_size 24 \
    : join.new k 102 \
    : join.run run.sesame