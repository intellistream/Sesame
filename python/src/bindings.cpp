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
param_t &GetGlobalParameters() {
    static param_t param;
    return param;
}

// Wrapper class for param_t
class Parameters {
public:

    Parameters() {
        GetGlobalParameters().algo = BirchType;
        GetGlobalParameters().num_points = 542;
        GetGlobalParameters().dim = 54;
        GetGlobalParameters().num_clusters = 2;
        GetGlobalParameters().max_in_nodes = 3; // Maximum number of internal nodes
        GetGlobalParameters().max_leaf_nodes = 3; // Maximum number of leaf nodes
        GetGlobalParameters().distance_threshold = 3550.0; // Distance threshold
        GetGlobalParameters().seed = 1; // Seed for random number generator
        GetGlobalParameters().coreset_size = 100; // Coreset size
        GetGlobalParameters().radius = 0.1; // Radius
        GetGlobalParameters().delta = 10.0; // Delta
        GetGlobalParameters().beta = 0.0021; // Beta
        GetGlobalParameters().buf_size = 500; // Buffer size
        GetGlobalParameters().alpha = 0.998; // Alpha
        GetGlobalParameters().lambda = 1.0; // Lambda
        GetGlobalParameters().clean_interval = 2500; // Clean interval
        GetGlobalParameters().min_weight = 0.5; // Minimum weight
        GetGlobalParameters().base = 2; // Base
        GetGlobalParameters().cm = 5.0; // Cm
        GetGlobalParameters().cl = 0.8; // Cl
        GetGlobalParameters().grid_width = 12.0; // Grid width
        GetGlobalParameters().min_points = 10; // Minimum points
        GetGlobalParameters().epsilon = 50.0; // Epsilon
        GetGlobalParameters().mu = 7.0; // Mu
        GetGlobalParameters().num_last_arr = 60; // Number of last arrive
        GetGlobalParameters().time_window = 6; // Time window
        GetGlobalParameters().num_online_clusters = 10; // Number of online clusters
        GetGlobalParameters().delta_grid = 0.2; // The delta parameter used in the grid for guessing the optimum
        GetGlobalParameters().num_samples = 100; // Number of samples
        GetGlobalParameters().landmark = 1000; // Landmark
        GetGlobalParameters().sliding = 1000; // Sliding
        GetGlobalParameters().outlier_distance_threshold = 1000; // Outlier distance threshold
        GetGlobalParameters().outlier_cap = 100; // Outlier cap
        GetGlobalParameters().outlier_density_threshold = 100; // Outlier density threshold
        GetGlobalParameters().neighbor_distance = 200; // Neighbor distance
        GetGlobalParameters().k = 0; // KMeans K
        GetGlobalParameters().arr_rate = 0; // Arrival rate
        GetGlobalParameters().run_offline = true; // Whether to run offline clustering
        GetGlobalParameters().run_eval = true; // Whether to run evaluation
        GetGlobalParameters().run_cmm = true; // Whether to run CMM evaluation
        GetGlobalParameters().run_pur = true; // Whether to run Purity evaluation
        GetGlobalParameters().time_interval = 100;
        GetGlobalParameters().offline_time_window = 0;
        GetGlobalParameters().opt = 2;
        GetGlobalParameters().input_file = "datasets/CoverType.txt";
        GetGlobalParameters().output_file = "results.txt";
        if (GetGlobalParameters().algo == BirchType) {
            GetGlobalParameters().outlier_cap = std::numeric_limits<int>::min();
        }
    }


    // AlgoType
    AlgoType algo() const { return GetGlobalParameters().algo; }

    void set_algo(AlgoType value) { GetGlobalParameters().algo = value; }

    // Dataset
    std::string input_file() const { return GetGlobalParameters().input_file; }

    void set_input_file(const std::string &value) { GetGlobalParameters().input_file = value; }

    int num_points() const { return GetGlobalParameters().num_points; }

    void set_num_points(int value) { GetGlobalParameters().num_points = value; }

    int dim() const { return GetGlobalParameters().dim; }

    void set_dim(int value) { GetGlobalParameters().dim = value; }

