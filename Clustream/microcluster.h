

#ifndef CLUT_MICROCLUSTER_H
#define CLUT_MICROCLUSTER_H



#pragma once

#include <vector>
#include <cmath>

#include <cassert>



class Micro_clusters{
public:
    const double RADIUS_FACTOR = 1.8;
    const double EPSILON = 0.00005;
    const double MIN_VARIANCE = 1e-50;//Set by user? Suppose not hard-coded
    double t;
    double *  centroid;
    unsigned int m;
    double *  CF1x;//  the sum of the data values for each dimension
    double *  CF2x;// the sum of the squares of  the data values for each dimension
    double CF1t; //  the sum of the time stamps Ti~... Tin
    double CF2t;//the sum of the squares of the time stamps  Til... Tin
    unsigned int cluster_num=0; //number of data point in the clusters

    Micro_clusters(double *  datapoint, double timestamp, double t, unsigned int m,unsigned int dimension);
    void insert(double  *  datapoint,double timestamp);

    void merge(Micro_clusters * other);

    double get_relevance_stamp();
    double get_mu_time();
    double get_sigma_time();
    double get_quantile(double z);
    double get_radius();
    double get_deviation();
    int get_Clusterid();
    int get_dimension()  {return dimension;}
    double *  get_centroid();
    double get_inclusion_probability(double *  datapoint);
    double *  get_variance_vector();
    double calc_Centroid_distance(double *  datapoint);

private:


    unsigned int dimension;
    double inverse_error(double x);

};



#endif //CLUT_MICROCLUSTER_H
