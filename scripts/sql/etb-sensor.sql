SELECT algo AS "Algorithm", win_s AS "WindowModel", out_s AS "OutlierDetection", ds_s AS "DataStructure", off_s AS "OfflineRefinement" FROM main 
WHERE workload="sensor";