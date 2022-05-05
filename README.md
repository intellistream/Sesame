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

### Evaluation Results
Here is the  parameter selection of every studied algorithms based on the best purity value:

##### 1. General Comparison

<u>(1) BIRCH</u>

| Metrics    | CoverType | KDD-99  | Sensor | EDS      | EDS_O   |
| ---------- | --------- | ------- | ------ | -------- | ------- |
| Purity     | 0.7898    | 0.99013 | 0.2545 | 0.877017 | 0.69688 |
| CMM        |           |         |        |          |         |
| Throughput |           |         |        |          |         |
| Latency    |           |         |        |          |         |
| ETB        |           |         |        |          |         |

<u>(2) StreamKM++</u>

| Metrics    | CoverType | KDD-99 | Sensor | EDS    | EDS_O  |
| ---------- | --------- | ------ | ------ | ------ | ------ |
| Purity     | 0.8942    | 0.9983 | 0.5526 | 0.9889 | 0.9766 |
| CMM        |           |        |        |        |        |
| Throughput |           |        |        |        |        |
| Latency    |           |        |        |        |        |
| ETB        |           |        |        |        |        |

<u>(3) EDMStream</u>

| Metrics    | CoverType | KDD-99 | Sensor | EDS    | EDS_O  |
| ---------- | --------- | ------ | ------ | ------ | ------ |
| Purity     | 0.6876    | 0.9629 | 0.1359 | 0.0313 | 0.3589 |
| CMM        |           |        |        |        |        |
| Throughput |           |        |        |        |        |
| Latency    |           |        |        |        |        |
| ETB        |           |        |        |        |        |

<u>(4) DBStream</u>

| Metrics    | CoverType | KDD-99 | Sensor | EDS    | EDS_O  |
| ---------- | --------- | ------ | ------ | ------ | ------ |
| Purity     | 0.6679    | 0.5624 | 0.0780 | 0.0384 | 0.0792 |
| CMM        |           |        |        |        |        |
| Throughput |           |        |        |        |        |
| Latency    |           |        |        |        |        |
| ETB        |           |        |        |        |        |

<u>(5) DStream</u>

| Metrics    | CoverType | KDD-99 | Sensor | EDS  | EDS_O |
| ---------- | --------- | ------ | ------ | ---- | ----- |
| Purity     |           |        |        |      |       |
| CMM        |           |        |        |      |       |
| Throughput |           |        |        |      |       |
| Latency    |           |        |        |      |       |
| ETB        |           |        |        |      |       |

<u>(6) DenStream</u>

| Metrics    | CoverType | KDD-99 | Sensor | EDS  | EDS_O |
| ---------- | --------- | ------ | ------ | ---- | ----- |
| Purity     |           |        |        |      |       |
| CMM        |           |        |        |      |       |
| Throughput |           |        |        |      |       |
| Latency    |           |        |        |      |       |
| ETB        |           |        |        |      |       |

<u>(7) CluStream</u>

| Metrics    | CoverType | KDD-99 | Sensor | EDS  | EDS_O |
| ---------- | --------- | ------ | ------ | ---- | ----- |
| Purity     | 0.7353    | 0.9771 | 0.0993 |      |       |
| CMM        |           |        |        |      |       |
| Throughput |           |        |        |      |       |
| Latency    |           |        |        |      |       |
| ETB        |           |        |        |      |       |

<u>(8) SL-KMeans</u>

| Metrics    | CoverType | KDD-99 | Sensor | EDS  | EDS_O |
| ---------- | --------- | ------ | ------ | ---- | ----- |
| Purity     |           |        |        |      |       |
| CMM        |           |        |        |      |       |
| Throughput |           |        |        |      |       |
| Latency    |           |        |        |      |       |
| ETB        |           |        |        |      |       |

##### 2. Design Aspect Study

To find the best parameters, we run every algorithm many times according to the following parameter testing ranges:

<details>
<summary> <big ><b>Ranges</b></big> </summary>
<b>1. General Comparison</b>

<u>(1) BIRCH </u>

| Parameter          | CoverType      | KDD-99         | Sensor         | EDS            | EDS_O          |
| ------------------ | -------------- | -------------- | -------------- | -------------- | -------------- |
| max_in_nodes       | [100,300,1000] | [100,300,1000] | [100,300,1000] | [100,300,1000] | [100,300,1000] |
| max_leaf_nodes     | [100,300,1000] | [100,300,1000] | [100,300,1000] | [100,300,1000] | [100,300,1000] |
| distance_threshold | [100,500,1600] | [100,500,1600] | [10,30,100]    | [20,20,100 ]   | [20,20,100 ]   |

<u>(2) StreamKM++ </u>

