SELECT distinct t.algo AS "Algorithm", 
cover.outlier_size AS "CoverType", kdd.outlier_size AS "KDD99", sensor.outlier_size AS "Sensor", insects.outlier_size AS "Insects", eds.outlier_size AS "EDS", ods.outlier_size AS "ODS"
FROM main t 
LEFT JOIN (SELECT algo, outlier_size FROM main WHERE workload="CoverType" AND algo_id>=20) as cover ON t.algo=cover.algo 
LEFT JOIN (SELECT algo, outlier_size FROM main WHERE workload="KDD99_F" AND algo_id>=20) as kdd ON t.algo=kdd.algo
LEFT JOIN (SELECT algo, outlier_size FROM main WHERE workload="sensor" AND algo_id>=20) as sensor ON t.algo=sensor.algo
LEFT JOIN (SELECT algo, outlier_size FROM main WHERE workload="INSECTS" AND algo_id>=20) as insects ON t.algo=insects.algo
LEFT JOIN (SELECT algo, outlier_size FROM main WHERE workload="EDS" AND num_points=245270 AND arr_rate=0 AND algo_id>=20) as eds ON t.algo=eds.algo
LEFT JOIN (SELECT algo, outlier_size FROM main WHERE workload="ODS" AND num_points=100000 AND arr_rate=0 AND algo_id>=20) as ods ON t.algo=ods.algo
WHERE t.algo_id>=20;
