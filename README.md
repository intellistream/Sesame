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

| DataSet   | Length                                         | Dimension | Cluster Number |
| --------- | ---------------------------------------------- | --------- | -------------- |
| CoverType | 581012                                         | 54        | 7              |
| KDD-99    | 494021                                         | 41        | 23             |
| Sensor    | 2219803                                        | 4         | 54             |
| EDS       | 245270 [45690, 100270, 150645, 200060, 245270] | 2         | 363            |
| EDS_O     | 100000 [94720,97360,100000]                    | 2         | 90             |

### Parameter Range

##### (1) BIRCH

Times:  64 + 64 + 64  + 80 + 80 = 352

| Parameter          | CoverType      | KDD-99         | Sensor         | EDS            | EDS_O          |
| ------------------ | -------------- | -------------- | -------------- | -------------- | -------------- |
| max_in_nodes       | [100,300,1000] | [100,300,1000] | [100,300,1000] | [100,300,1000] | [100,300,1000] |
| max_leaf_nodes     | [100,300,1000] | [100,300,1000] | [100,300,1000] | [100,300,1000] | [100,300,1000] |
| distance_threshold | [100,500,1600] | [100,500,1600] | [10,30,100]    | [20,20,100 ]   | [20,20,100 ]   |

##### (2) StreamKM++

Times: 30 + 30 + 30 + 30 + 30 = 150

| Parameter    | CoverType      | KDD-99            | Sensor            | EDS            | EDS_O          |
| ------------ | -------------- | ----------------- | ----------------- | -------------- | -------------- |
| seed         | 1,10,100       | 1,10,100          | 1,10,100          | 1,10,100       | 1,10,100       |
| num_clusters | 7              | 23                | 54                | 363            | 90             |
| coreset_size | [500,500,5000] | [1000,1000,10000] | [1000,1000,10000] | [500,500,5000] | [500,500,5000] |

##### (3) EDMStream

Times: 64 + 80 + 80 + 48 + 48 = 320

| Parameter | CoverType        | KDD-99            | Sensor            | EDS              | EDS_O            |
| --------- | ---------------- | ----------------- | ----------------- | ---------------- | ---------------- |
| radius    | [100,500,1600]   | [100,100,500]     | [0.2,0.7,3]       | {10,20,30}       | {10,20,30}       |
| delta     | 1500             | 1500              | 300               | 300              | 300              |
| beta      | [0.1,0.3, 1]     | [0.1,0.3,1]       | [0.1,0.3,1]       | [0.1,0.3,1]      | [0.1,0.3,1]      |
| buf_size  | [1000,1000,4000] | [1000,3000,10000] | [1000,3000,10000] | [1000,1000,4000] | [1000,1000,4000] |
| alpha     | 0.998            | 0.998             | 0.998             | 0.998            | 0.998            |
| lambda    | 1                | 1                 | 1                 | 1                | 1                |

##### (4) DBStream

Times: 10 + 14 + 1 + 15 + 15 = 55

| Parameter       | CoverType             | KDD-99       | Sensor | EDS | EDS_O |
| --------------- | --------------------- | ------------ | ------ | --------------- | --------------- |
| lambda          | {0.25,0.125}          | {0.25,0.125} | 0.998  | 0.998 | 0.998 |
| radius          | 300                   | [80,20, 200] | 2      | {10,20,30} | {10,20,30}            |
| clean_interval | 400                   | 500          | 1500   | 400 | 400 |
| min_weight       | 0.5                   | 0.5          | 0.25   | 0.5 | 0.5 |
| alpha           | {0.2,0.3,0.4,0.5,0.6} | 0.4          | 0.3    | {0.2,0.3,0.4,0.5,0.6} | {0.2,0.3,0.4,0.5,0.6} |
| base            | 2                     | 2            | 2      | 2 | 2 |

##### (5) DStream

Times: 192 + 200 + 8 + 10 + 10 = 420

