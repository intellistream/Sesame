// Copyright (C) 2021 by the IntelliStream team
// (https://github.com/intellistream)

#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <filesystem>
#include "Algorithm/AlgorithmFactory.hpp"
#include "Algorithm/DataStructure/GenericFactory.hpp"
#include "Sinks/DataSinkFactory.hpp"
#include "Sources/DataSourceFactory.hpp"
#include "Utils/BenchmarkUtils.hpp"
#include "const.h"

using namespace std::filesystem;
using namespace std;
using namespace SESAME;

namespace py = pybind11;

// Define a global variable to hold the parameters
param_t &GetGlobalParam() {
    static param_t param;
    return param;
}

// Wrapper class for param_t
class Param {
public:

    Param() {
        GetGlobalParam().algo = BirchType;
        GetGlobalParam().num_points = 542;
        GetGlobalParam().dim = 54;
        GetGlobalParam().num_clusters = 2;
        GetGlobalParam().max_in_nodes = 3; // Maximum number of internal nodes
        GetGlobalParam().max_leaf_nodes = 3; // Maximum number of leaf nodes
        GetGlobalParam().distance_threshold = 3550.0; // Distance threshold
        GetGlobalParam().seed = 1; // Seed for random number generator
        GetGlobalParam().coreset_size = 100; // Coreset size
        GetGlobalParam().radius = 0.1; // Radius
        GetGlobalParam().delta = 10.0; // Delta
        GetGlobalParam().beta = 0.0021; // Beta
        GetGlobalParam().buf_size = 500; // Buffer size
        GetGlobalParam().alpha = 0.998; // Alpha
        GetGlobalParam().lambda = 1.0; // Lambda
        GetGlobalParam().clean_interval = 2500; // Clean interval
        GetGlobalParam().min_weight = 0.5; // Minimum weight
        GetGlobalParam().base = 2; // Base
        GetGlobalParam().cm = 5.0; // Cm
        GetGlobalParam().cl = 0.8; // Cl
        GetGlobalParam().grid_width = 12.0; // Grid width
        GetGlobalParam().min_points = 10; // Minimum points
        GetGlobalParam().epsilon = 50.0; // Epsilon
        GetGlobalParam().mu = 7.0; // Mu
        GetGlobalParam().num_last_arr = 60; // Number of last arrive
        GetGlobalParam().time_window = 6; // Time window
        GetGlobalParam().num_online_clusters = 10; // Number of online clusters
        GetGlobalParam().delta_grid = 0.2; // The delta parameter used in the grid for guessing the optimum
        GetGlobalParam().num_samples = 100; // Number of samples
        GetGlobalParam().landmark = 1000; // Landmark
        GetGlobalParam().sliding = 1000; // Sliding
        GetGlobalParam().outlier_distance_threshold = 1000; // Outlier distance threshold
        GetGlobalParam().outlier_cap = 100; // Outlier cap
        GetGlobalParam().outlier_density_threshold = 100; // Outlier density threshold
        GetGlobalParam().neighbor_distance = 200; // Neighbor distance
        GetGlobalParam().k = 0; // KMeans K
        GetGlobalParam().arr_rate = 0; // Arrival rate
        GetGlobalParam().run_offline = true; // Whether to run offline clustering
        GetGlobalParam().run_eval = true; // Whether to run evaluation
        GetGlobalParam().run_cmm = true; // Whether to run CMM evaluation
        GetGlobalParam().run_pur = true; // Whether to run Purity evaluation
        GetGlobalParam().time_interval = 100;
        GetGlobalParam().offline_time_window = 0;
        GetGlobalParam().opt = 2;
        GetGlobalParam().input_file = "datasets/CoverType.txt";
        GetGlobalParam().output_file = "results.txt";
        if (GetGlobalParam().algo == BirchType) {
            GetGlobalParam().outlier_cap = std::numeric_limits<int>::min();
        }
    }


    // AlgoType
    AlgoType algo() const { return GetGlobalParam().algo; }

