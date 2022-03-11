# Sesame

This project aims at building a scalable stream mining library on modern hardware. 

- The repo contains currently several representative stream clustering algorithms.
- The code is still under active development and more algorithms will be introduced.
- We welcome your contributions, if you are interested to contribute to the project, please fork and submit a PR. If you have questions, feel free to log an issue or write an email to me: shuhao_zhang AT sutd.edu.sg

| Algorithm  | Window Model         | Outlier Detection | Data Structure  | Offline Refinement |
| ---------- | -------------------- | ----------------- | --------------- | ------------------ |
| **~~V1~~** | **~~Landmark~~**     | **~~Distance~~**  | **~~CF-Tree~~** | **~~KMeans++~~**   |
| **~~V2~~** | **~~Landmark~~**     | **~~Distance~~**  | **~~CF-Tree~~** | **~~DBSCAN~~**     |
| **~~V3~~** | **~~Landmark~~**     | **~~Distance~~**  | **~~CF-Tree~~** | **~~No~~**         |
| V4         | Sliding(Count-Based) | Distance          | CF-Tree         | No                 |
| V5         | Damped               | Distance          | CF-Tree         | No                 |
| V6         | Landmark             | Density           | CF-Tree         | No                 |
| V7         | Landmark             | Buffer            | CF-Tree         | No                 |
| V8         | Landmark             | Distance          | Macro Cluster   | No                 |
| V9         | Landmark             | Distance          | Grid            | No                 |



## How to Cite Sesame

* **[xxx]** We have an anonymized submission under review. Stay tuned.