    int num_clusters() const { return GetGlobalParameters().num_clusters; }

    void set_num_clusters(int value) { GetGlobalParameters().num_clusters = value; }

    // BIRCH
    int max_in_nodes() const { return GetGlobalParameters().max_in_nodes; }

    void set_max_in_nodes(int value) { GetGlobalParameters().max_in_nodes = value; }

    int max_leaf_nodes() const { return GetGlobalParameters().max_leaf_nodes; }

    void set_max_leaf_nodes(int value) { GetGlobalParameters().max_leaf_nodes = value; }

    double distance_threshold() const { return GetGlobalParameters().distance_threshold; }

    void set_distance_threshold(double value) { GetGlobalParameters().distance_threshold = value; }

    // StreamKM++
    int seed() const { return GetGlobalParameters().seed; }

    void set_seed(int value) { GetGlobalParameters().seed = value; }

    int coreset_size() const { return GetGlobalParameters().coreset_size; }

    void set_coreset_size(int value) { GetGlobalParameters().coreset_size = value; }

    // EDMStream
    double radius() const { return GetGlobalParameters().radius; }

    void set_radius(double value) { GetGlobalParameters().radius = value; }

    double delta() const { return GetGlobalParameters().delta; }

    void set_delta(double value) { GetGlobalParameters().delta = value; }

    double beta() const { return GetGlobalParameters().beta; }

    void set_beta(double value) { GetGlobalParameters().beta = value; }

    int buf_size() const { return GetGlobalParameters().buf_size; }

    void set_buf_size(int value) { GetGlobalParameters().buf_size = value; }

    double alpha() const { return GetGlobalParameters().alpha; }

    void set_alpha(double value) { GetGlobalParameters().alpha = value; }

    double lambda() const { return GetGlobalParameters().lambda; }

    void set_lambda(double value) { GetGlobalParameters().lambda = value; }

    // DBStream
    int clean_interval() const { return GetGlobalParameters().clean_interval; }

    void set_clean_interval(int value) { GetGlobalParameters().clean_interval = value; }

    double min_weight() const { return GetGlobalParameters().min_weight; }

    void set_min_weight(double value) { GetGlobalParameters().min_weight = value; }

    double base() const { return GetGlobalParameters().base; }

    void set_base(double value) { GetGlobalParameters().base = value; }

    // DStream
    double cm() const { return GetGlobalParameters().cm; }

    void set_cm(double value) { GetGlobalParameters().cm = value; }

    double cl() const { return GetGlobalParameters().cl; }

    void set_cl(double value) { GetGlobalParameters().cl = value; }

    double grid_width() const { return GetGlobalParameters().grid_width; }

    void set_grid_width(double value) { GetGlobalParameters().grid_width = value; }

    // DenStream
    unsigned int min_points() const { return GetGlobalParameters().min_points; }

    void set_min_points(unsigned int value) { GetGlobalParameters().min_points = value; }

    double epsilon() const { return GetGlobalParameters().epsilon; }

    void set_epsilon(double value) { GetGlobalParameters().epsilon = value; }

    double mu() const { return GetGlobalParameters().mu; }

    void set_mu(double value) { GetGlobalParameters().mu = value; }

    // Clustream
    int num_last_arr() const { return GetGlobalParameters().num_last_arr; }

    void set_num_last_arr(int value) { GetGlobalParameters().num_last_arr = value; }

    int time_window() const { return GetGlobalParameters().time_window; }

    void set_time_window(int value) { GetGlobalParameters().time_window = value; }

    int num_online_clusters() const { return GetGlobalParameters().num_online_clusters; }

    void set_num_online_clusters(int value) { GetGlobalParameters().num_online_clusters = value; }

    // SL-KMeans
    double delta_grid() const { return GetGlobalParameters().delta_grid; }

    void set_delta_grid(double value) { GetGlobalParameters().delta_grid = value; }

    int num_samples() const { return GetGlobalParameters().num_samples; }

    void set_num_samples(int value) { GetGlobalParameters().num_samples = value; }

    // Generic
    int landmark() const { return GetGlobalParameters().landmark; }

