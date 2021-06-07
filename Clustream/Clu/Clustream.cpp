#include "clustream.h"
 #include "algorithm"
CluStream::CluStream(int h,int m,int t){
	time_window=h;
	this->m=m;
	this->t=t;
}
void CluStream::offline_cluster(Point& datapoint,long timestamp){
	// 0. Initialize
	if(microclusters.size()!=m){
		microclusters.push_back(Microclusters(datapoint,timestamp,t,m));
		return;
	}
	// 1. Determine closest kernel
	Microclusters* closest_kernel = NULL;
	double min_distance = double_max;
	for ( int i = 0; i < microclusters.size(); i++ ) { //O(n)
		double dist = distance(datapoint, microclusters[i].center);
		if ( dist < min_distance ) {
			closest_kernel = &microclusters[i];
			min_distance = dist;
		}
	}
	// 2. Check whether instance fits into closestKernel
	double radius=0;
	if ( closest_kernel->n == 1 ) {
			// Special case: estimate radius by determining the distance to the
			// next closest cluster
			radius = double_max;
			Point center = closest_kernel->center;
			for ( int i = 0; i < microclusters.size(); i++ ) { //O(n)
				if ( &microclusters[i] == closest_kernel ) {
					continue;
				}

				double dist = distance(microclusters[i].center, center );
				radius = std::min( dist, radius );
			}
		} else {
			radius = closest_kernel->get_radius();
		}

		if ( min_distance < radius ) {
			// Date fits, put into kernel and be happy
			printf("%ld fits\n",timestamp);
			closest_kernel->insert( datapoint, timestamp );
			return;
		}

		// 3. Date does not fit, we need to free
		// some space to insert a new kernel
		long threshold = timestamp - time_window; // Kernels before this can be forgotten

		// 3.1 Try to forget old kernels
		for ( int i = 0; i < microclusters.size(); i++ ) {
			if ( microclusters[i].get_relevance_stamp() < threshold ) {
				microclusters[i] = Microclusters( datapoint, timestamp, t, m );
				printf("%ld forgot kernel\n",timestamp);
				return;
			}
		}
		// 3.2 Merge closest two kernels
		int closest_a = 0;
		int closest_b = 0;
		min_distance = double_max;
		for ( int i = 0; i < microclusters.size(); i++ ) { //O(n(n+1)/2)
			Point center_a = microclusters[i].center;
			for ( int j = i + 1; j < microclusters.size(); j++ ) {
				double dist = distance( center_a, microclusters[j].center );
				if ( dist < min_distance ) {
					min_distance = dist;
					closest_a = i;
					closest_b = j;
				}
			}
		}
		printf("%ld merged kernel\n",timestamp);
		microclusters[closest_a].add( microclusters[closest_b] );
		microclusters[closest_b] = Microclusters( datapoint, timestamp, t,  m );
}

double distance(Point& a,Point& b){
	double ans=0;
	for(int i=0;i<a.size();i++){
		double diff=b[i]-a[i];
		ans+=diff*diff;
	}
	return sqrt(ans);
}