    void set_algo(AlgoType value) { GetGlobalParam().algo = value; }

    // Dataset
    std::string input_file() const { return GetGlobalParam().input_file; }

    void set_input_file(const std::string &value) { GetGlobalParam().input_file = value; }

    int num_points() const { return GetGlobalParam().num_points; }

    void set_num_points(int value) { GetGlobalParam().num_points = value; }

    int dim() const { return GetGlobalParam().dim; }

    void set_dim(int value) { GetGlobalParam().dim = value; }

    int num_clusters() const { return GetGlobalParam().num_clusters; }

    void set_num_clusters(int value) { GetGlobalParam().num_clusters = value; }

    // BIRCH
    int max_in_nodes() const { return GetGlobalParam().max_in_nodes; }

    void set_max_in_nodes(int value) { GetGlobalParam().max_in_nodes = value; }

    int max_leaf_nodes() const { return GetGlobalParam().max_leaf_nodes; }

    void set_max_leaf_nodes(int value) { GetGlobalParam().max_leaf_nodes = value; }

    double distance_threshold() const { return GetGlobalParam().distance_threshold; }

    void set_distance_threshold(double value) { GetGlobalParam().distance_threshold = value; }

    // StreamKM++
    int seed() const { return GetGlobalParam().seed; }

    void set_seed(int value) { GetGlobalParam().seed = value; }

    int coreset_size() const { return GetGlobalParam().coreset_size; }

    void set_coreset_size(int value) { GetGlobalParam().coreset_size = value; }

    // EDMStream
    double radius() const { return GetGlobalParam().radius; }

    void set_radius(double value) { GetGlobalParam().radius = value; }

    double delta() const { return GetGlobalParam().delta; }

    void set_delta(double value) { GetGlobalParam().delta = value; }

    double beta() const { return GetGlobalParam().beta; }

    void set_beta(double value) { GetGlobalParam().beta = value; }

    int buf_size() const { return GetGlobalParam().buf_size; }

    void set_buf_size(int value) { GetGlobalParam().buf_size = value; }

    double alpha() const { return GetGlobalParam().alpha; }

    void set_alpha(double value) { GetGlobalParam().alpha = value; }

    double lambda() const { return GetGlobalParam().lambda; }

    void set_lambda(double value) { GetGlobalParam().lambda = value; }

    // DBStream
    int clean_interval() const { return GetGlobalParam().clean_interval; }

    void set_clean_interval(int value) { GetGlobalParam().clean_interval = value; }

    double min_weight() const { return GetGlobalParam().min_weight; }

    void set_min_weight(double value) { GetGlobalParam().min_weight = value; }

    double base() const { return GetGlobalParam().base; }

    void set_base(double value) { GetGlobalParam().base = value; }

    // DStream
    double cm() const { return GetGlobalParam().cm; }

    void set_cm(double value) { GetGlobalParam().cm = value; }

    double cl() const { return GetGlobalParam().cl; }

    void set_cl(double value) { GetGlobalParam().cl = value; }

    double grid_width() const { return GetGlobalParam().grid_width; }

    void set_grid_width(double value) { GetGlobalParam().grid_width = value; }

    // DenStream
    unsigned int min_points() const { return GetGlobalParam().min_points; }

    void set_min_points(unsigned int value) { GetGlobalParam().min_points = value; }

    double epsilon() const { return GetGlobalParam().epsilon; }

    void set_epsilon(double value) { GetGlobalParam().epsilon = value; }

    double mu() const { return GetGlobalParam().mu; }

    void set_mu(double value) { GetGlobalParam().mu = value; }

    // Clustream
    int num_last_arr() const { return GetGlobalParam().num_last_arr; }

    void set_num_last_arr(int value) { GetGlobalParam().num_last_arr = value; }

    int time_window() const { return GetGlobalParam().time_window; }

    void set_time_window(int value) { GetGlobalParam().time_window = value; }