| Parameter | CoverType       | KDD-99                    | Sensor          | EDS | EDS_O |
| ---------  | --------------- | ------------------------- | --------------- | ---------  | ---------  |
| lambda     | {0.998, 0.999}  | {0.998,0.999}             | 0.998           | 0.998 | 0.998 |
| beta       | {0.001,0.3} | {0.001,0.3}               | 0.001           | {0.001,0.3} | {0.001,0.3} |
| cm         | [5,5,20]        | {1.001, 3}                | 4               | 4 | 4 |
| cl         | {0.8,1.0,0.001} | {0.7,0.8,0.9,0.999,1.001} | 2               | 2 | 2 |
| grid_width | [12,1,15]       | [4,1,8]                   | [0.15,0.05,0.5] | [4,1,8] | [4,1,8] |

##### (6) DenStream

Times: 54 + 54 + 2 + 54 + 54 = 218

| Parameter  | CoverType    | KDD-99       | Sensor       | EDS          | EDS_O        |
| ---------- | ------------ | ------------ | ------------ | ------------ | ------------ |
| buf_size   | 1500         | 1500         | 1500         | 1500         | 1500         |
| min_points | {10,30,50}   | {10,30,50}   | 5            | {10,30,50}   | {10,30,50}   |
| epsilon    | [25,10,45]   | [25,10,45]   | 0.8          | {10,20,30}   | {10,20,30}   |
| base       | 2            | 2            | 2            | 2            | 2            |
| lambda     | {0.25,0.125} | {0.25,0.125} | {0.25,0.125} | {0.25,0.125} | {0.25,0.125} |
| mu         | [2,4,10]     | [2,4,10]     | 6            | [2,4,10]     | [2,4,10]     |
| beta       | 0.25         | 0.25         | 0.25         | 0.25         | 0.25         |

##### (7) CluStream

Times: 81 + 81 + 9 + 81 + 81 = 333

| Parameter           | CoverType     | KDD-99        | Sensor        | EDS           | EDS_O         |
| ------------------- | ------------- | ------------- | ------------- | ------------- | ------------- |
| num_last_arr        | {2,5,8}       | {2,9,16}      | {2,9,16}      | {2,9,16}      | {2,9,16}      |
| time_window         | {200,275,350} | {200,275,350} | 1000          | {200,275,350} | {200,275,350} |
| time_interval       | 100           | 100           | 100           | 100           | 100           |
| num_online_clusters | {80,110,140}  | {60,100,140}  | {100,150,200} | {80,110,140}  | {80,110,140}  |
| radius              | {2,14,26}     | {2,5,8}       | 15            | {10,20,30}    | {10,20,30}    |
| buf_size            | 1500          | 1500          | 500           | 1500          | 1500          |
| offline_time_window | 0             | 0             | 0             | 0             | 0             |

##### (8) SL-KMeans

Times: 9 + 9 + 9 + 9 + 9 = 45

| Parameter            | CoverType      | KDD-99         | Sensor       |
| -------------------- | -------------- | -------------- | ------------ |
| delta_grid           | {0.2, 0.5, 0.7}        | {0.2, 0.5, 0.7}       | {0.2, 0.5, 0.7}     |
| num_samples          | {100, 500, 1000}   | {100, 500, 1000}   | {100, 500, 1000}         |

### Flags

```javascript
flags = ["algo", "input_file", "num_points", "dim", "num_clusters", "max_in_nodes", "max_leaf_nodes", "distance_threshold", "seed", "coreset_size", "radius", "delta", "beta", "buf_size", "alpha", "lambda", "clean_interval", "min_weight", "base", "cm", "cl", "grid_width", "min_points", "epsilon", "mu", "num_last_arr", "time_window", "num_online_clusters", "delta_grid", "num_samples"];
```

## How to Cite Sesame

* **[xxx]** We have an anonymized submission under review. Stay tuned.
