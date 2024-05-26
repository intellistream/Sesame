#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include "Algorithm/Algorithm.hpp"
#include "Algorithm/AlgorithmFactory.hpp"
#include "Algorithm/DataStructure/GenericFactory.hpp"

#include <iostream>

using namespace pybind11::literals;
using namespace SESAME;
using namespace std;
namespace py = pybind11;

class Benne {
public:
  Benne(double threshold, int branching_factor, int n_clusters, int dim)
      : threshold(threshold), branching_factor(branching_factor),
        n_clusters(n_clusters), dim(dim) {
    param.algo = AlgoType::BenneType;
    param.distance_threshold = threshold;
    param.landmark = 10;
    param.max_in_nodes = branching_factor;
    param.max_leaf_nodes = branching_factor;
    param.num_clusters = n_clusters;
    param.dim = dim;
    algo = AlgorithmFactory::create(param);
    sinkPtr = GenericFactory::New<DataSink>(param);
    auto barrierPtr = UtilityFunctions::createBarrier(1);
    sinkPtr->setBarrier(barrierPtr);
    sinkPtr->start(0);
    algo->Init();
  }

  void fit(py::array_t<double> X) {
    py::buffer_info buf = X.request();

    if (buf.ndim != 2)
      throw std::runtime_error("numpy.ndarray dims must be 2!");

    double *ptr = static_cast<double *>(buf.ptr);

    auto num_elements = buf.shape[0] * buf.shape[1];
    auto num_vectors = num_elements / dim;
    for (int i = 0; i < num_vectors; i++) {
      SESAME::PointPtr point = std::make_shared<SESAME::Point>(dim, id++);
      for (int j = 0; j < dim; j++) {
        point->data()[j] = ptr[i * dim + j];
      }
      algo->RunOnline(point);
    }
    return;
  }

  py::array_t<int> fit_predict(py::array_t<double> X) {
    py::buffer_info buf = X.request();

    if (buf.ndim != 2)
      throw std::runtime_error("numpy.ndarray dims must be 2!");

    double *ptr = static_cast<double *>(buf.ptr);

    auto num_elements = buf.shape[0] * buf.shape[1];
    auto num_vectors = num_elements / dim;
    for (int i = 0; i < num_vectors; i++) {
      SESAME::PointPtr point =
          std::make_shared<SESAME::Point>(dim, id++, 1.0, 0.0, 0);
      for (int j = 0; j < dim; j++) {
        point->data()[j] = ptr[i * dim + j];
      }
      inputs.push_back(point);
      algo->RunOnline(point);
    }

    algo->RunOffline(sinkPtr);
    std::vector<PointPtr> &results = sinkPtr->getResults(), predicts;
    UtilityFunctions::groupByCenters(inputs, results, predicts, param.dim);
    // cout << "results size: " << results.size() << endl;
    // cout << "inputs size: " << inputs.size() << endl;
    // cout << "predicts size: " << predicts.size() << endl;
    std::vector<int> labels;
    for (auto &point : predicts) {
      labels.push_back(point->clu_id);
    }
    return py::array_t<int>(labels.size(), labels.data());
  }

  py::dict get_params() {
    return py::dict("threshold"_a = threshold,
                    "branching_factor"_a = branching_factor,
                    "n_clusters"_a = n_clusters);
  }

  void partial_fit(py::array_t<double> X) {
    throw std::runtime_error("Not supported");
  }

  py::array_t<int> predict(py::array_t<double> X) {
    throw std::runtime_error("Not supported");
  }

  void set_output(bool output) { throw std::runtime_error("Not supported"); }

  void set_params(py::dict params) {
    throw std::runtime_error("Not supported");
  }

private:
  double threshold;
  int branching_factor;
  int n_clusters;
  int dim;
  bool output;
  uint64_t id = 0;
  SESAME::SesameParam param;
  SESAME::AlgorithmPtr algo;
  std::vector<PointPtr> inputs;
  DataSinkPtr sinkPtr;
};

PYBIND11_MODULE(sesame_python, m) {
  py::class_<Benne>(m, "Benne")
      .def(py::init<double, int, int, int>(), py::arg("threshold") = 0.5,
           py::arg("branching_factor") = 50, py::arg("n_clusters") = 3,
           py::arg("dim") = 2)
      .def("fit", &Benne::fit)
      .def("fit_predict", &Benne::fit_predict)
      .def("get_params", &Benne::get_params)
      .def("partial_fit", &Benne::partial_fit)
      .def("predict", &Benne::predict)
      .def("set_output", &Benne::set_output)
      .def("set_params", &Benne::set_params);
}