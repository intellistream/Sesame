SELECT distinct t.algo AS "Algorithm", 
ODS1.qps AS "1", ODS2.qps AS "2", ODS3.qps AS "3"
FROM main t 
LEFT JOIN (SELECT algo,qps FROM main WHERE workload="ODS" AND arr_rate=0 AND num_points=94270) as ODS1 ON t.algo=ODS1.algo 
LEFT JOIN (SELECT algo,qps FROM main WHERE workload="ODS" AND arr_rate=0 AND num_points=97360) as ODS2 ON t.algo=ODS2.algo
LEFT JOIN (SELECT algo,qps FROM main WHERE workload="ODS" AND arr_rate=0 AND num_points=100000) as ODS3 ON t.algo=ODS3.algo;