    int num_online_clusters() const { return GetGlobalParam().num_online_clusters; }

    void set_num_online_clusters(int value) { GetGlobalParam().num_online_clusters = value; }

    // SL-KMeans
    double delta_grid() const { return GetGlobalParam().delta_grid; }

    void set_delta_grid(double value) { GetGlobalParam().delta_grid = value; }

    int num_samples() const { return GetGlobalParam().num_samples; }

    void set_num_samples(int value) { GetGlobalParam().num_samples = value; }

    // Generic
    int landmark() const { return GetGlobalParam().landmark; }

    void set_landmark(int value) { GetGlobalParam().landmark = value; }

    int sliding() const { return GetGlobalParam().sliding; }

    void set_sliding(int value) { GetGlobalParam().sliding = value; }

    double outlier_distance_threshold() const { return GetGlobalParam().outlier_distance_threshold; }

    void set_outlier_distance_threshold(double value) { GetGlobalParam().outlier_distance_threshold = value; }

    int outlier_cap() const { return GetGlobalParam().outlier_cap; }

    void set_outlier_cap(int value) { GetGlobalParam().outlier_cap = value; }

    double outlier_density_threshold() const { return GetGlobalParam().outlier_density_threshold; }

    void set_outlier_density_threshold(double value) { GetGlobalParam().outlier_density_threshold = value; }

    double neighbor_distance() const { return GetGlobalParam().neighbor_distance; }

    void set_neighbor_distance(double value) { GetGlobalParam().neighbor_distance = value; }

    int k() const { return GetGlobalParam().k; }

    void set_k(int value) { GetGlobalParam().k = value; }

    int arr_rate() const { return GetGlobalParam().arr_rate; }

    void set_arr_rate(int value) { GetGlobalParam().arr_rate = value; }

    bool run_offline() const { return GetGlobalParam().run_offline; }

    void set_run_offline(bool value) { GetGlobalParam().run_offline = value; }

    bool run_eval() const { return GetGlobalParam().run_eval; }

    void set_run_eval(bool value) { GetGlobalParam().run_eval = value; }

    bool run_cmm() const { return GetGlobalParam().run_cmm; }

    void set_run_cmm(bool value) { GetGlobalParam().run_cmm = value; }

    bool run_pur() const { return GetGlobalParam().run_pur; }

    void set_run_pur(bool value) { GetGlobalParam().run_pur = value; }

    void Print() { GetGlobalParam().Print(); }

    // Add other properties for the member variables of param_t

    // num_res
    int num_res() const { return GetGlobalParam().num_res; }

    void set_num_res(int value) { GetGlobalParam().num_res = value; }