    void set_landmark(int value) { GetGlobalParameters().landmark = value; }

    int sliding() const { return GetGlobalParameters().sliding; }

    void set_sliding(int value) { GetGlobalParameters().sliding = value; }

    double outlier_distance_threshold() const { return GetGlobalParameters().outlier_distance_threshold; }

    void set_outlier_distance_threshold(double value) { GetGlobalParameters().outlier_distance_threshold = value; }

    int outlier_cap() const { return GetGlobalParameters().outlier_cap; }

    void set_outlier_cap(int value) { GetGlobalParameters().outlier_cap = value; }

    double outlier_density_threshold() const { return GetGlobalParameters().outlier_density_threshold; }

    void set_outlier_density_threshold(double value) { GetGlobalParameters().outlier_density_threshold = value; }

    double neighbor_distance() const { return GetGlobalParameters().neighbor_distance; }

    void set_neighbor_distance(double value) { GetGlobalParameters().neighbor_distance = value; }

    int k() const { return GetGlobalParameters().k; }

    void set_k(int value) { GetGlobalParameters().k = value; }

    int arr_rate() const { return GetGlobalParameters().arr_rate; }

    void set_arr_rate(int value) { GetGlobalParameters().arr_rate = value; }

    bool run_offline() const { return GetGlobalParameters().run_offline; }

    void set_run_offline(bool value) { GetGlobalParameters().run_offline = value; }

    bool run_eval() const { return GetGlobalParameters().run_eval; }

    void set_run_eval(bool value) { GetGlobalParameters().run_eval = value; }

    bool run_cmm() const { return GetGlobalParameters().run_cmm; }

    void set_run_cmm(bool value) { GetGlobalParameters().run_cmm = value; }

    bool run_pur() const { return GetGlobalParameters().run_pur; }

    void set_run_pur(bool value) { GetGlobalParameters().run_pur = value; }

    void Print() { GetGlobalParameters().Print(); }

    // Add other properties for the member variables of param_t

    // num_res
    int num_res() const { return GetGlobalParameters().num_res; }

    void set_num_res(int value) { GetGlobalParameters().num_res = value; }

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
    param_t globalParameters;
};

py::tuple run() {
    warning();
    // Access the parameters using the global `param` object
    param_t &param = GetGlobalParameters();
    // Run algorithm producing results.
    auto res = SESAME::RunBenchmark(param);
    res->Print();
    return py::make_tuple(res.purity, res.cmm);
}