| Parameter    | CoverType      | KDD-99            | Sensor            | EDS            | EDS_O          |
| ------------ | -------------- | ----------------- | ----------------- | -------------- | -------------- |
| seed         | 1,10,100       | 1,10,100          | 1,10,100          | 1,10,100       | 1,10,100       |
| num_clusters | 7              | 23                | 54                | 363            | 90             |
| coreset_size | [500,500,5000] | [1000,1000,10000] | [1000,1000,10000] | [500,500,5000] | [500,500,5000] |

<u>(3) EDMStream </u>

| Parameter | CoverType        | KDD-99            | Sensor            | EDS              | EDS_O            |
| --------- | ---------------- | ----------------- | ----------------- | ---------------- | ---------------- |
| radius    | [100,500,1600]   | [100,100,500]     | [0.2,0.7,3]       | {10,20,30}       | {10,20,30}       |
| delta     | 1500             | 1500              | 300               | 300              | 300              |
| beta      | [0.1,0.3, 1]     | [0.1,0.3,1]       | [0.1,0.3,1]       | [0.1,0.3,1]      | [0.1,0.3,1]      |
| buf_size  | [1000,1000,4000] | [1000,3000,10000] | [1000,3000,10000] | [1000,1000,4000] | [1000,1000,4000] |
| alpha     | 0.998            | 0.998             | 0.998             | 0.998            | 0.998            |
| lambda    | 1                | 1                 | 1                 | 1                | 1                |

<u>(4) DBStream </u>

| Parameter       | CoverType             | KDD-99       | Sensor | EDS | EDS_O |
| --------------- | --------------------- | ------------ | ------ | --------------- | --------------- |
| lambda          | {0.25,0.125}          | {0.25,0.125} | 0.998  | 0.998 | 0.998 |
| radius          | 300                   | [80,20, 200] | 2      | {10,20,30} | {10,20,30}            |
| clean_interval | 400                   | 500          | 1500   | 400 | 400 |
| min_weight       | 0.5                   | 0.5          | 0.25   | 0.5 | 0.5 |
| alpha           | {0.2,0.3,0.4,0.5,0.6} | 0.4          | 0.3    | {0.2,0.3,0.4,0.5,0.6} | {0.2,0.3,0.4,0.5,0.6} |
| base            | 2                     | 2            | 2      | 2 | 2 |

<u>(5) DStream </u>

| Parameter | CoverType       | KDD-99                    | Sensor          | EDS | EDS_O |
| ---------  | --------------- | ------------------------- | --------------- | ---------  | ---------  |
| lambda     | {0.998, 0.999}  | {0.998,0.999}             | 0.998           | 0.998 | 0.998 |
| beta       | {0.001,0.3} | {0.001,0.3}               | 0.001           | {0.001,0.3} | {0.001,0.3} |
| cm         | [5,5,20]        | {1.001, 3}                | 4               | 4 | 4 |
| cl         | {0.8,1.0,0.001} | {0.7,0.8,0.9,0.999,1.001} | 2               | 2 | 2 |
| grid_width | [12,1,15]       | [4,1,8]                   | [0.15,0.05,0.5] | [4,1,8] | [4,1,8] |

<u>(6) DenStream</u>

| Parameter  | CoverType    | KDD-99       | Sensor       | EDS          | EDS_O        |
| ---------- | ------------ | ------------ | ------------ | ------------ | ------------ |
| buf_size   | 1500         | 1500         | 1500         | 1500         | 1500         |
| min_points | {10,30,50}   | {10,30,50}   | 5            | {10,30,50}   | {10,30,50}   |
| epsilon    | [25,10,45]   | [25,10,45]   | 0.8          | {10,20,30}   | {10,20,30}   |
| base       | 2            | 2            | 2            | 2            | 2            |
| lambda     | {0.25,0.125} | {0.25,0.125} | {0.25,0.125} | {0.25,0.125} | {0.25,0.125} |
| mu         | [2,4,10]     | [2,4,10]     | 6            | [2,4,10]     | [2,4,10]     |
| beta       | 0.25         | 0.25         | 0.25         | 0.25         | 0.25         |

<u>(7) CluStream</u>

| Parameter           | CoverType     | KDD-99        | Sensor        | EDS           | EDS_O         |
| ------------------- | ------------- | ------------- | ------------- | ------------- | ------------- |
| num_last_arr        | {2,5,8}       | {2,9,16}      | {2,9,16}      | {2,9,16}      | {2,9,16}      |
| time_window         | {200,275,350} | {200,275,350} | 1000          | {200,275,350} | {200,275,350} |
| time_interval       | 100           | 100           | 100           | 100           | 100           |
| num_online_clusters | {80,110,140}  | {60,100,140}  | {100,150,200} | {80,110,140}  | {80,110,140}  |
| radius              | {2,14,26}     | {2,5,8}       | 15            | {10,20,30}    | {10,20,30}    |
| buf_size            | 1500          | 1500          | 500           | 1500          | 1500          |
| offline_time_window | 0             | 0             | 0             | 0             | 0             |

