#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

using namespace pybind11::literals;
namespace py = pybind11;

class IBirch {
public:
    IBirch(double threshold, int branching_factor, int n_clusters, bool compute_labels) {
        this->threshold = threshold;
        this->branching_factor = branching_factor;
        this->n_clusters = n_clusters;
        this->compute_labels = compute_labels;
    }

    void fit(py::array_t<double> X) {
        // Do something
    }

    py::array_t<int> fit_predict(py::array_t<double> X) {
        // Do something
        return py::array_t<int>();
    }

    py::dict get_params() {
        return py::dict("threshold"_a=threshold, "branching_factor"_a=branching_factor, "n_clusters"_a=n_clusters, "compute_labels"_a=compute_labels);
    }

    void partial_fit(py::array_t<double> X) {
        // Do something
    }

    py::array_t<int> predict(py::array_t<double> X) {
        // Do something
        return py::array_t<int>();
    }

    void set_output(bool output) {
        this->output = output;
    }

    void set_params(py::dict params) {
        // Do something
    }
private:
    double threshold;
    int branching_factor;
    int n_clusters;
    bool compute_labels;
    bool output;
};

PYBIND11_MODULE(sesame_python, m) {
    py::class_<IBirch>(m, "Birch")
        .def(py::init<double, int, int, bool>())
        .def("fit", &IBirch::fit)
        .def("fit_predict", &IBirch::fit_predict)
        .def("get_params", &IBirch::get_params)
        .def("partial_fit", &IBirch::partial_fit)
        .def("predict", &IBirch::predict)
        .def("set_output", &IBirch::set_output)
        .def("set_params", &IBirch::set_params);
}