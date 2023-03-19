SELECT distinct t.algo AS "Algorithm", 
cover.num_res AS "CoverType", kdd.num_res AS "KDD99", sensor.num_res AS "Sensor", insects.num_res AS "Insects", eds.num_res AS "EDS", ods.num_res AS "ODS"
FROM main t 
LEFT JOIN (SELECT algo, num_res FROM main WHERE workload="CoverType") as cover ON t.algo=cover.algo 
LEFT JOIN (SELECT algo, num_res FROM main WHERE workload="KDD99_F") as kdd ON t.algo=kdd.algo
LEFT JOIN (SELECT algo, num_res FROM main WHERE workload="sensor") as sensor ON t.algo=sensor.algo
LEFT JOIN (SELECT algo, num_res FROM main WHERE workload="INSECTS") as insects ON t.algo=insects.algo
LEFT JOIN (SELECT algo, num_res FROM main WHERE workload="EDS" AND num_points=245270 AND arr_rate=0) as eds ON t.algo=eds.algo
LEFT JOIN (SELECT algo, num_res FROM main WHERE workload="ODS" AND num_points=100000 AND arr_rate=0) as ods ON t.algo=ods.algo
;