<u>(8) SL-KMeans </u>

| Parameter            | CoverType      | KDD-99         | Sensor       |
| -------------------- | -------------- | -------------- | ------------ |
| delta_grid           | {0.2, 0.5, 0.7}        | {0.2, 0.5, 0.7}       | {0.2, 0.5, 0.7}     |
| num_samples          | {100, 500, 1000}   | {100, 500, 1000}   | {100, 500, 1000}         |

<b>2. Design Aspect Study</b>

<u>(1) V1 </u>

| Parameter                  | CoverType          | KDD-99              | Sensor              | EDS                 | EDS_O               |
| -------------------------- | ------------------ | ------------------- | ------------------- | ------------------- | ------------------- |
| max_in_nodes               | 400                | 400                 | 400                 | 100                 | 100                 |
| max_leaf_nodes             | 100                | 100                 | 100                 | 100                 | 100                 |
| distance_threshold         | 1600               | 100                 | 100                 | 100                 | 20                  |
| Landmark                   | {1000,10000,20000} | {1000,10000,20000 } | {1000,10000,20000 } | {1000,10000,20000 } | {1000,10000,20000 } |
| outlier_distance_threshold | 1000               | 1000                | 20                  | {10,15}             | {10,15}             |
| outlier_cap                | {100,300,500}      | {100,300,500}       | {100,300,500}       | {100,300,500}       | {100,300,500}       |
| seed                       | {1,10,100}         | {1,10,100}          | {1,10,100}          | {1,10,100}          | {1,10,100}          |

<u>(2) V2 </u>

| Parameter                  | CoverType          | KDD-99              | Sensor              | EDS                 | EDS_O               |
| -------------------------- | ------------------ | ------------------- | ------------------- | ------------------- | ------------------- |
| max_in_nodes               | 400                | 400                 | 400                 | 100                 | 100                 |
| max_leaf_nodes             | 100                | 100                 | 100                 | 100                 | 100                 |
| distance_threshold         | 1600               | 100                 | 100                 | 100                 | 20                  |
| landmark                   | {1000,10000,20000} | {1000,10000,20000 } | {1000,10000,20000 } | {1000,10000,20000 } | {1000,10000,20000 } |
| outlier_distance_threshold | 1000               | 1000                | 20                  | {10,15}             | {10,15}             |
| outlier_cap                | {100,300,500}      | {100,300,500}       | {100,300,500}       | {100,300,500}       | {100,300,500}       |
| min_points                 | 10                 | 10                  | 5                   | 10                  | 10                  |
| epsilon                    | 25                 | 25                  | 0.8                 | 10                  | 10                  |

<u>(3) V3 </u>

| Parameter                  | CoverType          | KDD-99              | Sensor              | EDS                 | EDS_O               |
| -------------------------- | ------------------ | ------------------- | ------------------- | ------------------- | ------------------- |
| max_in_nodes               | 400                | 400                 | 400                 | 100                 | 100                 |
| max_leaf_nodes             | 100                | 100                 | 100                 | 100                 | 100                 |
| distance_threshold         | 1600               | 100                 | 100                 | 100                 | 20                  |
| landmark                   | {1000,10000,20000} | {1000,10000,20000 } | {1000,10000,20000 } | {1000,10000,20000 } | {1000,10000,20000 } |
| outlier_distance_threshold | 1000               | 1000                | 20                  | {10,15}             | {10,15}             |
| outlier_cap                | {100,300,500}      | {100,300,500}       | {100,300,500}       | {100,300,500}       | {100,300,500}       |

<u>(4) V4 </u>

| Parameter                  | CoverType          | KDD-99              | Sensor              | EDS                 | EDS_O               |
| -------------------------- | ------------------ | ------------------- | ------------------- | ------------------- | ------------------- |
| max_in_nodes               | 400                | 400                 | 400                 | 100                 | 100                 |
| max_leaf_nodes             | 100                | 100                 | 100                 | 100                 | 100                 |
| distance_threshold         | 1600               | 100                 | 100                 | 100                 | 20                  |
| sliding                    | {1000,10000,20000} | {1000,10000,20000 } | {1000,10000,20000 } | {1000,10000,20000 } | {1000,10000,20000 } |
| outlier_distance_threshold | 1000               | 1000                | 20                  | {10,15}             | {10,15}             |
| outlier_cap                | {100,300,500}      | {100,300,500}       | {100,300,500}       | {100,300,500}       | {100,300,500}       |

<u>(5) V5 </u>

