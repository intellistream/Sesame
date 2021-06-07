#pragma once
#include "microcluster.h"
#include <vector>
#include <cmath>
#include <cassert>
#include <limits>
#include <cstdio>

const double double_max=std::numeric_limits<double>::max();

double distance(Point& a,Point& b);

class CluStream{
public:
	std::vector<Microclusters> microclusters;
	int time_window;
	int m;
	int t;

	CluStream(int h,int m,int t);
	void offline_cluster(Point& datapoint,long timestamp);
};