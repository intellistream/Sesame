CREATE TABLE IF NOT EXISTS sesame (
                id INT PRIMARY KEY AUTO_INCREMENT, \
                bench_begin TIMESTAMP, \
                run_begin TIMESTAMP, \
                run_end TIMESTAMP, \
                tag VARCHAR(512), \
                algo VARCHAR(16), \
                workload VARCHAR(16), \
                num_points INT, \
                dim INT, \
                num_clusters INT, \
                max_in_nodes INT, \
                max_leaf_nodes INT, \
                distance_threshold DOUBLE, \
                seed INT, \
                coreset_size INT, \
                radius DOUBLE, \
                delta DOUBLE, \
                beta DOUBLE, \
                buf_size INT, \
                alpha DOUBLE, \
                lambda DOUBLE, \
                clean_interval INT, \
                min_weight DOUBLE, \
                base INT, \
                cm DOUBLE, \
                cl DOUBLE, \
                grid_width DOUBLE, \
                min_points INT, \
                epsilon DOUBLE, \
                mu DOUBLE, \
                num_last_arr INT, \
                time_window INT, \
                num_online_clusters INT, \
                delta_grid DOUBLE, \
                num_samples DOUBLE, \
                ds_us BIGINT, \
                out_us BIGINT, \
                ref_us BIGINT, \
                sum_us BIGINT, \
                lat_us DOUBLE, \
                et_s DOUBLE, \
                qps DOUBLE, \
                cmm DOUBLE, \
                purity DOUBLE \
);

CREATE VIEW birch as SELECT id, bench_begin, algo, workload, max_in_nodes, max_leaf_nodes, distance_threshold, cmm, purity FROM sesame WHERE algo = 'Birch'; 

CREATE VIEW streamkm as SELECT id, bench_begin, algo, tag, workload, seed, num_clusters, coreset_size, cmm, purity FROM sesame WHERE algo = 'StreamKMeans'; 

CREATE VIEW edmstream as SELECT id, bench_begin, algo, workload, radius, delta, beta, buf_size, alpha, lambda, cmm, purity FROM sesame WHERE algo = 'EDMStream'; 

CREATE VIEW dbstream as SELECT id, bench_begin, algo, workload, lambda, radius, clean_interval, min_weight, alpha, base, cmm, purity FROM sesame WHERE algo = 'DBStream'; 

CREATE VIEW dstream as SELECT id, bench_begin, algo, workload, lambda, beta, cm, cl, grid_width, cmm, purity FROM sesame WHERE algo = 'DStream'; 

CREATE VIEW denstream as SELECT id, bench_begin, algo, workload, num_points, buf_size, min_points, epsilon, base, lambda, mu, beta, cmm, purity FROM sesame WHERE algo = 'DenStream'; 

CREATE VIEW clustream as SELECT id, bench_begin, algo, workload, num_last_arr, time_window, num_online_clusters, radius, buf_size, cmm, purity FROM sesame WHERE algo = 'CluStream'; 

CREATE VIEW slkmeans as SELECT id, bench_begin, algo, workload, num_points, delta_grid, num_samples, cmm, purity FROM sesame WHERE algo = 'SL-KMeans'; 

CREATE VIEW perf as SELECT id, bench_begin, algo, workload, purity, cmm, qps, lat_us, sum_us/1000000 as sum_s, ds_us/1000000 as ds_s, out_us/1000000 as out_s, ref_us/1000000 as off_s FROM sesame WHERE id > 5008;
