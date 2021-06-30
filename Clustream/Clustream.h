#ifndef CLUSTREAM_CLUSTREAM_H
#define CLUSTREAM_CLUSTREAM_H
#include <algorithm>
#include <cmath>
#include <cassert>
#include <limits>
#include <ctime>
#include <cstdio>
#include"microcluster.h"

/*
Args:\n\
	h (int): Time of Landmark Window parameter ,
	q (int): Maximum number of micro clusters to use,
    m (int): The last m arriving in the micro clusters,
	t (int): Multiplier for the Cluster radius,(the maximal boundary factor,calculated by the
	  RMS deviation of the data points in Mp from the centroid in the paper),

Attributes:\n\
	timestamp (int): current timestamp
	points_fitted (int): number of data points that entered the 'fit' case
	points_forgot (int): number of data points that entered the 'forgot micro cluster' case
	points_merged (int): number of data points that entered the 'merged micro cluster' case"

*/


const double double_max = std::numeric_limits<double>::max();


class CluStream {
public:

    Micro_clusters ** microClusters;
    unsigned int m;
    int time_window;// Length of Landmark Window
    int q; //total number of micro clusters
    int t;//radius factor
    long unsigned int points_fitted;
    long unsigned int points_forgot;
    long unsigned int points_merged;
    clock_t StartTime;


    CluStream(int time_window, int q, int t,int dimension,unsigned int m);
    void init_kernels_offline(double ** InitData,int size);
    void online_cluster(double* data);
    double distance(double* a, double* b, unsigned int dim);

   // double *  get_kernel_centers();
private:
    unsigned int dimension;
    unsigned int total_cluster;


};




#endif //CLUSTREAM_CLUSTREAM_H
