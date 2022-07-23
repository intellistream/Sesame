SELECT distinct t.algo AS "Algorithm", 
eds1.cmm AS "1", eds2.cmm AS "2", eds3.cmm AS "3", eds4.cmm AS "4", eds5.cmm AS "5"
FROM temp t 
LEFT JOIN (SELECT algo,cmm FROM temp WHERE workload="EDS" AND arr_rate=0 AND num_points=45690) as eds1 ON t.algo=eds1.algo 
LEFT JOIN (SELECT algo,cmm FROM temp WHERE workload="EDS" AND arr_rate=0 AND num_points=100270) as eds2 ON t.algo=eds2.algo
LEFT JOIN (SELECT algo,cmm FROM temp WHERE workload="EDS" AND arr_rate=0 AND num_points=150645) as eds3 ON t.algo=eds3.algo
LEFT JOIN (SELECT algo,cmm FROM temp WHERE workload="EDS" AND arr_rate=0 AND num_points=200060) as eds4 ON t.algo=eds4.algo
LEFT JOIN (SELECT algo,cmm FROM temp WHERE workload="EDS" AND arr_rate=0 AND num_points=245270) as eds5 ON t.algo=eds5.algo;
