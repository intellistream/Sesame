# Sesame

This project aims at building a scalable stream mining library on modern hardware. 

- The repo contains currently several representative stream clustering algorithms.
- The code is still under active development and more algorithms will be introduced.
- We welcome your contributions, if you are interested to contribute to the project, please fork and submit a PR. If you have questions, feel free to log an issue or write an email to me: shuhao_zhang AT sutd.edu.sg

| Algorithm  | Window Model                 | Outlier Detection | Data Structure  | Offline Refinement |
| ---------- | ---------------------------- | ----------------- | --------------- | ------------------ |
| V1 | Landmark             | Distance  | CF-Tree | KMeans++   |
| V2 | Landmark             | Distance  | CF-Tree | DBSCAN     |
| V3 | Landmark             | Distance  | CF-Tree | No         |
| V4 | Sliding(Count-Based) | Distance  | CF-Tree | No         |
| V5 | Damped               | Distance  | CF-Tree | No         |
| V6 | Landmark             | No        |CF-Tree  | No         |
| V7 | Landmark             | Density   | CF-Tree | No         |
| V8 | Landmark             | Distance  | Micro-Cluster | No         |
| V9     | Landmark                 | Distance      | Grid          | No             |



# Algorithm Evaluation

### DataSet

| DataSet   | Length  | Dimension | Cluster Number |
| --------- | ------- | --------- | -------------- |
| CoverType | 581012  | 54        | 7              |
| KDD-99    | 494021  | 43        | 23             |
| Sensor    | 2219803 | 5         | 54             |
|           |         |           |                |
|           |         |           |                |
|           |         |           |                |

### Parameter Range

##### (1) BIRCH

| Parameter         | CoverType      | KDD-99         | Sensor         |
| ----------------- | -------------- | -------------- | -------------- |
| max_in_nodes      | [10,10,400]    | [100,100,4000] | [100,100,4000] |
| max_leaf_nodes    | [10,10,400]    | [100,100,4000] | [100,100,4000] |
| distance_threshold| [100,100,2000] | [100,100,2000] | [10,5,60]      |

##### (2) StreamKM++

| Parameter     | CoverType      | KDD-99           | Sensor           |
| ------------- | -------------- | ---------------- | ---------------- |
| seed          | 1,10,100       | 1,10,100         | 1,10,100         |
| num_clusters  | 7              | 23               | 54               |
| corest_size   | [100,100,5000] | [1000,100,10000] | [1000,100,10000] |

##### (3) EDMStream

| Parameter  | CoverType      | KDD-99            | Sensor            |
| ---------- | -------------- | ----------------- | ----------------- |
| radius     | [50,50,1000]   | [10,5,500]        | [0.1,0.1,5]       |
| delta      | [100,100,2000] | [100,100,2000]    | [10,10,500]       |
| beta       | [0.1,0.1,1]    | [0.1,0.1,1]       | [0.1,0.1,1]       |
| buf_size   | [100,100,2000] | [1000,1000,10000] | [1000,1000,10000] |
| alpha      | 0.998          | 0.998             | 0.998             |
| lambda     | 1              | 1                 | 1                 |

##### (4) DBStream

| Parameter       | CoverType             | KDD-99       | Sensor |
| --------------- | --------------------- | ------------ | ------ |
| lambda          | {0.25,0.125}          | {0.25,0.125} | 0.998  |
| radius          | 300                   | [80,10, 200] | 2      |
| clean_interval | 400                   | 500          | 1500   |
| min_weight       | 0.5                   | 0.5          | 0.25   |
| alpha           | {0.2,0.3,0.4,0.5,0.6} | 0.4          | 0.3    |
| base            | 2                     | 2            | 2      |

##### (5) DStream

| Parameter | CoverType       | KDD-99                    | Sensor          |
| ---------  | --------------- | ------------------------- | --------------- |
| lambda     | {0.998, 0.999}  | {0.998,0.999}             | 0.998           |
| beta       | 0.001, 0.3      | {0.001,0.3}               | 0.001           |
| cm         | [5,5,20]        | {1.001, 3}                | 4               |
| cl         | {0.8,1.0,0.001} | {0.7,0.8,0.9,0.999,1.001} | 2               |
| grid_width | [12,1,15]       | [4,1,8]                   | [0.15,0.05,0.5] |

##### (6) DenStream

| Parameter      | CoverType      | KDD-99         | Sensor         |
| -------------- | -------------- | -------------- | -------------- |
| buf_size       | [500,500,2000] | [500,500,2000] | [500,500,1500] |
| min_points     | [10,10,50]     | [10,10,50]     | 5              |
| epsilon        | [20,5,45]      | [25,5,45]      | 0.8            |
| base           | 2              | 2              | 2              |
| lambda         | {0.25,0.125}   | {0.25,0.125}   | {0.25,0.125}   |
| mu             | [2,1,10]       | [2,1,10]       | 6              |
| beta           | 0.25           | 0.25           | 0.25           |

##### (7) Clustream

| Parameter            | CoverType      | KDD-99         | Sensor       |
| -------------------- | -------------- | -------------- | ------------ |
| num_last_arr      | [2,1,8]        | [2,1,16]       | [2,1,15]     |
| time_window           | [200,50,350]   | [200,50,350]   | 1000         |
| time_interval         | random         | random         | random       |
| num_offline_clusters | 7              | 23             | 54           |
| num_clusters        | [80,10,150]    | [60,10,150]    | [100,10,200] |
| radius         | [2,1,26]       | [2,1,8]        | 15           |
| buf_size           | [500,500,1500] | [500,500,1500] | 500          |
| offline_time_window    | random         | random         | random       |

##### (8) SL-KMeans

| Parameter            | CoverType      | KDD-99         | Sensor       |
| -------------------- | -------------- | -------------- | ------------ |
| delta_grid           | {0.2, 0.5, 0.7}        | {0.2, 0.5, 0.7}       | {0.2, 0.5, 0.7}     |
| num_samples          | {100, 500, 1000}   | {100, 500, 1000}   | {100, 500, 1000}         |

## How to Cite Sesame

* **[xxx]** We have an anonymized submission under review. Stay tuned.