| Parameter                  | CoverType     | KDD-99        | Sensor        | EDS           | EDS_O         |
| -------------------------- | ------------- | ------------- | ------------- | ------------- | ------------- |
| max_in_nodes               | 400           | 400           | 400           | 100           | 100           |
| max_leaf_nodes             | 100           | 100           | 100           | 100           | 100           |
| distance_threshold         | 1600          | 100           | 100           | 100           | 20            |
| outlier_distance_threshold | 1000          | 1000          | 20            | {10,15}       | {10,15}       |
| outlier_cap                | {100,300,500} | {100,300,500} | {100,300,500} | {100,300,500} | {100,300,500} |
| lamda                      | {0.5,1.5,2}   | {0.5,1.5,2}   | {0.5,1.5,2}   | {0.5,1.5,2}   | {0.5,1.5,2}   |
| alpha                      | {1.1,1.5,2}   | {1.1,1.5,2}   | {1.1,1.5,2}   | {1.1,1.5,2}   | {1.1,1.5,2}   |

<u>(6) V6 </u>

| Parameter          | CoverType          | KDD-99             | Sensor             | EDS                | EDS_O              |
| ------------------ | ------------------ | ------------------ | ------------------ | ------------------ | ------------------ |
| max_in_nodes       | 400                | 400                | 400                | 100                | 100                |
| max_leaf_nodes     | 100                | 100                | 100                | 100                | 100                |
| distance_threshold | 1600               | 100                | 100                | 100                | 20                 |
| landmark           | {1000,10000,20000} | {1000,10000,20000} | {1000,10000,20000} | {1000,10000,20000} | {1000,10000,20000} |

<u>(7) V7 </u>

| Parameter                 | CoverType          | KDD-99             | Sensor             | EDS                | EDS_O              |
| ------------------------- | ------------------ | ------------------ | ------------------ | ------------------ | ------------------ |
| max_in_nodes              | 400                | 400                | 400                | 100                | 100                |
| max_leaf_nodes            | 100                | 100                | 100                | 100                | 100                |
| distance_threshold        | 1600               | 100                | 100                | 100                | 20                 |
| landmark                  | {1000,10000,20000} | {1000,10000,20000} | {1000,10000,20000} | {1000,10000,20000} | {1000,10000,20000} |
| outlier_density_threshold | {100,300,500}      | {100,300,500}      | {100,300,500}      | {100,300,500}      | {100,300,500}      |
| outlier_cap               | {100,300,500}      | {100,300,500}      | {100,300,500}      | {100,300,500}      | {100,300,500}      |
| neighbor_distance         | {2000,3000,5000}   | {200,300,500}      | {200,300,500}      | {200,300,500}      | {30,50}            |

<u>(8) V8 </u>

| Parameter                  | CoverType          | KDD-99              | Sensor              | EDS                 | EDS_O               |
| -------------------------- | ------------------ | ------------------- | ------------------- | ------------------- | ------------------- |
| distance_threshold         | 1600               | 100                 | 100                 | 100                 | 20                  |
| landmark                   | {1000,10000,20000} | {1000,10000,20000 } | {1000,10000,20000 } | {1000,10000,20000 } | {1000,10000,20000 } |
| outlier_distance_threshold | 1000               | 1000                | 20                  | {10,15}             | {10,15}             |
| outlier_cap                | {100,300,500}      | {100,300,500}       | {100,300,500}       | {100,300,500}       | {100,300,500}       |

<u>(9) V9 </u>

| Parameter                  | CoverType          | KDD-99                    | Sensor             | EDS                | EDS_O              |
| -------------------------- | ------------------ | ------------------------- | ------------------ | ------------------ | ------------------ |
| beta                       | {0.001,0.3}        | {0.001,0.3}               | 0.001              | {0.001,0.3}        | {0.001,0.3}        |
| cm                         | [5,5,20]           | {1.001, 3}                | 4                  | 4                  | 4                  |
| cl                         | {0.8,1.0,0.001}    | {0.7,0.8,0.9,0.999,1.001} | 2                  | 2                  | 2                  |
| landmark                   | {1000,10000,20000} | {1000,10000,20000}        | {1000,10000,20000} | {1000,10000,20000} | {1000,10000,20000} |
| outlier_distance_threshold | 1000               | 1000                      | 20                 | {10,15}            | {10,15}            |
| outlier_cap                | {100,300,500}      | {100,300,500}             | {100,300,500}      | {100,300,500}      | {100,300,500}      |


</details> 

The running results according to different parameter settings are shown as follows:

<details>

<summary> <big><b>Results</b></big> </summary>

<b>1. General Comparison</b>

<u>(1) BIRCH </u>