    // Define docstrings for each parameter
    static constexpr const char *algo_doc = "Algorithm to use";
    static constexpr const char *input_file_doc = "Input file path";
    static constexpr const char *num_points_doc = "Number of points";
    static constexpr const char *dim_doc = "Dimension of points";
    static constexpr const char *num_clusters_doc = "Number of clusters";
    // BIRCH
    static constexpr const char *max_in_nodes_doc = "Maximum number of internal nodes";
    static constexpr const char *max_leaf_nodes_doc = "Maximum number of leaf nodes";
    static constexpr const char *distance_threshold_doc = "Distance threshold";
    // StreamKM++
    static constexpr const char *seed_doc = "Seed for random number generator";
    static constexpr const char *coreset_size_doc = "Coreset size";
    // EDMStream
    static constexpr const char *radius_doc = "Radius";
    static constexpr const char *delta_doc = "Delta";
    static constexpr const char *beta_doc = "Beta";
    static constexpr const char *buf_size_doc = "Buffer size";
    static constexpr const char *alpha_doc = "Alpha";
    static constexpr const char *lambda_doc = "Lambda";
    // DBStream
    static constexpr const char *clean_interval_doc = "Clean interval";
    static constexpr const char *min_weight_doc = "Minimum weight";
    static constexpr const char *base_doc = "Base";
    // DStream
    static constexpr const char *cm_doc = "Cm";
    static constexpr const char *cl_doc = "Cl";
    static constexpr const char *grid_width_doc = "Grid width";
    // DenStream
    static constexpr const char *min_points_doc = "Minimum points";
    static constexpr const char *epsilon_doc = "Epsilon";
    static constexpr const char *mu_doc = "Mu";
    // Clustream
    static constexpr const char *num_last_arr_doc = "Number of last arrive";
    static constexpr const char *time_window_doc = "Time window";
    static constexpr const char *num_online_clusters_doc = "Number of online clusters";
    // SL-KMeans
    static constexpr const char *delta_grid_doc = "The delta parameter used in the grid for guessing the optimum.";
    static constexpr const char *num_samples_doc = "Number of samples";
    // Generic
    static constexpr const char *landmark_doc = "Landmark";
    static constexpr const char *sliding_doc = "Sliding";
    static constexpr const char *outlier_distance_threshold_doc = "Outlier distance threshold";
    static constexpr const char *outlier_cap_doc = "Outlier cap";
    static constexpr const char *outlier_density_threshold_doc = "Outlier density threshold";
    static constexpr const char *neighbor_distance_doc = "Neighbor distance";
    static constexpr const char *k_doc = "KMeans K";
    static constexpr const char *arr_rate_doc = "Arrival rate";
    static constexpr const char *run_offline_doc = "Whether to run offline clustering";
    static constexpr const char *run_eval_doc = "Whether to run evaluation";
    static constexpr const char *run_cmm_doc = "Whether to run CMM evaluation";
    static constexpr const char *run_pur_doc = "Whether to run Purity evaluation";

private:
    param_t globalParam;
};

py::tuple run() {
    warning();
    // Access the parameters using the global `param` object
    param_t &param = GetGlobalParam();
    // Run algorithm producing results.
    auto res = SESAME::RunBenchmark(param);
    // return py::make_tuple(res.first, res.second);
    return py::make_tuple();
}

