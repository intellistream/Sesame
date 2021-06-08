#include "microcluster.h"

Microclusters::Microclusters(DataPoint& datapoint,long timestamp, double t, unsigned int m):ls(datapoint.size()),ss(datapoint.size()){
	this->t=t;
	this->q=q;
	n=1;
	for(int i=0;i<datapoint.size();i++){//iterate on every dimension in the data point
		double data=datapoint[i];
		ls[i]=data;
		ss[i]=data*data;
	}
	lst=timestamp;
	sst=timestamp*timestamp;
	centroid=get_centroid();
}
/*
 *insert a new data point from input data stream(test data sets)
 */
void Microclusters::insert(DataPoint& datapoint,long timestamp){ 
	n++;
	for(int i=0;i<datapoint.size();i++){
		double data=datapoint[i];
		ls[i]+=data;
		ss[i]+=data*data;
	}
	lst+=timestamp;
	sst+=timestamp*timestamp;
	centroid=get_centroid();
}
void Microclusters::add(Microclusters& other){//merge two microclusters
	n+=other.n;
	for(int i=0;i<other.ls.size();i++){
		ls[i]+=other.ls[i];
		ss[i]+=other.ss[i];
	}
	lst+=other.lst;
	sst+=other.sst;
	centroid=get_centroid();
}


double Microclusters::get_relevance_stamp(){
	if(n<(2*q))
		return get_mu_time();
	return get_mu_time() + get_sigma_time() * get_quantile( ((double)q)/(2*n) );
}
double Microclusters::get_mu_time(){
	return lst/n;
}
double Microclusters::get_sigma_time(){
	return sqrt(sst/n - (lst/n)*(lst/n));
}
double Microclusters::get_quantile(double z){
	assert( z >= 0 && z <= 1 );
	return sqrt( 2 ) * inverse_error( 2*z - 1 );
}
double Microclusters::get_radius(){
	if(n==1)
		return 0;
	return get_deviation()*RADIUS_FACTOR;
}
double Microclusters::get_deviation(){
	DataPoint variance=get_variance_vector();
	double sum_of_deviation=0;
	for(int i=0;i<variance.size();i++){
		sum_of_deviation+=sqrt(variance[i]);
	}
	return sum_of_deviation/variance.size();
}
DataPoint Microclusters::get_centroid(){
	if(n==1)
		return ls;
	DataPoint ans(ls.size());
	for(int i=0;i<ls.size();i++){
		ans[i]=ls[i]/n;
	}
	return ans;
}
double Microclusters::get_inclusion_probability(DataPoint& datapoint){
	if(n==1){
		double distance=0;
		for(int i=0;i<ls.size();i++){
			double d=ls[i]-datapoint[i];
			distance+=d*d;
		}
		distance=sqrt(distance);
		if(distance<EPSILON)
			return 1;
		return 0;
	}
	else{
		double dist=calc_normalized_distance(datapoint);
		if(dist<=get_radius())
			return 0;
		else 
			return 1;
	}
}
DataPoint Microclusters::get_variance_vector(){
	DataPoint ans(ls.size());
	for(int i=0;i<ls.size();i++){
		double linear_sum=ls[i];
		double squared_sum=ss[i];

		double ls_div_n=			linear_sum/n;
		double ls_div_squared=	ls_div_n*ls_div_n;
		double ss_div_n=		squared_sum/n;

		ans[i]=ss_div_n-ls_div_squared;

		if(ans[i]<=0.0)
			ans[i]=MIN_VARIANCE;
	}
	return ans;
}
double Microclusters::calc_normalized_distance(DataPoint& point){
	// variance=get_variance_vector();
	double ans=0;

	for(int i=0;i<centroid.size();i++){
		double diff=centroid[i]-point[i];
		ans+=(diff*diff); // variance[i];
	}
	return sqrt(ans);
}
double Microclusters::inverse_error(double x){
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