| id   | algo  | workload  | max_in_nodes | max_leaf_nodes | distance_threshold | cmm      | purity   |
| ---- | ----- | --------- | ------------ | -------------- | ------------------ | -------- | -------- |
| 79   | Birch | CoverType | 400          | 100            | 100                | 0.410019 | 0.7898   |
| 80   | Birch | CoverType | 400          | 100            | 600                | 0.410019 | 0.7898   |
| 81   | Birch | CoverType | 400          | 100            | 1100               | 0.410019 | 0.7898   |
| 82   | Birch | CoverType | 400          | 100            | 1600               | 0.410019 | 0.7898   |
| 2367 | Birch | EDS       | 100          | 100            | 20                 | 0.150215 | 0.877017 |
| 2368 | Birch | EDS       | 100          | 100            | 40                 | 0.150215 | 0.877017 |
| 2369 | Birch | EDS       | 100          | 100            | 60                 | 0.150215 | 0.877017 |
| 2370 | Birch | EDS       | 100          | 100            | 80                 | 0.150215 | 0.877017 |
| 2371 | Birch | EDS       | 100          | 100            | 100                | 0.150215 | 0.877017 |
| 279  | Birch | EDS_O     | 100          | 100            | 20                 | 0.354992 | 0.69688  |
| 374  | Birch | KDD99     | 400          | 100            | 100                | 0.591514 | 0.99013  |
| 231  | Birch | sensor    | 400          | 100            | 100                | 0.344888 | 0.2545   |

<u>(2) StreamKM++ </u>

| id   | algo         | workload  | tag       | workload | seed | num_clusters | coreset_size | cmm      | purity   |
| ---- | ------------ | --------- | --------- | -------- | ---- | ------------ | ------------ | -------- | -------- |
| 1456 | StreamKMeans | CoverType |           | CoverType| 1    |  7           | 5000         | 0.406903 | 0.89424  |
| 2074 | StreamKMeans | EDS       |           | EDS      | 10   | 363          | 5000         | 0.150501 | 0.988853 |
| 2204 | StreamKMeans | EDS_O     |           | EDS_O    | 10   | 90           | 5000         | 0.372093 | 0.97659  |
| 2336 | StreamKMeans | KDD99     |           | KDD99    | 100  | 23           | 10000        | 0.591537 | 0.99829  |
| 2366 | StreamKMeans | sensor    |           | sensor   | 100  | 54           | 10000        | 0.3484   | 0.55258  |

<u>(3) EDMStream </u>

