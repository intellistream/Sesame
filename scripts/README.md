In the future, we will write a list of python program here for testing our system.

### Purity and Throughput on 4 real-world workloads

- G1-G15
- 8 existing algo

### CMM and Throughput on EDS

- G3, G6, G8, G9, G10,G11,G12,G13 G14, G15 (every step)

### Purity and Throughput on ODS

- G3, G4, G5 (every step)

### Number of clusters on EDS

- G3, G4, G5 (every 2000 points)

### Real-world Offline on 4 real-world workloads

- StreamKM++ with / without offline
- DenStream with / without offline

### Purity and Throughput on FCT on Window Model Configuration

- Landmark of G3: 1000 / 2000 / 4000 / 8000 / 16000 / 32000 / 50000 / 100000
- Sliding of G4: 1000 / 2000 / 4000 / 8000 / 16000 / 32000 / 50000 / 100000
- Damped of G5: alpha: 1.1, lambda: [0.2, 0.2, 2.0] | alpha: [1.1, 0.2, 2.7], lambda: 0.5
- DStream:  cl = 0.8, cm = 3, measure NGrids,  gap value, purity, and throughput when point number = 1000 / 2000 / 4000 / 8000 / 16000 / 32000 / 50000 / 100000

#### Default Parameter:

**EDMStream (G10):** alpha = 0.998, num_cache = 1000, radius = 250, lambda = 1(fixed), delta = 1500

**DStream (G9):** cm =3, cl = 0.8,  lambda = 0.998, beta= 0.3  (cm > cl)

**DenStream:** beta = 0.2, epsilon = 16, mu = 10, lambda = 0.25, base = 2 (fixed), buf_sizeSize = 1000

**StreamKM++ (G15):** coreset_size >= 200 k (suggested =)

**CluStream:** 

#### Tuning Range:

**EDMStream:** 

|           | FCT   | KDD                | Sensor                | Insects            | EDS                | ODS                |
| --------- | ----- | ------------------ | --------------------- | ------------------ | ------------------ | ------------------ |
| alpha     | 0.998 | 0.998              | 0.998                 | 0.998              | 0.998              | 0.998              |
| num_cache | 1000  | {1000, 3000, 5000} | {1000, 3000, 5000}    | {1000, 3000, 5000} | {1000, 3000, 5000} | {1000, 3000, 5000} |
| radius    | 250   | {50, 150, 250}     | {0.2, 0.4, 0.6, 1, 2} | {50, 150, 250}     | {0.5, 1, 2, 5, 10} | {0.5, 1, 2, 5, 10} |
| lambda    | 1     | 1                  | 1                     | 1                  | 1                  | 1                  |
| delta     | 1500  | [500, 500, 2500]   | [500, 500, 2500]      | [500, 500, 2500]   | [500, 500, 2500]   | [500, 500, 2500]   |

**G10:** 同EDMStream范围，但是去掉lambda，新加landmark范围: 1000 / 2000 / 4000 / 8000 / 16000 / 32000 / 50000 / 100000

**DenStream:** for all workloads:

beta = 0.2, epsilon = 16, mu = 10, lambda = 0.25, base = 2 (fixed), buf_sizeSize = 1000

**DStream:**

|            | FCT            | KDD                 | Sensor         | Insects        | EDS            | ODS            |
| ---------- | -------------- | ------------------- | -------------- | -------------- | -------------- | -------------- |
| Lambda     | 0.998          | 0.998               | 0.998          | 0.998          | 0.998          | 0.998          |
| beta       | {0.1,0.2, 0.3} | {0.1,0.2, 0.3}      | {0.1,0.2, 0.3} | {0.1,0.2, 0.3} | {0.1,0.2, 0.3} | {0.1,0.2, 0.3} |
| Grid_width | [20,20,200]    | [1000, 1000, 10000] | [20,20,200]    | [20,20,200]    | [20,20,200]    | [20,20,200]    |
| cm         | [5,5,20]       | [5,5,20]            | [3,3,18]       | [5,5,20]       | [3,3,18]       | [3,3,18]       |
| Cl         | [0.4, 0.2, 1]  | [0.4, 0.2, 1]       | [0.2, 0.2, 1]  | [0.4, 0.2, 1]  | [0.2, 0.2, 1]  | [0.2, 0.2, 1]  |

**G9:**  同DStream, 新加landmark范围: 1000 / 2000 / 4000 / 8000 / 16000 / 32000 / 50000 / 100000, cm范围全部修改成[20, 10, 100], cl范围修改成[2,2,10]

