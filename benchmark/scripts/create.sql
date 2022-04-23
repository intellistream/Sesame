CREATE TABLE IF NOT EXISTS sesame (
                id INT PRIMARY KEY AUTO_INCREMENT, \
                bench_begin TIMESTAMP, \
                run_begin TIMESTAMP, \
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
                ds_us INT, \
                out_us INT, \
                ref_us INT, \
                sum_us INT, \
                lat_us DOUBLE, \
                et_s DOUBLE, \
                qps DOUBLE, \
                cmm DOUBLE, \
                purity DOUBLE \
);

CREATE VIEW birch as SELECT id, bench_begin, algo, workload, max_in_nodes, max_leaf_nodes, distance_threshold, cmm, purity FROM sesame WHERE algo = 'Birch'; 