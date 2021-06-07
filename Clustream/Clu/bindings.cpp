//#include "Clustream.h"
#include<iostream>
using namespace std;
#define INIT_DOC \
"CluStream data stream clustering algorithm implementation\n\
\n\
Args:\n\
	h (int): Range of the window\n\
	m (int): Maximum number of micro kernels to use\n\
	t (int): Multiplier for the kernel radius\n\
Attributes:\n\
	time_window (int) : h\n\
	m (int): m\n\
	t (int): t\n\
	timestamp (int): current timestamp\n\
	points_fitted (int): number of datapoints that entered the 'fit' case\n\
	points_forgot (int): number of datapoints that entered the 'forgot kernel' case\n\
	points_merged (int): number of datapoints that entered the 'merged kernel' case"
#define BATCH_ONLINE_CLUSTER_DOC \
"Process a chunk of datapoints all at once\n\
\n\
Args:\n\
	batch (ndarray): the datapoint chunk matrix\n"
#define GET_KERNEL_CENTERS_DOC \
"Get current microclusters centroids\n\
\n\
Returns:\n\
	ndarray\n"
#define INIT_KERNELS_OFFLINE_DOC \
"initialize m kernels with its coresponding initpoints and clustering centers\n\
\n\
Args:\n\
	cluster_centers (ndarray): the offline clustering of the initpoints\n\
	initpoints (ndarray): datapoints to initialize \n"

int main()
{
      cout << "INIT_DOC:,\n, " << INIT_DOC << endl;
        cout << "BATCH_ONLINE_CLUSTER_DOC:\n, " << BATCH_ONLINE_CLUSTER_DOC << endl;
      
}
/*
PYBIND11_MODULE(clustream, m) {
    py::class_<CluStream>(m, "CluStream", INIT_DOC)
        .def(py::init<int, int, int>(), py::arg("h") = 100, py::arg("m") = 1000, py::arg("t") = 2)
        .def_readonly("m", &CluStream::m)
        .def_readonly("time_window", &CluStream::time_window)
        .def_readonly("t", &CluStream::t)
        .def_readonly("timestamp", &CluStream::timestamp)
        .def_readonly("points_fitted", &CluStream::points_fitted)
        .def_readonly("points_forgot", &CluStream::points_forgot)
        .def_readonly("points_merged", &CluStream::points_merged)
        .def("batch_online_cluster", &CluStream::batch_online_cluster, BATCH_ONLINE_CLUSTER_DOC)
        .def("partial_fit", &CluStream::batch_online_cluster, BATCH_ONLINE_CLUSTER_DOC)
        .def("get_kernel_centers", &CluStream::get_kernel_centers, GET_KERNEL_CENTERS_DOC)
        .def("get_partial_cluster_centers", &CluStream::get_kernel_centers, GET_KERNEL_CENTERS_DOC)
        .def("init_kernels_offline", &CluStream::init_kernels_offline, INIT_KERNELS_OFFLINE_DOC)
        ;
}

*/