| id   | algo      | workload  | workload  | radius | delta | beta | buf_size | alpha | lambda | cmm      | purity   |
| ---- | --------- | --------- | --------- | ------ | ----- | ---- | -------- | ----- | ------ | -------- | -------- |
| 1480 | EDMStream | CoverType | CoverType | 100    | 1500  | 1    | 2000     | 0.998 | 1      | 0.408466 | 0.68761  |
| 1481 | EDMStream | CoverType | CoverType | 100    | 1500  | 1    | 3000     | 0.998 | 1      | 0.408466 | 0.68761  |
| 1483 | EDMStream | CoverType | CoverType | 100    | 1500  | 1    | 4000     | 0.998 | 1      | 0.408466 | 0.68761  |
| 1478 | EDMStream | CoverType | CoverType | 100    | 1500  | 1    | 1000     | 0.998 | 1      | 0.408466 | 0.68761  |
| 2245 | EDMStream | EDS       | EDS       | 30     | 300   | 0.1  | 1000     | 0.998 | 1      | 0.150491 | 0.031272 |
| 2247 | EDMStream | EDS       | EDS       | 30     | 300   | 0.1  | 2000     | 0.998 | 1      | 0.150491 | 0.031272 |
| 2248 | EDMStream | EDS       | EDS       | 30     | 300   | 0.1  | 3000     | 0.998 | 1      | 0.150491 | 0.031272 |
| 2249 | EDMStream | EDS       | EDS       | 30     | 300   | 0.1  | 4000     | 0.998 | 1      | 0.150491 | 0.031272 |
| 2250 | EDMStream | EDS       | EDS       | 30     | 300   | 0.4  | 1000     | 0.998 | 1      | 0.150491 | 0.031272 |
| 2251 | EDMStream | EDS       | EDS       | 30     | 300   | 0.4  | 2000     | 0.998 | 1      | 0.150491 | 0.031272 |
| 2252 | EDMStream | EDS       | EDS       | 30     | 300   | 0.4  | 3000     | 0.998 | 1      | 0.150491 | 0.031272 |
| 2253 | EDMStream | EDS       | EDS       | 30     | 300   | 0.4  | 4000     | 0.998 | 1      | 0.150491 | 0.031272 |
| 2254 | EDMStream | EDS       | EDS       | 30     | 300   | 0.7  | 1000     | 0.998 | 1      | 0.150491 | 0.031272 |
| 2255 | EDMStream | EDS       | EDS       | 30     | 300   | 0.7  | 2000     | 0.998 | 1      | 0.150491 | 0.031272 |
| 2257 | EDMStream | EDS       | EDS       | 30     | 300   | 0.7  | 3000     | 0.998 | 1      | 0.150491 | 0.031272 |
| 2258 | EDMStream | EDS       | EDS       | 30     | 300   | 0.7  | 4000     | 0.998 | 1      | 0.150491 | 0.031272 |
| 2259 | EDMStream | EDS       | EDS       | 30     | 300   | 1    | 1000     | 0.998 | 1      | 0.150491 | 0.031272 |
| 2260 | EDMStream | EDS       | EDS       | 30     | 300   | 1    | 2000     | 0.998 | 1      | 0.150491 | 0.031272 |
| 2261 | EDMStream | EDS       | EDS       | 30     | 300   | 1    | 3000     | 0.998 | 1      | 0.150491 | 0.031272 |
| 2262 | EDMStream | EDS       | EDS       | 30     | 300   | 1    | 4000     | 0.998 | 1      | 0.150491 | 0.031272 |
| 2312 | EDMStream | EDS_O     | EDS_O     | 20     | 300   | 1    | 1000     | 0.998 | 1      | 0.347311 | 0.35894  |
| 2313 | EDMStream | EDS_O     | EDS_O     | 20     | 300   | 1    | 2000     | 0.998 | 1      | 0.347311 | 0.35894  |
| 2315 | EDMStream | EDS_O     | EDS_O     | 20     | 300   | 1    | 3000     | 0.998 | 1      | 0.347311 | 0.35894  |
| 2316 | EDMStream | EDS_O     | EDS_O     | 20     | 300   | 1    | 4000     | 0.998 | 1      | 0.347311 | 0.35894  |
| 1761 | EDMStream | KDD99     | KDD99     | 100    | 1500  | 0.7  | 1000     | 0.998 | 1      | 0.591539 | 0.96294  |
| 1816 | EDMStream | KDD99     | KDD99     | 100    | 1500  | 0.7  | 4000     | 0.998 | 1      | 0.591539 | 0.96294  |
| 1869 | EDMStream | KDD99     | KDD99     | 100    | 1500  | 0.7  | 7000     | 0.998 | 1      | 0.591539 | 0.96294  |
| 1923 | EDMStream | KDD99     | KDD99     | 100    | 1500  | 0.7  | 10000    | 0.998 | 1      | 0.591539 | 0.96294  |
| 2146 | EDMStream | sensor    | sensor    | 1.6    | 300   | 0.4  | 4000     | 0.998 | 1      | 0.356389 | 0.13586  |
| 2141 | EDMStream | sensor    | sensor    | 1.6    | 300   | 0.1  | 1000     | 0.998 | 1      | 0.356389 | 0.13586  |
| 2142 | EDMStream | sensor    | sensor    | 1.6    | 300   | 0.1  | 4000     | 0.998 | 1      | 0.356389 | 0.13586  |
| 2143 | EDMStream | sensor    | sensor    | 1.6    | 300   | 0.1  | 7000     | 0.998 | 1      | 0.356389 | 0.13586  |
| 2144 | EDMStream | sensor    | sensor    | 1.6    | 300   | 0.1  | 10000    | 0.998 | 1      | 0.356389 | 0.13586  |
| 2145 | EDMStream | sensor    | sensor    | 1.6    | 300   | 0.4  | 1000     | 0.998 | 1      | 0.356389 | 0.13586  |
| 2161 | EDMStream | sensor    | sensor    | 1.6    | 300   | 1    | 10000    | 0.998 | 1      | 0.356389 | 0.13586  |
| 2148 | EDMStream | sensor    | sensor    | 1.6    | 300   | 0.4  | 7000     | 0.998 | 1      | 0.356389 | 0.13586  |
| 2149 | EDMStream | sensor    | sensor    | 1.6    | 300   | 0.4  | 10000    | 0.998 | 1      | 0.356389 | 0.13586  |
| 2150 | EDMStream | sensor    | sensor    | 1.6    | 300   | 0.7  | 1000     | 0.998 | 1      | 0.356389 | 0.13586  |
| 2151 | EDMStream | sensor    | sensor    | 1.6    | 300   | 0.7  | 4000     | 0.998 | 1      | 0.356389 | 0.13586  |
| 2152 | EDMStream | sensor    | sensor    | 1.6    | 300   | 0.7  | 7000     | 0.998 | 1      | 0.356389 | 0.13586  |
| 2153 | EDMStream | sensor    | sensor    | 1.6    | 300   | 0.7  | 10000    | 0.998 | 1      | 0.356389 | 0.13586  |
| 2156 | EDMStream | sensor    | sensor    | 1.6    | 300   | 1    | 1000     | 0.998 | 1      | 0.356389 | 0.13586  |
| 2157 | EDMStream | sensor    | sensor    | 1.6    | 300   | 1    | 4000     | 0.998 | 1      | 0.356389 | 0.13586  |
| 2159 | EDMStream | sensor    | sensor    | 1.6    | 300   | 1    | 7000     | 0.998 | 1      | 0.356389 | 0.13586  |

