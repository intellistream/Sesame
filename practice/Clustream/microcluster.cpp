#include "microcluster.h"
#include <cstring>
#include <iostream>


//Initialize a new micro cluster
Micro_clusters::Micro_clusters(double *  datapoint, double timestamp, double t, unsigned int m,unsigned int dimension)
{
    this->t=t;
    this->m=m;
    this->dimension=dimension;
    cluster_num=1;
    CF1x=new double[dimension];
    CF2x=new double[dimension];
    for (unsigned int i = 0;i < dimension;i++) {

        double data = datapoint[i];

        CF1x[i] = data;
        CF2x[i] = data * data;
       // std::cout<<CF1x[i]<<" "<<CF2x[i]<<std::endl;
    }

    CF1t=timestamp;
    CF2t=timestamp*timestamp;

    centroid=get_centroid();
}


//insert a new data point from input data stream(test data sets)

void Micro_clusters::insert(double * datapoint,double timestamp)
{
    cluster_num++;
    for(int i=0;i<dimension;i++)
    {
        double data=datapoint[i];
        CF1x[i]+=data;
        CF2x[i]+=data*data;
    }

    CF1t+=timestamp;
    CF2t+=timestamp*timestamp;
    centroid=get_centroid();

}

//merge two micro-clusters

void Micro_clusters::merge(Micro_clusters * other){
    cluster_num+=other->cluster_num;

    for(int i=0;i<dimension;i++)
    {
        CF1x[i]+=other->CF1x[i];
        CF2x[i]+=other->CF2x[i];
    }
    CF1t+=other->CF1t;
    CF2t+=other->CF2t;
    centroid=get_centroid();
}


//obtain relevance stamp of a cluster to judge whether it needs to be deleted

double Micro_clusters::get_relevance_stamp()
{
    if(cluster_num<(2*m))
        return get_mu_time();
    return get_mu_time() + get_sigma_time() * get_quantile( ((double)m)/(2*cluster_num) );
}

//mean_timestamp

double Micro_clusters::get_mu_time(){
    return CF1t/cluster_num;
}

//standard_deviation_timestamp

double Micro_clusters::get_sigma_time()
{
    return sqrt(CF2t/cluster_num - (CF1t/cluster_num)*(CF1t/cluster_num));
}


double Micro_clusters::get_quantile(double z)
{
    assert( z >= 0 && z <= 1 );
    return sqrt( 2 ) * inverse_error( 2*z - 1 );
}


double Micro_clusters::get_radius(){
    if(cluster_num==1)
        return 0;
    if(t=0)
        t=RADIUS_FACTOR;
    return get_deviation()*t;
}

//calculate RMS deviation very confused

double Micro_clusters::get_deviation(){
    double * variance=get_variance_vector();
    double sum_of_deviation=0;
    for(int i=0;i<dimension;i++){
        sum_of_deviation += sqrt(variance[i]);
    }
    return  sum_of_deviation/dimension;
}

//calculate centroid of a  cluster

double * Micro_clusters::get_centroid(){
    if(cluster_num==1)
        return CF1x;
    double * datapoint= new double [dimension];
    for(int i=0;i<dimension;i++){
        datapoint[i]=CF1x[i]/cluster_num;
    }
    return datapoint;
}

double Micro_clusters::get_inclusion_probability(double * datapoint){
    if(cluster_num==1)
    {
        double distance=0;
        for(int i=0;i<dimension;i++){
            double d=CF1x[i]-datapoint[i];
            distance+=d*d;
        }
        distance=sqrt(distance);
        if(distance<EPSILON)
        return 1;
        return 0;
    }
    else{
        double dist=calc_Centroid_distance(datapoint);
        if(dist<=get_radius())
            return 0;
        else
            return 1;
    }
}

//Calculate the standard deviation of vector in micro clusters

double * Micro_clusters::get_variance_vector(){
    double  * datapoint;
    datapoint=new double [dimension];
    for(int i=0;i<dimension;i++){
        double linear_sum=CF1x[i];
        double squared_sum=CF2x[i];

        double ls_div_n=			linear_sum/cluster_num;
        double ls_div_squared=	ls_div_n*ls_div_n;
        double ss_div_n=		squared_sum/cluster_num;

        datapoint[i]=ss_div_n-ls_div_squared;

        if(datapoint[i]<=0.0)
            datapoint[i]=MIN_VARIANCE;
    }
    return datapoint;
}


// calculate the distance between input data point and centroid of micro-clusters

double Micro_clusters::calc_Centroid_distance(double * datapoint){

    double temp=0;

    for(int i=0;i<dimension;i++){
        double diff=centroid[i]-datapoint[i];
        temp+=(diff*diff);
    }
    return sqrt(temp);
}

double Micro_clusters::inverse_error(double x){
    double z = sqrt(M_PI) * x;
    double res = (z) / 2;

    double z2 = z * z;
    double zProd = z * z2; // z^3
    res += (1.0 / 24) * zProd;

    zProd *= z2;  // z^5
    res += (7.0 / 960) * zProd;

    zProd *= z2;  // z^7
    res += (127 * zProd) / 80640;

    zProd *= z2;  // z^9
    res += (4369 * zProd) / 11612160;

    zProd *= z2;  // z^11
    res += (34807 * zProd) / 364953600;

    zProd *= z2;  // z^13
    res += (20036983 * zProd) / 797058662400;

    return res;
}