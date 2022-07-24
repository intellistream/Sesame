SELECT distinct t.algo AS "Algorithm", 
eds1.qps AS "2k/s", eds2.qps AS "20k/s", eds3.qps AS "50k/s", eds4.qps AS "100k/s"
FROM temp t 
LEFT JOIN (SELECT algo,qps FROM temp WHERE workload="EDS" AND arr_rate=2000 AND num_points=245270) as eds1 ON t.algo=eds1.algo 
LEFT JOIN (SELECT algo,qps FROM temp WHERE workload="EDS" AND arr_rate=20000 AND num_points=245270) as eds2 ON t.algo=eds2.algo
LEFT JOIN (SELECT algo,qps FROM temp WHERE workload="EDS" AND arr_rate=50000 AND num_points=245270) as eds3 ON t.algo=eds3.algo
LEFT JOIN (SELECT algo,qps FROM temp WHERE workload="EDS" AND arr_rate=100000 AND num_points=245270) as eds4 ON t.algo=eds4.algo;