<u>(4) DBStream </u>

| id   | algo     | workload  | workload  | lambda | radius | clean_interval | min_weight | alpha | base | cmm      | purity  |
| ---- | -------- | --------- | --------- | ------ | ------ | -------------- | ---------- | ----- | ---- | -------- | ------- |
| 1396 | DBStream | CoverType | CoverType | 0.125  | 300    | 400            | 0.5        | 0.3   | 2    | 0.402467 | 0.66789 |
| 1397 | DBStream | CoverType | CoverType | 0.125  | 300    | 400            | 0.5        | 0.4   | 2    | 0.402467 | 0.66789 |
| 1398 | DBStream | CoverType | CoverType | 0.125  | 300    | 400            | 0.5        | 0.5   | 2    | 0.402467 | 0.66789 |
| 1399 | DBStream | CoverType | CoverType | 0.125  | 300    | 400            | 0.5        | 0.6   | 2    | 0.402467 | 0.66789 |
| 1395 | DBStream | CoverType | CoverType | 0.125  | 300    | 400            | 0.5        | 0.2   | 2    | 0.402467 | 0.66789 |
| 1416 | DBStream | EDS       | EDS       | 0.998  | 20     | 400            | 0.5        | 0.2   | 2    | 0.150491 | 0.03835 |
| 1428 | DBStream | EDS       | EDS       | 0.998  | 20     | 400            | 0.5        | 0.6   | 2    | 0.150491 | 0.03835 |
| 1425 | DBStream | EDS       | EDS       | 0.998  | 20     | 400            | 0.5        | 0.5   | 2    | 0.150491 | 0.03835 |
| 1422 | DBStream | EDS       | EDS       | 0.998  | 20     | 400            | 0.5        | 0.4   | 2    | 0.150491 | 0.03835 |
| 1419 | DBStream | EDS       | EDS       | 0.998  | 20     | 400            | 0.5        | 0.3   | 2    | 0.150491 | 0.03835 |
| 1430 | DBStream | EDS_O     | EDS_O     | 0.998  | 10     | 400            | 0.5        | 0.2   | 2    | 0.346867 | 0.0792  |
| 1444 | DBStream | EDS_O     | EDS_O     | 0.998  | 30     | 400            | 0.5        | 0.6   | 2    | 0.346867 | 0.0792  |
| 1443 | DBStream | EDS_O     | EDS_O     | 0.998  | 20     | 400            | 0.5        | 0.6   | 2    | 0.346867 | 0.0792  |
| 1442 | DBStream | EDS_O     | EDS_O     | 0.998  | 10     | 400            | 0.5        | 0.6   | 2    | 0.346867 | 0.0792  |
| 1441 | DBStream | EDS_O     | EDS_O     | 0.998  | 30     | 400            | 0.5        | 0.5   | 2    | 0.346867 | 0.0792  |
| 1440 | DBStream | EDS_O     | EDS_O     | 0.998  | 20     | 400            | 0.5        | 0.5   | 2    | 0.346867 | 0.0792  |
| 1439 | DBStream | EDS_O     | EDS_O     | 0.998  | 10     | 400            | 0.5        | 0.5   | 2    | 0.346867 | 0.0792  |
| 1438 | DBStream | EDS_O     | EDS_O     | 0.998  | 30     | 400            | 0.5        | 0.4   | 2    | 0.346867 | 0.0792  |
| 1437 | DBStream | EDS_O     | EDS_O     | 0.998  | 20     | 400            | 0.5        | 0.4   | 2    | 0.346867 | 0.0792  |
| 1436 | DBStream | EDS_O     | EDS_O     | 0.998  | 10     | 400            | 0.5        | 0.4   | 2    | 0.346867 | 0.0792  |
| 1435 | DBStream | EDS_O     | EDS_O     | 0.998  | 30     | 400            | 0.5        | 0.3   | 2    | 0.346867 | 0.0792  |
| 1434 | DBStream | EDS_O     | EDS_O     | 0.998  | 20     | 400            | 0.5        | 0.3   | 2    | 0.346867 | 0.0792  |
| 1433 | DBStream | EDS_O     | EDS_O     | 0.998  | 10     | 400            | 0.5        | 0.3   | 2    | 0.346867 | 0.0792  |
| 1432 | DBStream | EDS_O     | EDS_O     | 0.998  | 30     | 400            | 0.5        | 0.2   | 2    | 0.346867 | 0.0792  |
| 1431 | DBStream | EDS_O     | EDS_O     | 0.998  | 20     | 400            | 0.5        | 0.2   | 2    | 0.346867 | 0.0792  |
| 1405 | DBStream | KDD99     | KDD99     | 0.25   | 180    | 500            | 0.5        | 0.4   | 2    | 0.591282 | 0.56237 |
| 1400 | DBStream | KDD99     | KDD99     | 0.25   | 80     | 500            | 0.5        | 0.4   | 2    | 0.591282 | 0.56237 |
| 1401 | DBStream | KDD99     | KDD99     | 0.25   | 100    | 500            | 0.5        | 0.4   | 2    | 0.591282 | 0.56237 |
| 1402 | DBStream | KDD99     | KDD99     | 0.25   | 120    | 500            | 0.5        | 0.4   | 2    | 0.591282 | 0.56237 |
| 1403 | DBStream | KDD99     | KDD99     | 0.25   | 140    | 500            | 0.5        | 0.4   | 2    | 0.591282 | 0.56237 |
| 1404 | DBStream | KDD99     | KDD99     | 0.25   | 160    | 500            | 0.5        | 0.4   | 2    | 0.591282 | 0.56237 |
| 1413 | DBStream | KDD99     | KDD99     | 0.125  | 200    | 500            | 0.5        | 0.4   | 2    | 0.591282 | 0.56237 |
| 1412 | DBStream | KDD99     | KDD99     | 0.125  | 180    | 500            | 0.5        | 0.4   | 2    | 0.591282 | 0.56237 |
| 1411 | DBStream | KDD99     | KDD99     | 0.125  | 160    | 500            | 0.5        | 0.4   | 2    | 0.591282 | 0.56237 |
| 1410 | DBStream | KDD99     | KDD99     | 0.125  | 140    | 500            | 0.5        | 0.4   | 2    | 0.591282 | 0.56237 |
| 1409 | DBStream | KDD99     | KDD99     | 0.125  | 120    | 500            | 0.5        | 0.4   | 2    | 0.591282 | 0.56237 |
| 1408 | DBStream | KDD99     | KDD99     | 0.125  | 100    | 500            | 0.5        | 0.4   | 2    | 0.591282 | 0.56237 |
| 1407 | DBStream | KDD99     | KDD99     | 0.125  | 80     | 500            | 0.5        | 0.4   | 2    | 0.591282 | 0.56237 |
| 1406 | DBStream | KDD99     | KDD99     | 0.25   | 200    | 500            | 0.5        | 0.4   | 2    | 0.591282 | 0.56237 |
| 1414 | DBStream | sensor    | sensor    | 0.998  | 2      | 1500           | 0.5        | 0.3   | 2    | 0.340904 | 0.07795 |

