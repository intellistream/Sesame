ori_meta=$meta

export cover="join.new input_file $HOME/Sesame/build/benchmark/datasets/CoverType.txt : join.new num_points 581012 : join.new dim 54 : join.new num_clusters 7 : join.new run_cmm false"
export kdd99="join.new input_file $HOME/Sesame/build/benchmark/datasets/KDD99_F.txt : join.new num_points 4898431 : join.new dim 41 : join.new num_clusters 23 : join.new run_cmm false"
export sensor="join.new input_file $HOME/Sesame/build/benchmark/datasets/sensor.txt : join.new num_points 2219803 : join.new dim 5 : join.new num_clusters 55 : join.new run_cmm false : join.new arr_rate 0"
export insects="join.new input_file $HOME/Sesame/build/benchmark/datasets/INSECTS.txt : join.new num_points 905145 : join.new dim 33 : join.new num_clusters 24 : join.new run_cmm false : join.new arr_rate 0"
export eds="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 245270 : join.new dim 2 : join.new num_clusters 363 : join.new run_cmm false"

meta="{bench.tag=extra.outlier_cap} $ori_meta"

ticat ${meta} : ${eds}\
    : join.new algo 23 \
    : join.new max_in_nodes 50 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark 1000 \
    : join.new outlier_distance_threshold 500 \
    : join.new outlier_cap 100,200,500,1000,2000,5000,10000,20000,40000,60000,80000,100000 \
    : join.run run.sesame

ticat ${meta} : ${eds}\
    : join.new algo 27 \
    : join.new max_in_nodes 100 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark 1000 \
    : join.new outlier_density_threshold 100 \
    : join.new neighbor_distance 500 \
    : join.new outlier_cap 100,200,500,1000,2000,5000,10000,20000,40000,60000,80000,100000 \
    : join.run run.sesame

meta="{bench.tag=extra.landmark} $ori_meta"

ticat ${meta} : ${cover} \
    : join.new algo 23 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 600 \
    : join.new landmark [50000,500000,50000] \
    : join.new outlier_distance_threshold 1000 \
    : join.new outlier_cap 500 \
    : join.run run.sesame

meta="{bench.tag=extra.sliding} $ori_meta"

ticat ${meta} : ${cover} \
    : join.new algo 24 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 600 \
    : join.new sliding [50000,500000,50000]  \
    : join.new outlier_distance_threshold 1000 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

meta="{bench.tag=extra.damped} $ori_meta"

ticat ${meta} : ${cover} \
    : join.new algo 25 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 600 \
    : join.new outlier_distance_threshold 1000 \
    : join.new outlier_cap 5 \
    : join.new lambda [0.2,3,0.2] \
    : join.new alpha 1.1 \
    : join.new buf_size 4000 \
    : join.run run.sesame

ticat ${meta} : ${cover} \
    : join.new algo 25 \
    : join.new max_in_nodes 400 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 600 \
    : join.new outlier_distance_threshold 1000 \
    : join.new outlier_cap 5 \
    : join.new lambda 2 \
    : join.new alpha [1.1,2.9,0.2] \
    : join.new buf_size 4000 \
    : join.run run.sesame

meta="{bench.tag=extra.offline} $ori_meta"

ticat ${meta} : ${cover} \
    : join.new algo 1 \
    : join.new seed 1 \
    : join.new coreset_size 7 \
    : join.new k 70 \
    : join.new run_offline true,false \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 1 \
    : join.new seed 100 \
    : join.new coreset_size 23 \
    : join.new k 454 \
    : join.new run_offline true,false \
    : join.run run.sesame

ticat ${meta} : ${sensor} \
    : join.new algo 1 \
    : join.new seed 100 \
    : join.new coreset_size 55 \
    : join.new k 1399 \
    : join.new run_offline true,false \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 1 \
    : join.new seed 1 \
    : join.new coreset_size 24 \
    : join.new k 102 \
    : join.new run_offline true,false \
    : join.run run.sesame

ticat ${meta} : ${cover} \
    : join.new algo 3 \
    : join.new buf_size 500 \
    : join.new min_points 30 \
    : join.new epsilon 35 \
    : join.new base 2 \
    : join.new lambda 0.25 \
    : join.new mu 6 \
    : join.new beta 0.2 \
    : join.new run_offline true,false \
    : join.run run.sesame

ticat ${meta} : ${kdd99} \
    : join.new algo 3 \
    : join.new buf_size 500 \
    : join.new min_points 30 \
    : join.new epsilon 35 \
    : join.new base 2 \
    : join.new lambda 0.25 \
    : join.new mu 6 \
    : join.new beta 0.2 \
    : join.new run_offline true,false \
    : join.run run.sesame

ticat ${meta} : ${sensor} \
    : join.new algo 3 \
    : join.new buf_size 500 \
    : join.new min_points 5 \
    : join.new epsilon 0.8 \
    : join.new base 2 \
    : join.new lambda 0.25 \
    : join.new mu 6 \
    : join.new beta 0.2 \
    : join.new run_offline true,false \
    : join.run run.sesame

ticat ${meta} : ${insects} \
    : join.new algo 3 \
    : join.new buf_size 1000 \
    : join.new min_points 30 \
    : join.new epsilon 0.1 \
    : join.new base 2 \
    : join.new lambda 0.25 \
    : join.new mu 6 \
    : join.new beta 0.6 \
    : join.new run_offline true,false \
    : join.run run.sesame

meta="{bench.tag=extra.dstream} $ori_meta"

ticat ${meta} \
    : join.new input_file $HOME/Sesame/build/benchmark/datasets/CoverType.txt \
    : join.new num_points [50000,500000,50000] \
    : join.new dim 54 \
    : join.new num_clusters 7 \
    : join.new run_cmm false \
    : join.new algo 6 \
    : join.new lambda 0.998 \
    : join.new beta 0.1 \
    : join.new cm 5 \
    : join.new cl 0.8 \
    : join.new grid_width 50 \
    : join.run run.sesame

meta="{bench.tag=extra.step} $ori_meta"
eds="join.new input_file $HOME/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points [20000,240000,20000] : join.new dim 2 : join.new num_clusters 363 : join.new run_eval false"

ticat ${meta} : ${eds}\
    : join.new algo 23 \
    : join.new max_in_nodes 500 \
    : join.new max_leaf_nodes 100 \
    : join.new distance_threshold 100 \
    : join.new landmark 10000 \
    : join.new outlier_distance_threshold 500 \
    : join.new outlier_cap 5 \
    : join.run run.sesame

ticat ${meta} : ${eds}\
    : join.new algo 24 \
    : join.new max_in_nodes 50 \
    : join.new max_leaf_nodes 10 \
    : join.new distance_threshold 10 \
    : join.new sliding 20000 \
    : join.new outlier_distance_threshold 15 \
    : join.new outlier_cap 500 \
    : join.run run.sesame

ticat ${meta} : ${eds}\
    : join.new algo 25 \
    : join.new max_in_nodes 50 \
    : join.new max_leaf_nodes 50 \
    : join.new distance_threshold 40 \
    : join.new outlier_distance_threshold 60 \
    : join.new outlier_cap 5 \
    : join.new lambda 0.5 \
    : join.new alpha 1.1 \
    : join.new buf_size 10000 \
    : join.run run.sesame
