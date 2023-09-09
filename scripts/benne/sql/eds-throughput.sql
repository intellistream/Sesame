SELECT distinct t.algo AS "Algorithm", 
eds1.qps AS "1", eds2.qps AS "2", eds3.qps AS "3", eds4.qps AS "4", eds5.qps AS "5"
FROM benne t
LEFT JOIN (SELECT algo,qps FROM benne WHERE workload="EDS" AND num_points=45690) as eds1 ON t.algo=eds1.algo 
LEFT JOIN (SELECT algo,qps FROM benne WHERE workload="EDS" AND num_points=100270) as eds2 ON t.algo=eds2.algo
LEFT JOIN (SELECT algo,qps FROM benne WHERE workload="EDS" AND num_points=150645) as eds3 ON t.algo=eds3.algo
LEFT JOIN (SELECT algo,qps FROM benne WHERE workload="EDS" AND num_points=200060) as eds4 ON t.algo=eds4.algo
LEFT JOIN (SELECT algo,qps FROM benne WHERE workload="EDS" AND num_points=245270) as eds5 ON t.algo=eds5.algo;
