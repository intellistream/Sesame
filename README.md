# Sesame

This project aims at building a scalable stream mining library on modern hardware. 

- The repo contains currently several representative real-world stream clustering algorithms and several synthetic algorithms.
- We welcome your contributions, if you are interested to contribute to the project, please fork and submit a PR. If you have questions, feel free to log an issue.

## Build Dependency
- GCC-11 (In our paper, we use gcc-11.2.0)
- Boost: 1.78.0 [Link](https://github.com/boostorg/boost)
- GFLAGS: 2.2.0 [Link](https://github.com/gflags/gflags/blob/master/INSTALL.md)

## Real-world algorithms

| Algorithm  | Window Model | Outlier Detection | Summarizing Data Structure | Offline Refinement |
| ---------- | ------------ | ----------------- | -------------------------- | ------------------ |
| BIRCH      | LandmarkWM   | OutlierD          | CFT                        | ❌                  |
| CluStream  | LandmarkWM   | OutlierD-T        | MCs                        | ✅                  |
| DenStream  | DampedWM     | OutlierD-BT       | MCs                        | ✅                  |
| DStream    | DampedWM     | OutlierD-T        | Grids                      | ❌                  |
| StreamKM++ | LandmarkWM   | NoOutlierD        | CoreT                      | ✅                  |
| DBStream   | DampedWM     | OutlierD-T        | MCs                        | ✅                  |
| EDMStream  | DampedWM     | OutlierD-BT       | DPT                        | ❌                  |
| SL-KMeans  | SlidingWM    | NoOutlierD        | AMS                        | ❌                  |

## Synthetic algorithms

| Algorithm  | Window Model                 | Outlier Detection | Summarizing Data Structure | Offline Refinement |
| ---------- | ---------------------------- | ----------------- | --------------------------| -------------------|
| G1 | LandmarkWM           | OutlierD | MCs |   ✅   |
| G2 | LandmarkWM   | OutlierD | MCs | ✅ |
| G3 | LandmarkWM   | OutlierD | CFT |   ❌   |
| G4 | SlidingWM | OutlierD | MCs |   ❌   |
| G5 | DampedWM             | OutlierD-B | MCs |   ❌   |
| G6 | LandmarkWM | NoOutlierD | MCs  | ❌ |
| G8 | LandmarkWM | OutlierD | MCs | ❌ |
| G9 | LandmarkWM   | OutlierD | Grids |   ❌   |
| G10 | LandmarkWM       | OutlierD | DPT |   ❌      |
| G11 | LandmarkWM       | OutlierD-T | MCs |   ❌      |
| G12 | LandmarkWM       | OutlierD-B | MCs |   ❌      |
| G13 | LandmarkWM       | OutlierD-BT | MCs |   ❌      |
| G14 | LandmarkWM | OutlierD | AMS |   ❌      |
| G15 | LandmarkWM       | OutlierD | CoreT |   ❌      |

## Datasets

| DataSet   | Length                                | Dimension | Cluster Number |
| --------- | ------------------------------------- | --------- | -------------- |
| CoverType | 581012                                | 54        | 7              |
| KDD-99    | 4898431                               | 41        | 23             |
| Insects   | 905145                                | 33        | 24             |
| Sensor    | 2219803                               | 5         | 55             |
| EDS       | 45690, 100270, 150645, 200060, 245270 | 2         | 75, 145, 218, 289, 363 |
| ODS     | 94720,97360,100000                    | 2         | 90, 90, 90 |

You may download the datasets here: https://zenodo.org/records/8210331

## How to Cite Sesame

* **[SIGMOD 2023]** Xin Wang and Zhengru Wang and Zhenyu Wu and Shuhao Zhang and Xuanhua Shi and Li Lu. Data Stream Clustering: An In-depth Empirical Study, SIGMOD, 2023

```
@inproceedings{wang2023sesame,
	title        = {Data Stream Clustering: An In-depth Empirical Study},
	author       = {Xin Wang and Zhengru Wang and Zhenyu Wu and Shuhao Zhang and Xuanhua Shi and Li Lu},
	year         = 2023,
	booktitle    = {Proceedings of the 2023 International Conference on Management of Data (SIGMOD)},
	location     = {Seattle, WA, USA},
	publisher    = {Association for Computing Machinery},
	address      = {New York, NY, USA},
	series       = {SIGMOD '23},
	abbr         = {SIGMOD},
	bibtex_show  = {true},
	selected     = {true},
	pdf          = {papers/Sesame.pdf},
	code         = {https://github.com/intellistream/Sesame},
	doi	         = {10.1145/3589307},
        url          = {https://doi.org/10.1145/3589307}
}
```

