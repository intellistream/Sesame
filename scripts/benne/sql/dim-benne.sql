SELECT workload, algo, obj, purity, qps FROM benne WHERE workload LIKE "Dim%" AND algo_id=8 ORDER BY dim, algo_id, obj;