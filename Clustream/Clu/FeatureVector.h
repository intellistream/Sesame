#pragma once
#include <vector>


/*

Just tries for constructing feature vector class


*/
const double RADIUS_FACTOR = 1.8;
const double EPSILON = 0.00005;
const double MIN_VARIANCE = 1e-50;

typedef std::vector<double> Point; //define data point vector [xi1,xi2,...,xid] in double 

class FeatureVector{
public:
	unsigned int n;
	Point ls;
	Point ss;
	double lst;
	double sst;


};