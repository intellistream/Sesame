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

CoverType

KDD-99

Sensor

### Parameter Range

##### (1) BIRCH

| Parameter         | CoverType      | KDD-99         | Sensor         |
| ----------------- | -------------- | -------------- | -------------- |
| MaxInternalNodes  | [10,10,400]    | [100,100,4000] | [100,100,4000] |
| MaxLeafNodes      | [10,10,400]    | [100,100,4000] | [100,100,4000] |
| ThresholdDistance | [100,100,2000] | [100,100,2000] | [10,5,60]      |

##### (2) StreamKM++

| Parameter     | CoverType      | KDD-99           | Sensor           |
| ------------- | -------------- | ---------------- | ---------------- |
| Seed          | 1,10,100       | 1,10,100         | 1,10,100         |
| ClusterNumber | 7              | 23               | 54               |
| CorestSize    | [100,100,5000] | [1000,100,10000] | [1000,100,10000] |

##### (3) EDMStream

| Parameter  | CoverType       | KDD-99           | Sensor           |
| ---------- | --------------- | ---------------- | ---------------- |
| Radius     | [50,50,1000]    | [10,5,500]       | [0.1,0.1,5]      |
| Delta      | [10,10,1000]    | [10,10,1000]     | [10,10,1000]     |
| Beta       | [0.001,0.001,1] | [0.001,0.001,1]  | [0.001,0.001,1]  |
| BufferSize | [100,100,2000]  | [1000,100,10000] | [1000,100,10000] |
| Alpha      | [0.1,0.01,0.99] | [0.1,0.01,0.99]  | [0.1,0.01,0.99]  |
| Lamda      | [1,0.2,5]       | [1,0.2,5]        | [1,0.2,5]        |

##### (4) DBStream

| Parameter       | CoverType             | KDD-99       | Sensor |
| --------------- | --------------------- | ------------ | ------ |
| lambda          | {0.25,0.125}          | {0.25,0.125} | 0.998  |
| radius          | 300                   | [80,10, 200] | 2      |
| cleanUpInterval | 400                   | 500          | 1500   |
| weightMin       | 0.5                   | 0.5          | 0.25   |
| alpha           | {0.2,0.3,0.4,0.5,0.6} | 0.4          | 0.3    |
| base            | 2                     | 2            | 2      |

##### (5) DStream

| Parameter | CoverType       | KDD-99                    | Sensor          |
| --------- | --------------- | ------------------------- | --------------- |
| lambda    | {0.998, 0.999}  | {0.998,0.999}             | 0.998           |
| beta      | 0.001, 0.3      | {0.001,0.3}               | 0.001           |
| cm        | [5,5,20]        | {1.001, 3}                | 4               |
| cl        | {0.8,1.0,0.001} | {0.7,0.8,0.9,0.999,1.001} | 2               |
| gridWidth | [12,1,15]       | [4,1,8]                   | [0.15,0.05,0.5] |

##### (6) DenStream

| Parameter      | CoverType      | KDD-99         | Sensor         |
| -------------- | -------------- | -------------- | -------------- |
| initBufferSize | [500,500,2000] | [500,500,2000] | [500,500,1500] |
| minPoints      | [10,10,50]     | [10,10,50]     | 5              |
| epsilon        | [20,5,45]      | [25,5,45]      | 0.8            |
| base           | 2              | 2              | 2              |
| lambda         | {0.25,0.125}   | {0.25,0.125}   | {0.25,0.125}   |
| mu             | [2,1,10]       | [2,1,10]       | 6              |
| beta           | 0.25           | 0.25           | 0.25           |

##### (7) Clustream

| Parameter            | CoverType      | KDD-99         | Sensor       |
| -------------------- | -------------- | -------------- | ------------ |
| lastArrivingNum      | [2,1,8]        | [2,1,16]       | [2,1,15]     |
| timeWindow           | [200,50,350]   | [200,50,350]   | 1000         |
| timeInterval         | random         | random         | random       |
| offlineClusterNumber | 7              | 23             | 54           |
| ClusterNumber        | [80,10,150]    | [60,10,150]    | [100,10,200] |
| radiusFactor         | [2,1,26]       | [2,1,8]        | 15           |
| initBuffer           | [500,500,1500] | [500,500,1500] | 500          |
| offlineTimeWindow    | random         | random         | random       |

##### (8) SL-KMeans

## How to Cite Sesame

* **[xxx]** We have an anonymized submission under review. Stay tuned.
