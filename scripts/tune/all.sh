#!/bin/bash

# export cover="join.new input_file /home/shaun/Sesame/build/benchmark/datasets/CoverType.txt : join.new num_points 581012 : join.new dim 54 : join.new num_clusters 7"
# export kdd99="join.new input_file /home/shaun/Sesame/build/benchmark/datasets/KDD99.txt : join.new num_points 494021 : join.new dim 41 : join.new num_clusters 23"
# export sensor="join.new input_file /home/shaun/Sesame/build/benchmark/datasets/sensor.txt : join.new num_points 1000000 : join.new dim 5 : join.new num_clusters 55"
# export insects="join.new input_file /home/shaun/Sesame/build/benchmark/datasets/INSECTS.txt : join.new num_points 905145 : join.new dim 33 : join.new num_clusters 24"

export cover="join.new input_file /home/shaun/Sesame/build/benchmark/datasets/CoverType.txt : join.new num_points 581012 : join.new dim 54 : join.new num_clusters 7 : join.new arr_rate 1000"
export kdd99="join.new input_file /home/shaun/Sesame/build/benchmark/datasets/KDD99.txt : join.new num_points 494021 : join.new dim 41 : join.new num_clusters 23 : join.new arr_rate 10000"
export sensor="join.new input_file /home/shaun/Sesame/build/benchmark/datasets/sensor.txt : join.new num_points 1000000 : join.new dim 5 : join.new num_clusters 55 : join.new arr_rate 100000"
export insects="join.new input_file /home/shuhao/Sesame/build/benchmark/datasets/INSECTS.txt : join.new num_points 905145 : join.new dim 33 : join.new num_clusters 24 : join.new arr_rate 50000"

export eds="join.new input_file /home/shaun/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 245270 : join.new dim 2 : join.new num_clusters 363"
export edso="join.new input_file /home/shaun/Sesame/build/benchmark/datasets/EDS_O.txt : join.new num_points 100000 : join.new dim 2 : join.new num_clusters 90"

export eds1="join.new input_file /home/shaun/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 45690 : join.new dim 2 : join.new num_clusters 75"
export eds2="join.new input_file /home/shaun/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 100270 : join.new dim 2 : join.new num_clusters 145"
export eds3="join.new input_file /home/shaun/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 150645 : join.new dim 2 : join.new num_clusters 218"
export eds4="join.new input_file /home/shaun/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 200060 : join.new dim 2 : join.new num_clusters 289"
export eds5="join.new input_file /home/shaun/Sesame/build/benchmark/datasets/EDS.txt : join.new num_points 245270 : join.new dim 2 : join.new num_clusters 363"

export edso1="join.new input_file /home/shaun/Sesame/build/benchmark/datasets/EDS_O.txt : join.new num_points 94270 : join.new dim 2 : join.new num_clusters 90"
export edso2="join.new input_file /home/shaun/Sesame/build/benchmark/datasets/EDS_O.txt : join.new num_points 97360 : join.new dim 2 : join.new num_clusters 90"
export edso3="join.new input_file /home/shaun/Sesame/build/benchmark/datasets/EDS_O.txt : join.new num_points 100000 : join.new dim 2 : join.new num_clusters 90"