// Initialize the module
PYBIND11_MODULE(benne, m) {
    m.doc() = "Module documentation string";

    py::class_<Parameters>(m,
                           "Parameters")
            .def(py::init<>())
            .def_property("algo", &Parameters::algo, &Parameters::set_algo, Parameters::algo_doc)
            .def_property("input_file", &Parameters::input_file, &Parameters::set_input_file,
                          Parameters::input_file_doc)
            .def_property("num_points", &Parameters::num_points, &Parameters::set_num_points,
                          Parameters::num_points_doc)
            .def_property("dim", &Parameters::dim, &Parameters::set_dim, Parameters::dim_doc)
            .def_property("num_clusters", &Parameters::num_clusters, &Parameters::set_num_clusters,
                          Parameters::num_clusters_doc)
                    // BIRCH
            .def_property("max_in_nodes", &Parameters::max_in_nodes, &Parameters::set_max_in_nodes,
                          Parameters::max_in_nodes_doc)
            .def_property("max_leaf_nodes", &Parameters::max_leaf_nodes, &Parameters::set_max_leaf_nodes,
                          Parameters::max_leaf_nodes_doc)
            .def_property("distance_threshold", &Parameters::distance_threshold, &Parameters::set_distance_threshold,
                          Parameters::distance_threshold_doc)
                    // StreamKM++
            .def_property("seed", &Parameters::seed, &Parameters::set_seed, Parameters::seed_doc)
            .def_property("coreset_size", &Parameters::coreset_size, &Parameters::set_coreset_size,
                          Parameters::coreset_size_doc)
                    // EDMStream
            .def_property("radius", &Parameters::radius, &Parameters::set_radius, Parameters::radius_doc)
            .def_property("delta", &Parameters::delta, &Parameters::set_delta, Parameters::delta_doc)
            .def_property("beta", &Parameters::beta, &Parameters::set_beta, Parameters::beta_doc)
            .def_property("buf_size", &Parameters::buf_size, &Parameters::set_buf_size, Parameters::buf_size_doc)
            .def_property("alpha", &Parameters::alpha, &Parameters::set_alpha, Parameters::alpha_doc)
            .def_property("lambda", &Parameters::lambda, &Parameters::set_lambda, Parameters::lambda_doc)
                    // DBStream
            .def_property("clean_interval", &Parameters::clean_interval, &Parameters::set_clean_interval,
                          Parameters::clean_interval_doc)
            .def_property("min_weight", &Parameters::min_weight, &Parameters::set_min_weight,
                          Parameters::min_weight_doc)
            .def_property("base", &Parameters::base, &Parameters::set_base, Parameters::base_doc)
                    // DStream
            .def_property("cm", &Parameters::cm, &Parameters::set_cm, Parameters::cm_doc)
            .def_property("cl", &Parameters::cl, &Parameters::set_cl, Parameters::cl_doc)
            .def_property("grid_width", &Parameters::grid_width, &Parameters::set_grid_width,
                          Parameters::grid_width_doc)
                    // DenStream
            .def_property("min_points", &Parameters::min_points, &Parameters::set_min_points,
                          Parameters::min_points_doc)
            .def_property("epsilon", &Parameters::epsilon, &Parameters::set_epsilon, Parameters::epsilon_doc)
            .def_property("mu", &Parameters::mu, &Parameters::set_mu, Parameters::mu_doc)
                    // Clustream
            .def_property("num_last_arr", &Parameters::num_last_arr, &Parameters::set_num_last_arr,
                          Parameters::num_last_arr_doc)
            .def_property("time_window", &Parameters::time_window, &Parameters::set_time_window,
                          Parameters::time_window_doc)
            .def_property("num_online_clusters", &Parameters::num_online_clusters, &Parameters::set_num_online_clusters,
                          Parameters::num_online_clusters_doc)
                    // SL-KMeans
            .def_property("delta_grid", &Parameters::delta_grid, &Parameters::set_delta_grid,
                          Parameters::delta_grid_doc)
            .def_property("num_samples", &Parameters::num_samples, &Parameters::set_num_samples,
                          Parameters::num_samples_doc)
                    // Generic
            .def_property("landmark", &Parameters::landmark, &Parameters::set_landmark, Parameters::landmark_doc)
            .def_property("sliding", &Parameters::sliding, &Parameters::set_sliding, Parameters::sliding_doc)
            .def_property("outlier_distance_threshold", &Parameters::outlier_distance_threshold,
                          &Parameters::set_outlier_distance_threshold, Parameters::outlier_distance_threshold_doc)
            .def_property("outlier_cap", &Parameters::outlier_cap, &Parameters::set_outlier_cap,
                          Parameters::outlier_cap_doc)
            .def_property("outlier_density_threshold", &Parameters::outlier_density_threshold,
                          &Parameters::set_outlier_density_threshold, Parameters::outlier_density_threshold_doc)
            .def_property("neighbor_distance", &Parameters::neighbor_distance, &Parameters::set_neighbor_distance,
                          Parameters::neighbor_distance_doc)
            .def_property("k", &Parameters::k, &Parameters::set_k, Parameters::k_doc)
            .def_property("arr_rate", &Parameters::arr_rate, &Parameters::set_arr_rate, Parameters::arr_rate_doc)
            .def_property("run_offline", &Parameters::run_offline, &Parameters::set_run_offline,
                          Parameters::run_offline_doc)
            .def_property("run_eval", &Parameters::run_eval, &Parameters::set_run_eval, Parameters::run_eval_doc)
            .def_property("run_cmm", &Parameters::run_cmm, &Parameters::set_run_cmm, Parameters::run_cmm_doc)
            .def_property("run_pur", &Parameters::run_pur, &Parameters::set_run_pur, Parameters::run_pur_doc);

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

