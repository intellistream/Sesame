SELECT distinct t.algo AS "Algorithm", 
cover.cluster_size AS "CoverType", kdd.cluster_size AS "KDD99", sensor.cluster_size AS "Sensor", insects.cluster_size AS "Insects", eds.cluster_size AS "EDS", ods.cluster_size AS "ODS"
FROM main t
LEFT JOIN (SELECT algo, cluster_size FROM main WHERE workload="CoverType") as cover ON t.algo=cover.algo 
LEFT JOIN (SELECT algo, cluster_size FROM main WHERE workload="KDD99_F") as kdd ON t.algo=kdd.algo
LEFT JOIN (SELECT algo, cluster_size FROM main WHERE workload="sensor") as sensor ON t.algo=sensor.algo
LEFT JOIN (SELECT algo, cluster_size FROM main WHERE workload="INSECTS") as insects ON t.algo=insects.algo
LEFT JOIN (SELECT algo, cluster_size FROM main WHERE workload="EDS" AND num_points=245270 AND arr_rate=0) as eds ON t.algo=eds.algo
LEFT JOIN (SELECT algo, cluster_size FROM main WHERE workload="EDS_O" AND num_points=100000 AND arr_rate=0) as ods ON t.algo=ods.algo
WHERE t.algo_id>=20;
