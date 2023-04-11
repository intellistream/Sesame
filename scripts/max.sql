SELECT id, algo, workload, max_in_nodes, max_leaf_nodes, distance_threshold, cmm, purity FROM birch WHERE (workload,purity) IN (SELECT workload, max(purity) FROM birch WHERE id>6292 GROUP BY workload) ORDER by workload;

SELECT id, algo, workload, tag, workload, seed, num_clusters, coreset_size, cmm, purity FROM streamkm WHERE (workload,purity) IN (SELECT workload, max(purity) FROM streamkm WHERE tag="offline" GROUP BY workload) ORDER by workload;

SELECT id, algo, workload, radius, delta, beta, buf_size, alpha, lambda, cmm, purity FROM edmstream WHERE (workload,purity) IN (SELECT workload, max(purity) FROM edmstream GROUP BY workload) ORDER by workload;

SELECT id, algo, workload, lambda, radius, clean_interval, min_weight, alpha, base, cmm, purity FROM dbstream WHERE (workload, purity) IN (SELECT workload, max(purity) FROM dbstream GROUP BY workload) ORDER by workload;

SELECT id, algo, workload, lambda, beta, cm, cl, grid_width, cmm, purity FROM dstream WHERE (workload, purity) IN (SELECT workload, max(purity) FROM dstream GROUP BY workload) ORDER by workload;

SELECT id, algo, workload, num_points, buf_size, min_points, epsilon, base, lambda, mu, beta, cmm, purity FROM denstream WHERE (workload, purity) IN (SELECT workload, max(purity) FROM denstream WHERE purity!=1 GROUP BY workload) ORDER by workload;

SELECT id, algo, workload, num_last_arr, time_window, num_online_clusters, radius, buf_size, cmm, purity FROM clustream WHERE (workload, purity) IN (SELECT workload, max(purity) FROM clustream GROUP BY workload) ORDER by workload;

SELECT id, algo, workload, delta_grid, num_samples, cmm, purity FROM slkmeans WHERE (workload, purity) IN (SELECT workload, max(purity) FROM slkmeans GROUP BY workload) ORDER by workload;



SELECT * FROM perf WHERE workload="INSECTS" ORDER BY algo, arr_rate, run_begin;

SELECT * FROM perf WHERE workload="INSECTS" AND id >= 9360 ORDER BY algo, arr_rate, run_begin;