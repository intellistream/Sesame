SELECT workload, algo, obj, num_points, purity, qps, sum_us, mig_us, det_us, first_algo, final_algo, change_count, change_log FROM benne WHERE workload="KDD99_F" ORDER BY obj, num_points;