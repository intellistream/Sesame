#include "Python/Benne.hpp"
#include "Python/Birch.hpp"

namespace py = pybind11;
using namespace py::literals;
using namespace SESAME;
using namespace std;

PYBIND11_MODULE(pysame, m) {
  py::class_<Benne>(m, "Benne")
      .def(py::init<double, int, int, int>(), py::arg("threshold") = 0.5,
           py::arg("branching_factor") = 50, py::arg("n_clusters") = 3,
           py::arg("dim") = 2)
      .def("fit", &Benne::fit, py::return_value_policy::reference_internal)
      .def("fit_predict", &Benne::fit_predict)
      .def("get_params", &Benne::get_params)
      .def("partial_fit", &Benne::partial_fit,
           py::return_value_policy::reference_internal)
      .def("predict", &Benne::predict)
      .def("set_output", &Benne::set_output)
      .def("set_params", &Benne::set_params);
  py::class_<Birch>(m, "Birch")
      .def(py::init<double, int, int, int>(), py::arg("threshold") = 0.5,
           py::arg("branching_factor") = 50, py::arg("n_clusters") = 3,
           py::arg("dim") = 2)
      .def("fit", &Birch::fit, py::return_value_policy::reference_internal)
      .def("fit_predict", &Birch::fit_predict)
      .def("get_params", &Birch::get_params)
      .def("partial_fit", &Birch::partial_fit,
           py::return_value_policy::reference_internal)
      .def("predict", &Birch::predict)
      .def("set_output", &Birch::set_output)
      .def("set_params", &Birch::set_params);
}