SELECT
    DISTINCT t.algo AS "algo",
    fct.sum_us "fct.sum_us",
    fct.mig_us "fct.mig_us",
    fct.det_us "fct.det_us",
    kdd.sum_us "kdd.sum_us",
    kdd.mig_us "kdd.mig_us",
    kdd.det_us "kdd.det_us",
    sensor.sum_us "sensor.sum_us",
    sensor.mig_us "sensor.mig_us",
    sensor.det_us "sensor.det_us",
    insects.sum_us "insects.sum_us",
    insects.mig_us "insects.mig_us",
    insects.det_us "insects.det_us",
    eds.sum_us "eds.sum_us",
    eds.mig_us "eds.mig_us",
    eds.det_us "eds.det_us",
    ods.sum_us "ods.sum_us",
    ods.mig_us "ods.mig_us",
    ods.det_us "ods.det_us"
FROM
    benne t
    LEFT JOIN (
        SELECT
            algo,
            sum_us,
            mig_us,
            det_us
        FROM
            benne
        WHERE
            workload = "CoverType"
            AND tag = ''
    ) AS fct ON t.algo = fct.algo
    LEFT JOIN (
        SELECT
            algo,
            sum_us,
            mig_us,
            det_us
        FROM
            benne
        WHERE
            workload = "KDD99_F"
            AND num_points = 4898431
    ) AS kdd ON t.algo = kdd.algo
    LEFT JOIN (
        SELECT
            algo,
            sum_us,
            mig_us,
            det_us
        FROM
            benne
        WHERE
            workload = "sensor"
            AND tag = ''
    ) AS sensor ON t.algo = sensor.algo
    LEFT JOIN (
        SELECT
            algo,
            sum_us,
            mig_us,
            det_us
        FROM
            benne
        WHERE
            workload = "INSECTS"
            AND tag = ''
    ) AS insects ON t.algo = insects.algo
    LEFT JOIN (
        SELECT
            algo,
            sum_us,
            mig_us,
            det_us
        FROM
            benne
        WHERE
            workload = "EDS"
            AND tag = ''
            AND num_points = 245270
    ) AS eds ON t.algo = eds.algo
    LEFT JOIN (
        SELECT
            algo,
            sum_us,
            mig_us,
            det_us
        FROM
            benne
        WHERE
            workload = "EDS_O"
            AND tag = ''
            AND num_points = 100000
    ) AS ods ON t.algo = ods.algo
WHERE
    t.algo_id = 8;