<u>(5) DStream</u>

all are the same

<u>(6) DenStream</u>

all are the same

<u>(7) CluStream</u>

| id   | algo      | workload  | workload  | num_last_arr | time_window | num_online_clusters | radius | buf_size | cmm      | purity  |
| ---- | --------- | --------- | --------- | ------------ | ----------- | ------------------- | ------ | -------- | -------- | ------- |
| 2414 | CluStream | CoverType | CoverType | 2            | 350         | 110                 | 14     | 1500     | 0.403882 | 0.73528 |
| 2508 | CluStream | KDD99     | KDD99     | 2            | 350         | 60                  | 2      | 1500     | 0.591417 | 0.97706 |
| 2509 | CluStream | KDD99     | KDD99     | 2            | 350         | 60                  | 5      | 1500     | 0.591417 | 0.97706 |
| 2510 | CluStream | KDD99     | KDD99     | 2            | 350         | 60                  | 8      | 1500     | 0.591417 | 0.97706 |
| 2846 | CluStream | sensor    | sensor    | 2            | 1000        | 200                 | 15     | 500      | 0.343071 | 0.09932 |

<u>(8) SL-KMeans</u>

all are the same
</details>


### Flags

```javascript
flags = ["algo", "input_file", "num_points", "dim", "num_clusters", "max_in_nodes", "max_leaf_nodes", "distance_threshold", "seed", "coreset_size", "radius", "delta", "beta", "buf_size", "alpha", "lambda", "clean_interval", "min_weight", "base", "cm", "cl", "grid_width", "min_points", "epsilon", "mu", "num_last_arr", "time_window", "num_online_clusters", "delta_grid", "num_samples"];
```

## How to Cite Sesame

* **[xxx]** We have an anonymized submission under review. Stay tuned.
