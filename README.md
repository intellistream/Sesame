# Sesame

[![version](https://img.shields.io/github/v/release/intellistream/Sesame?color=green)](https://github.com/intellistream/Sesame/releases)
[![version](https://img.shields.io/badge/cpp-c%2B%2B20-red)](https://github.com/intellistream/Sesame)
[![pyversion](https://img.shields.io/pypi/pyversions/pysame.svg?color=)](https://pypi.org/project/pysame/)
[![os](https://img.shields.io/badge/os-linux-yellow)](https://github.com/intellistream/Sesame)
[![PyPI - License](https://img.shields.io/pypi/l/pysame.svg?color=orange)](https://github.com/intellistream/Sesame/blob/main/LICENSE)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.8210331.svg)](https://zenodo.org/records/8210331)

## About

Sesame is scalable stream mining library on modern hardware written in C++


By now Sesame contains several representative real-world stream clustering algorithms and synthetic algorithms

## Quick Start

### Installation
```bash
pip3 install pysame
```

### Python Example

```python
#!python3

from pysame import Benne, Birch, BenneObj

X = [[0, 1], [0.3, 1], [-0.3, 1], [0, -1], [0.3, -1], [-0.3, -1]]

# run birch algorithm
brc = Birch(
    n_clusters=2,
    dim=2,
    distance_threshold=0.5,
)
print(brc.partial_fit(X).predict(X))

# run benne algorithm
bne = Benne(
    n_clusters=2,
    dim=2,
    distance_threshold=0.5,
    obj=BenneObj.accuracy,
)
print(bne.partial_fit(X).predict(X))
```

## Build Sesame

### Prerequisites
- gcc 12 or lator
- cmake 3.14 or later
- [boost](https://github.com/boostorg/boost)
- [gflags](https://github.com/gflags/gflags)

### Checkout Source Code
```bash
git clone https://github.com/intellistream/Sesame --recursive --depth=1
cd Sesame
```

### Build
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

### Run Tests
Download the datasets from [Zenodo](https://zenodo.org/records/8210331) and put them in the `datasets` directory:
```bash
cd Sesame/datasets
pip3 install zenodo_get
zenodo_get 8210331
```
Run the tests:
```bash
cd Sesame/build/test
./google_test
```

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

Datasets can download from zenodo: https://zenodo.org/records/8210331

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

