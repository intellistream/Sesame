SELECT algo, workload, run_offline, purity, qps FROM extra WHERE tag="extra.offline" ORDER BY algo, workload, run_offline;