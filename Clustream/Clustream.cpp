#include "Clustream.h"
#include "KMeans.h"
#include <ctime>
#include "iostream"
#include <cstring>

CluStream::CluStream(int time_window, int q,  int t,int dimension,unsigned int m)
{
    this->time_window = time_window;
    this->q = q;
    this->t = t;
    this->dimension = dimension;
    this->m=m;
    points_fitted = 0;
    points_forgot = 0;
    points_merged = 0;
    this->microClusters=  new  Micro_clusters * [q];
    for(int i=0;i<q;i++)
    {
        microClusters[i]=NULL;
    }
    total_cluster=0;
    StartTime = clock();

}


void CluStream::init_kernels_offline( double **InitData,int size)
{
    //Using K-Means to cluster init data
    KMeans* kmeans = new KMeans(dimension,q);
    int* labels = new int[size];
    kmeans->SetInitMode(KMeans::InitUniform);
    kmeans->Cluster(InitData,size,labels);

    //Initialize original Micro clusters

    for (unsigned int i = 0;(int)i < size;i++)
    { int cluster_id=labels[i];
        if(microClusters[cluster_id]==NULL )
         {
            microClusters[cluster_id]= new Micro_clusters(InitData[i],
                                                          ((double)(clock()-StartTime))/CLOCKS_PER_SEC,t, m,dimension);

            total_cluster++;
         }
        microClusters[cluster_id]->insert(InitData[i], ((double)(clock()-StartTime))/CLOCKS_PER_SEC);
    }

}
void CluStream::online_cluster(double* data)
{
    // 0. Initialize
    if ((int)total_cluster<q )
    {
        std::cout<<"After initializing, the current number of cluster is not the set number..."<<std::endl;
      //  printf("The program took %f seconds to execute", ((double)(clock()-StartTime))/CLOCKS_PER_SEC);
        microClusters[total_cluster-1]=new Micro_clusters(data,((double)(clock()-StartTime))/CLOCKS_PER_SEC,t, m,dimension);

        return;
    }

    // 1. Determine closest kernel
    Micro_clusters* closest_cluster;
    double min_distance=double_max;

    for (unsigned int i = 0; i < q; i++) {
        double dist = distance(data,  microClusters[i]->get_centroid(), dimension);

        if (dist < min_distance) {
            closest_cluster = microClusters[i];
            min_distance = dist;
        }
    }

    // 2. Check whether instance fits into closest micro cluster
    double radius = 0;
    if (closest_cluster->cluster_num == 1) {
        // Special case: estimate radius by determining the distance to the
        // next closest cluster
        radius = double_max;
        double * centroid = closest_cluster->get_centroid();
        for (unsigned int i = 0; i < q; i++) { //O(n)
            if (&closest_cluster[i] == closest_cluster) {
                continue;
            }

            double dist = distance(microClusters[i]->get_centroid(), centroid,dimension);
            radius = std::min(dist, radius);
        }
    }

    else
        radius = closest_cluster->get_radius();

    if (min_distance < radius) {
        // Date fits, put into kernel and be happy
#ifdef DEBUG
      cout<<"This data fits"<<endl;
#endif
        points_fitted++;
        closest_cluster->insert(data, ((double)(clock()-StartTime))/CLOCKS_PER_SEC);
        return;
    }

/* 3. Date does not fit, we need to free
 * some space to insert a new micro cluster :
 * 1 delete oldest one & create a new cluster OR
 * 2 Merge two clusters & create a new cluster
 * */

    long threshold = ((double)(clock()-StartTime))/CLOCKS_PER_SEC - time_window; // Kernels before this can be forgotten

    // 3.1 Try to forget old kernels
    for (unsigned int i = 0; i <q; i++) {
        if (microClusters[i]->get_relevance_stamp() < threshold) {

            microClusters[i] = new Micro_clusters(data, ((double)(clock()-StartTime))/CLOCKS_PER_SEC, t, m, dimension);

#ifdef DEBUG
        cout<<"forgot kernel"<<endl;
#endif
            points_forgot++;
            return;
        }
    }
    // 3.2 Merge two closest  clusters
    int closest_a = 0;
    int closest_b = 0;
    min_distance = double_max;
    for (unsigned int i = 0; i < q; i++) { //O(n(n+1)/2)
        double * centroid_a = microClusters[i]->get_centroid();
        for (unsigned int j = i + 1; j < q; j++) {
            double dist = distance(centroid_a, microClusters[j]->get_centroid(),dimension);
            if (dist < min_distance) {
                min_distance = dist;
                closest_a = i;
                closest_b = j;
            }
        }
    }
#ifdef DEBUG
   cout<<" merged kernel"<<endl;
#endif
    points_merged++;
    microClusters[closest_a]->merge(microClusters[closest_b]);

    microClusters[closest_b] =  new Micro_clusters(data, ((double)(clock()-StartTime))/CLOCKS_PER_SEC, t, m, dimension);;

}



double CluStream::distance(double* a, double* b, unsigned int dim) {
    double temp = 0;
    for (unsigned int i = 0;i < dim;i++) {
        double diff = b[i] - a[i];
        temp += diff * diff;
    }
    return sqrt(temp);
}
