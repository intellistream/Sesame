SELECT algo AS "Algorithm", 0 AS "WindowModel", out_s AS "OutlierDetection", ds_s AS "DataStructure", off_s AS "OfflineRefinement" FROM temp 
WHERE workload="CoverType";