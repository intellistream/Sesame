# MOStream
### Prerequisites
- gcc 11 or latr
- cmake 3.14 or later
- boost
- gflags
- ticat
- mysql
### Build
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```
### Run Benchmark
Run benchmark:
```bash
cd scripts/benne
./run.sh
```
### Benchmark Result Tables

real-world algorithms on 4 datasets:

|algo         |workload |purity   |qps    |
|-------------|---------|---------|-------|
|Birch        |CoverType|0.562229 |225885 |
|StreamKMeans |CoverType|0.489109 |23177.3|
|CluStream    |CoverType|0.487599 |24890  |
|DenStream    |CoverType|0.548178 |9254.53|
|DBStream     |CoverType|0.487599 |82805.5|
|EDMStream    |CoverType|0.512048 |227606 |
|DStream      |CoverType|0.493124 |32779.4|
|SLKMeans     |CoverType|0.493613 |14911.8|
|BenneEff     |CoverType|0.521301 |240417 |
|BenneAcc     |CoverType|0.73752  |176212 |
|Birch        |INSECTS  |0.188058 |845001 |
|StreamKMeans |INSECTS  |0.488874 |19939.9|
|CluStream    |INSECTS  |0.188058 |192332 |
|DenStream    |INSECTS  |0.51571  |139924 |
|DBStream     |INSECTS  |0.425394 |890.007|
|EDMStream    |INSECTS  |0.500077 |300098 |
|DStream      |INSECTS  |0.188058 |136008 |
|SLKMeans     |INSECTS  |0.316064 |9548.79|
|BenneAcc     |INSECTS  |0.500227 |139159 |
|BenneEff     |INSECTS  |0.188058 |923562 |
|Birch        |KDD99_F  |0.941589 |340383 |
|StreamKMeans |KDD99_F  |0.581972 |29971.4|
|CluStream    |KDD99_F  |0.573222 |103739 |
|DenStream    |KDD99_F  |0.969101 |16177.1|
|DBStream     |KDD99_F  |0.724066 |247424 |
|EDMStream    |KDD99_F  |0.957839 |371575 |
|DStream      |KDD99_F  |0.960606 |380476 |
|SLKMeans     |KDD99_F  |0.645238 |1085.05|
|BenneAccNoMig|KDD99_F  |0.999071 |1570.43|
|BenneAcc     |KDD99_F  |0.999071 |1565.59|
|Birch        |sensor   |0.0300351|534377 |
|StreamKMeans |sensor   |0.0693233|74500.7|
|CluStream    |sensor   |0.0295923|77192.2|
|DenStream    |sensor   |0.0638467|43284.1|
|DBStream     |sensor   |0.0401955|57440.2|
|EDMStream    |sensor   |0.0782214|44856.6|
|DStream      |sensor   |0.0295941|252303 |
|SLKMeans     |sensor   |0.0295923|74552.8|
|BenneEff     |sensor   |0.030509 |210625 |
|BenneAcc     |sensor   |0.202618 |138474 |

Other results please refer to `scripts/benne/raw/*.csv`