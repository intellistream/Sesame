SELECT distinct t.algo AS "Algorithm", 
cover.purity AS "CoverType", kdd.purity AS "KDD99", sensor.purity AS "Sensor", insects.purity AS "Insects", eds.purity AS "EDS", ods.purity AS "ODS"
FROM main t 
LEFT JOIN (SELECT algo, purity FROM main WHERE workload="CoverType") as cover ON t.algo=cover.algo 
LEFT JOIN (SELECT algo, purity FROM main WHERE workload="KDD99_F") as kdd ON t.algo=kdd.algo
LEFT JOIN (SELECT algo, purity FROM main WHERE workload="sensor") as sensor ON t.algo=sensor.algo
LEFT JOIN (SELECT algo, purity FROM main WHERE workload="INSECTS") as insects ON t.algo=insects.algo
LEFT JOIN (SELECT algo, purity FROM main WHERE workload="EDS" AND num_points=245270 AND arr_rate=0) as eds ON t.algo=eds.algo
LEFT JOIN (SELECT algo, purity FROM main WHERE workload="EDS_O" AND num_points=100000 AND arr_rate=0) as ods ON t.algo=ods.algo
;

