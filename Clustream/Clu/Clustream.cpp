#include "clustream.h"
 #include "algorithm"
CluStream::CluStream(int h,int m,int t){
	time_window=h;//time horizon of window
	this->q=q;//total number of microclusters 
	this->t=t;
}
void CluStream::offline_cluster(DataPoint& datapoint,long timestamp){
	// 0. Initialize
	if(microclusters.size()!=q){
		microclusters.push_back(Microclusters(datapoint,timestamp,t,q));
		return;
	}
	// 1. Determine closest microcluster
	Microclusters* closest_microcluster = NULL;
	double min_distance = double_max;
	for ( int i = 0; i < microclusters.size(); i++ ) { //O(n)
		double dist = distance(datapoint, microclusters[i].centroid);
		if ( dist < min_distance ) {
			closest_microcluster = &microclusters[i];
			min_distance = dist;
		}
	}
	// 2. Check whether instance fits into closestKernel
	double radius=0;
	if ( closest_microcluster->n == 1 ) {
			// Special case: estimate radius by determining the distance to the
			// next closest cluster
			radius = double_max;
			DataPoint centroid = closest_microcluster->centroid;
			for ( int i = 0; i < microclusters.size(); i++ ) { //O(n)
				if ( &microclusters[i] == closest_microcluster ) {
					continue;
				}

				double dist = distance(microclusters[i].centroid, centroid );
				radius = std::min( dist, radius );
			}
		} else {
			radius = closest_microcluster->get_radius();
		}

		if ( min_distance < radius ) {
			// Date fits, put into kernel and be happy
			printf("%ld fits\n",timestamp);
			closest_microcluster->insert( datapoint, timestamp );
			return;
		}

		// 3. Date does not fit, we need to free
		// some space to insert a new microcluster
		long threshold = timestamp - time_window; // Microclusters before this can be forgotten

		// 3.1 Try to forget old kernels
		for ( int i = 0; i < microclusters.size(); i++ ) {
			if ( microclusters[i].get_relevance_stamp() < threshold ) {
				microclusters[i] = Microclusters( datapoint, timestamp, t, q );
				printf("%ld forgot microcluster\n",timestamp);
				return;
			}
		}
		// 3.2 Merge closest two microclusters
		int closest_a = 0;
		int closest_b = 0;
		min_distance = double_max;
		for ( int i = 0; i < microclusters.size(); i++ ) { //O(n(n+1)/2)
			DataPoint center_a = microclusters[i].centroid;
			for ( int j = i + 1; j < microclusters.size(); j++ ) {
				double dist = distance( center_a, microclusters[j].centroid );
				if ( dist < min_distance ) {
					min_distance = dist;
					closest_a = i;
					closest_b = j;
				}
			}
		}
		printf("%ld merged microcluster\n",timestamp);
		microclusters[closest_a].add( microclusters[closest_b] );
		microclusters[closest_b] = Microclusters( datapoint, timestamp, t,  q );
}
//distance(a,b)= sqrt((a1-b1)^2+(a2-b2)^2+...+(ad-bd)^2) d: number of dimensions in a and b
double distance(DataPoint& a,DataPoint& b){
	double ans=0;
	for(int i=0;i<a.size();i++){
		double diff=b[i]-a[i];
		ans+=diff*diff;
	}
	return sqrt(ans);
}