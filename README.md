# Sesame

This project aims at building a scalable stream mining library on modern hardware. 

- The repo contains currently several representative stream clustering algorithms.
- The code is still under active development and more algorithms will be introduced.
- We welcome your contributions, if you are interested to contribute to the project, please fork and submit a PR. If you have questions, feel free to log an issue or write an email to me: shuhao_zhang AT sutd.edu.sg

| Algorithm  | Window Model                 | Outlier Detection | Summarizing Data Structure | Offline Refinement |
| ---------- | ---------------------------- | ----------------- | --------------------------| -------------------|
| G1 | LandmarkWM           | OutlierD-B | MCs |   Refine   |
| G2 | LandmarkWM   | OutlierD-B | MCs | Refine |
| G3 | LandmarkWM   | OutlierD-B | CFT |   NoRefine   |
| G4 | SlidingWM | OutlierD-B | MCs |   NoRefine   |
| G5 | DampedWM             | OutlierD-B | MCs |   NoRefine   |
| G6 | LandmarkWM | NoOutlierD |MCs  | NoRefine |
| G7 | LandmarkWM | OutlierD-B | MCs |  NoRefine   |
| **G8** | **LandmarkWM** | **OutlierD-B** | **MCs** | **NoRefine** |
| **G9** | **LandmarkWM**   | **OutlierD-T** | **Grids** |   **NoRefine**   |
| G10 | LandmarkWM       | OutlierD-B | DPT |   NoRefine       |
| G11 | LandmarkWM       | OutlierD-T | MCs |   NoRefine       |
| G12 | LandmarkWM       | OutlierD | MCs |   NoRefine       |
| G13 | LandmarkWM       | OutlierD-BT | MCs |   NoRefine       |
| G14 | LandmarkWM | OutlierD-B | AMS |   NoRefine       |
| G15 | LandmarkWM       | OutlierD-B | CoreT |   NoRefine       |



# Algorithm Evaluation

## 1. DataSet

| DataSet   | Length                                | Dimension | Cluster Number |
| --------- | ------------------------------------- | --------- | -------------- |
| CoverType | 581012                                | 54        | 7              |
| KDD-99    | 4898431                               | 41        | 23             |
| Insects   | 905145                                | 33        | 24             |
| Sensor    | 2219803                               | 5         | 55             |
| EDS       | 45690, 100270, 150645, 200060, 245270 | 2         | 75, 145, 218, 289, 363 |
| EDS_O     | 94720,97360,100000                    | 2         | 90, 90, 90 |

## 2. Flags

```javascript
flags = ["algo", "input_file", "num_points", "dim", "num_clusters", "max_in_nodes", "max_leaf_nodes", "distance_threshold", "seed", "coreset_size", "radius", "delta", "beta", "buf_size", "alpha", "lambda", "clean_interval", "min_weight", "base", "cm", "cl", "grid_width", "min_points", "epsilon", "mu", "num_last_arr", "time_window", "num_online_clusters", "delta_grid", "num_samples"];
```






## How to Cite Sesame

* **[xxx]** We have an anonymized submission under review. Stay tuned.