// Initialize the module
PYBIND11_MODULE(benne, m) {
    m.doc() = "Module documentation string";

    py::class_<Param>(m,
                           "Param")
            .def(py::init<>())
            .def_property("algo", &Param::algo, &Param::set_algo, Param::algo_doc)
            .def_property("input_file", &Param::input_file, &Param::set_input_file,
                          Param::input_file_doc)
            .def_property("num_points", &Param::num_points, &Param::set_num_points,
                          Param::num_points_doc)
            .def_property("dim", &Param::dim, &Param::set_dim, Param::dim_doc)
            .def_property("num_clusters", &Param::num_clusters, &Param::set_num_clusters,
                          Param::num_clusters_doc)
                    // BIRCH
            .def_property("max_in_nodes", &Param::max_in_nodes, &Param::set_max_in_nodes,
                          Param::max_in_nodes_doc)
            .def_property("max_leaf_nodes", &Param::max_leaf_nodes, &Param::set_max_leaf_nodes,
                          Param::max_leaf_nodes_doc)
            .def_property("distance_threshold", &Param::distance_threshold, &Param::set_distance_threshold,
                          Param::distance_threshold_doc)
                    // StreamKM++
            .def_property("seed", &Param::seed, &Param::set_seed, Param::seed_doc)
            .def_property("coreset_size", &Param::coreset_size, &Param::set_coreset_size,
                          Param::coreset_size_doc)
                    // EDMStream
            .def_property("radius", &Param::radius, &Param::set_radius, Param::radius_doc)
            .def_property("delta", &Param::delta, &Param::set_delta, Param::delta_doc)
            .def_property("beta", &Param::beta, &Param::set_beta, Param::beta_doc)
            .def_property("buf_size", &Param::buf_size, &Param::set_buf_size, Param::buf_size_doc)
            .def_property("alpha", &Param::alpha, &Param::set_alpha, Param::alpha_doc)
            .def_property("lambda", &Param::lambda, &Param::set_lambda, Param::lambda_doc)
                    // DBStream
            .def_property("clean_interval", &Param::clean_interval, &Param::set_clean_interval,
                          Param::clean_interval_doc)
            .def_property("min_weight", &Param::min_weight, &Param::set_min_weight,
                          Param::min_weight_doc)
            .def_property("base", &Param::base, &Param::set_base, Param::base_doc)
                    // DStream
            .def_property("cm", &Param::cm, &Param::set_cm, Param::cm_doc)
            .def_property("cl", &Param::cl, &Param::set_cl, Param::cl_doc)
            .def_property("grid_width", &Param::grid_width, &Param::set_grid_width,
                          Param::grid_width_doc)
                    // DenStream
            .def_property("min_points", &Param::min_points, &Param::set_min_points,
                          Param::min_points_doc)
            .def_property("epsilon", &Param::epsilon, &Param::set_epsilon, Param::epsilon_doc)
            .def_property("mu", &Param::mu, &Param::set_mu, Param::mu_doc)
                    // Clustream
            .def_property("num_last_arr", &Param::num_last_arr, &Param::set_num_last_arr,
                          Param::num_last_arr_doc)
            .def_property("time_window", &Param::time_window, &Param::set_time_window,
                          Param::time_window_doc)
            .def_property("num_online_clusters", &Param::num_online_clusters, &Param::set_num_online_clusters,
                          Param::num_online_clusters_doc)
                    // SL-KMeans
            .def_property("delta_grid", &Param::delta_grid, &Param::set_delta_grid,
                          Param::delta_grid_doc)
            .def_property("num_samples", &Param::num_samples, &Param::set_num_samples,
                          Param::num_samples_doc)
                    // Generic
            .def_property("landmark", &Param::landmark, &Param::set_landmark, Param::landmark_doc)
            .def_property("sliding", &Param::sliding, &Param::set_sliding, Param::sliding_doc)
            .def_property("outlier_distance_threshold", &Param::outlier_distance_threshold,
                          &Param::set_outlier_distance_threshold, Param::outlier_distance_threshold_doc)
            .def_property("outlier_cap", &Param::outlier_cap, &Param::set_outlier_cap,
                          Param::outlier_cap_doc)
            .def_property("outlier_density_threshold", &Param::outlier_density_threshold,
                          &Param::set_outlier_density_threshold, Param::outlier_density_threshold_doc)
            .def_property("neighbor_distance", &Param::neighbor_distance, &Param::set_neighbor_distance,
                          Param::neighbor_distance_doc)
            .def_property("k", &Param::k, &Param::set_k, Param::k_doc)
            .def_property("arr_rate", &Param::arr_rate, &Param::set_arr_rate, Param::arr_rate_doc)
            .def_property("run_offline", &Param::run_offline, &Param::set_run_offline,
                          Param::run_offline_doc)
            .def_property("run_eval", &Param::run_eval, &Param::set_run_eval, Param::run_eval_doc)
            .def_property("run_cmm", &Param::run_cmm, &Param::set_run_cmm, Param::run_cmm_doc)
            .def_property("run_pur", &Param::run_pur, &Param::set_run_pur, Param::run_pur_doc);

    py::enum_<AlgoType>(m, "AlgoType")
            .value("BIRCH", BirchType)
            .value("STREAMKM", StreamKMeansType)
            .value("CLUSTREAM", CluStreamType)
            .value("DENSTREAM", DenStreamType)
            .value("DBSTREAM", DBStreamType)
            .value("EDMSTREAM", EDMStreamType)
            .value("DSTREAM", DStreamType)
            .value("SLKMEANS", SLKMeansType);
    // Expose the algo_names array as a Python list
    py::list algoNames;
    for (int i = 0; i < sizeof(algo_names) / sizeof(algo_names[0]); ++i) {
        algoNames.append(algo_names[i]);
    }
    m.attr("algo_names") = algoNames;

    m.def("run", &run, py::return_value_policy::reference);
}

