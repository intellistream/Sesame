# Benne

* This project borrows the benchmark setting from [[Sesame](https://github.com/intellistream/Sesame)].
* Benne is a modularized data stream clustering algorithm combined with four design aspects: Summarizing Data Structure, Window Model, Outlier Detection Mechanism and Offline Refinement Strategy.
* Benne is able to automatically detect the changes of evolving stream characteristics and self-adjusts its component according to the changes.

## Build and Run

* GCC-11 (In our paper, we use gcc-11.2.0)
* Boost: 1.78.0 [[Link](https://github.com/boostorg/boost)]
* GFLAGS: 2.2.0 [[Link](https://github.com/gflags/gflags/blob/master/INSTALL.md)]

## Real-world algorithms


| Algorithm  | Window Model | Outlier Detection | Summarizing Data Structure | Offline Refinement |
| ------------ | -------------- | ------------------- | ---------------------------- | -------------------- |
| BIRCH      | LandmarkWM   | OutlierD          | CFT                        | ❌                 |
| CluStream  | LandmarkWM   | OutlierD-T        | MCs                        | ✅                 |
| DenStream  | DampedWM     | OutlierD-BT       | MCs                        | ✅                 |
| DStream    | DampedWM     | OutlierD-T        | Grids                      | ❌                 |
| StreamKM++ | LandmarkWM   | NoOutlierD        | CoreT                      | ✅                 |
| DBStream   | DampedWM     | OutlierD-T        | MCs                        | ✅                 |
| EDMStream  | DampedWM     | OutlierD-BT       | DPT                        | ❌                 |
| SL-KMeans  | SlidingWM    | NoOutlierD        | AMS                        | ❌                 |

## Datasets


| DataSet   | Length                                | Dimension | Cluster Number         |
| ----------- | --------------------------------------- | ----------- | ------------------------ |
| CoverType | 581012                                | 54        | 7                      |
| KDD-99    | 4898431                               | 41        | 23                     |
| Insects   | 905145                                | 33        | 24                     |
| Sensor    | 2219803                               | 5         | 55                     |
| EDS       | 45690, 100270, 150645, 200060, 245270 | 2         | 75, 145, 218, 289, 363 |
| ODS       | 94720,97360,100000                    | 2         | 90, 90, 90             |

## How to Cite
* xxxx
